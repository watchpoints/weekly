---
title: 大厂面试题：如果判断一个服务是否可用
date: 2025-07-26
description: do book
draft: false
tags: 
categories:
  - do book
---

# 一、大纲--提出问题



![boost:intrusive_ptr vs std::shared_ptr](https://s2.loli.net/2025/09/01/nh3IQ6DxJs5SNAG.png)



### 核心疑问1 ：为什么 boost::intrusive_ptr 比 std::shared_ptr快

-  **intrusive_ptr 性能更高主要是缓存一致性，而不是原子操作本身。**
### 核心疑问2 ：boost::intrusive_ptr 构造，怎么能调用直接用户自定义的 intrusive_ptr_add_ref 接口,这个2个类非继承，非组合关系？【万恶的c++语法】


- intrusive_ptr_add_ref 接口函数 ​​并不是 intrusive_ptr 类的成员函数​​，而是独立的全局函数（或友元函数）
- 在 用户自定义类 的设计中，intrusive_ptr_add_ref 为什么必须声明友元函数 可以通过 friend 关键字让非成员函数访问类的私有成员
- ADL（Argument-Dependent Lookup，参数依赖查找）万恶的c++语法
- **模板延迟实例化**：`intrusive_ptr<T>` 里调用 `intrusive_ptr_add_ref(px)` 时，只有在 `T` 被确定时，编译器才查找函数。
- **好处**：
- `intrusive_ptr` 可以管理任何类型，只要该类型提供 接口`intrusive_ptr_add_ref`  `intrusive_ptr_release接口`。
    
- 如果某些类型没有这些函数，只要你不实例化模板，也不会报错。


### 核心疑问3  std::make_shared 为什么自定义删除器deleter情况下为什么退化为两次分配?

| 删除器类型             | `sizeof(D)` | 说明         |
| ----------------- | ----------- | ---------- |
| 函数指针              | 8（64 位系统）   | 大小固定       |
| 无捕获 lambda        | 1（空类最小大小）   | 编译器优化空类    |
| 有捕获 lambda / 函数对象 | 捕获成员大小      | 编译器生成的函数对象 |
| `std::function`   | 固定（24~32字节） | 内部可能堆分配    |

# 二、疑问1 ：为什么 boost::intrusive_ptr 比 std::shared_ptr快

## 2.1 从数据结构看区别


设计哲学是：
- ​**​智能指针只负责所有权管理
- 引用计数逻辑由用户定义​**​。
- 这种设计使其成为唯一能直接管理 **侵入式对象**的 C++ 智能指针


| 特性         | `std::shared_ptr`               | `boost::intrusive_ptr`                       |
| ---------- | ------------------------------- | -------------------------------------------- |
| **引用计数存储** | 单独的 **控制块**（control block）      | 引用计数嵌入**到对象本身**                              |
| **内存分配**   | 至少两次：一次分配对象，一次分配控制块             | 一次分配（**对象 + 引用计数一起**）                        |
| **原子操作**   | 控制块中维护 `atomic<size_t>`         | 对象内部维护 `atomic<size_t>`                      |
| **缓存一致性**  | 访问两块不同的内存 → 可能导致 **cache miss** | 对象 + 引用计数更可能命中同一 cache line                  |
| **侵入性**    | 不需要修改对象定义                       | **需要在对象中定义** `intrusive_ptr_add_ref/release` |
| **循环引用**   | 同样会有问题【存在同样问题】                  | 同样会有问题【存在同样问题】                               |
| **典型场景**   | 应用层、通用库                         | 系统内核、性能敏感路径                                  |

>核心结论1：boost:intrusive_ptr 比 shared_ptr 快，
>核心原因是缓存一致性（缓存局部性），
>而不是原子更新本身。


## 2.2 intrusive_ptr 用法


```c++
//使用intrusive_ptr只能指针来管理  ObjectCacher::BufferHead类
boost::intrusive_ptr<ceph::BufferHead> bh(new ceph::BufferHead);

namespace ceph {
//用户自定义 BufferHead类 继承  RefCountedObject
//BufferHead类 具备引用计数能力
	class BufferHead:public RefCountedObject {
	public:
	    BufferHead() : nref(0) {}
	    void read_data();
	    void write_data();
	};
	
	//基类
	class RefCountedObject {
	public:
	    RefCountedObject() : nref(0) {}
	    mutable std::atomic<int> nref;
	};
	// 用户定义的函数 Boost 如何找到用户定义函数?
	friend inline void intrusive_ptr_add_ref(RefCountedObject* p) noexcept {
	    p->nref.fetch_add(1, std::memory_order_relaxed);
	    //用户可以自定义原子操作策略
	}
	// 用户定义的函数 Boost 如何找到用户定义函数?
	friend inline void intrusive_ptr_release(RefCountedObject* p) noexcept {
	    if (p->nref.fetch_sub(1, std::memory_order_acq_rel) == 1)
	        delete p;
	}
} //namespace ceph 
```


 **关键点**

- 函数必须定义在 `ceph` 命名空间内 【为什么】
    
- Boost 在编译时看到 `intrusive_ptr<ceph::BufferHead>`，会自动去 `ceph` 命名空间查找函数。【为什么】
    
- `intrusive_ptr_add_ref` 和 `intrusive_ptr_release` ​**​并不是 `intrusive_ptr` 类的成员函数​**​，而是独立的全局函数（或友元函数）。这是 Boost 设计上的关键点，目的是解耦引用计数逻辑和智能指针本身


## 2.3  核心疑问2 ：boost::intrusive_ptr 构造，怎么能调用直接用户自定义的 intrusive_ptr_add_ref 接口,这个2个类非继承，非组合关系？


>画外音：远离 c++语法，优雅，但是不好理解


Scott Meyers
- C++顶级权威之一，为世界各地客户提供培训和咨询服务。
- 发表有畅销的Effective C++系列图书（《Effective C++》、《More Effective C++》和《Effective STL》）
- 提出了：ADL（argument-dependent lookup）后来的“Koenig lookup”


ADL（Argument-Dependent Lookup，参数依赖查找）
- 是 C++ 中一种特殊的名称查找规则，
   它允许编译器在​**​函数参数所属的命名空间​**​中查找未限定的函数名。
- 这一机制是 C++ 标准库和 Boost 等框架实现灵活性的关键。


**intrusive_ptr 调用构造函数定义变量时候**

**会自动调用 `intrusive_ptr_add_ref(p)`** 


1. 编译器首先在 **`boost` 命名空间查找该函数**（因为 `intrusive_ptr` 定义在 `boost` 中）。
2. 如果没找到，**ADL 会被触发，编译器会检查实参 `p` 的类型 `T`** 所属的命名空间。
3. 如果你在 `T` 的命名空间（如 `ceph`）中定义了 `intrusive_ptr_add_ref`，编译器就能成功找到它。。


关键点：
-  在 `intrusive_ptr` 的设计中，友元函数是实现引用计数的核心机制
- 在 intrusive_ptr 的设计中，intrusive_ptr_add_ref 为什么必须声明友元函数？ 可以通过 friend 关键字让非成员函数访问类的私有成员
- `intrusive_ptr_add_ref` 和 `intrusive_ptr_release` ​**​并不是 `intrusive_ptr` 类的成员函数​**​，而是独立的全局函数（或友元函数）。
- 这是 Boost 设计上的关键点，目的是解耦引用计数逻辑和智能指针本身

## 2.4 intrusive_ptr源码分析


```c++
代码位置：
 https://github.com/steinwurf/boost/blob/master/boost/smart_ptr/intrusive_ptr.hpp

template<class T>
class intrusive_ptr {
private:
    T* ptr_; //只有托管的指针，没有引用计数
    
public:
    // 构造时增加引用计数
    intrusive_ptr(T* p) : ptr_(p) {
        //intrusive_ptr_add_ref` 不是 `intrusive_ptr` 的成员函数
        //模版在实例化 时候才会被替换
        if (ptr_) intrusive_ptr_add_ref(ptr_);
    }

    // 析构时减少引用计数
    ~intrusive_ptr() {
      //intrusive_ptr_add_ref` 不是 `intrusive_ptr` 的成员函数
        if (ptr_) intrusive_ptr_release(ptr_);
    }
};
```


`intrusive_ptr_add_ref` 不是 `intrusive_ptr` 的成员函数，主要原因是：

1. **访问控制**：智能指针无法访问对象私有引用计数。
    
2. **类型泛化**：不同对象可能有不同的引用计数实现，不能统一写在指针类里。
    
3. **设计哲学**：侵入式引用计数让对象自己维护生命周期，智能指针只是调用接口。
    


## 2. 5 为什么 intrusive_ptr 更快[多核架构]



 > 核心原因是缓存一致性

**真正的原因是缓存局部性**：

- `shared_ptr`：
    
    - 对象在一块内存 ---位置A
        
    - 控制块在另一块内存 ---位置B
        
    - 引用计数的修改操作会导致 **两次缓存行访问**
        
- `intrusive_ptr`：
    
    - 引用计数嵌入对象  ---位置A
        
    - **大概率命中同一缓存行**，减少总线同步
        

这里从多核角度解释:Redis为例子

为什么CPU结构也会影响Redis的性能？

CPU 的 NUMA 架构对 Redis 性能的影响

Redis 假如 2个线程, - 
- 一个线程读取请求 记录请求到内存 a
- 一个线程处理请求 读取a记录,处理业务
- 2个线程 在不同内核上

![](https://s2.loli.net/2025/09/01/4Un3FO9XCDRhNMS.png)

- 图中的网络中断处理程序被绑在了 CPU Socket 1 的某个核上，而 Redis 实例则被绑在了 CPU Socket 2 上。
    
- 此时，网络中断处理程序读取到的网络数据，被保存在 CPU Socket 1 的本地内存中，当 Redis 实例要访问网络数据时，就需要 Socket 2 通过总线把内存访问命令发送到 Socket 1 上，进行远程访问，时间开销比较大。
## 2.6  原子操作不是瓶颈

很多人会误以为 `intrusive_ptr` 更快是因为原子更新更高效，但这是误解。

- 两者的原子操作都是通过 CPU 指令（如 `lock xadd`）实现，**性能几乎一样**。
    
- Benchmark 结果显示，单线程情况下，两者差距不到 5ns。
    
- 真正导致性能差异的，是 **cache line 竞争** 和 **控制块访问**。
    

所以，**原子更新不是瓶颈**，**缓存局部性才是王道**。

---

# 三 shared_ptr 的二次分配问题

## 3.1 场景
- https://github.com/boostorg/smart_ptr/blob/develop/doc/smart_ptr/make_shared.adoc
- https://github.com/boostorg/smart_ptr/blob/develop/include/boost/smart_ptr/shared_ptr.hpp

| 场景                               | 分配次数 | 原因                                      |
| -------------------------------- | ---- | --------------------------------------- |
| `shared_ptr<T>(new T)`           | 2    | 对象 + 控制块                                |
| `make_shared<T>()`               | 1    | 控制块和对象合并                                |
| `make_shared<T>(custom_deleter)` | 2    | 控制块需要存删除器，无法合并对象内存                      |
| `make_shared<T>()` + `weak_ptr`  | 1~2  | 现代实现一般 1 次，但复杂 deleter 或 allocator 可能退化 |

`shared_ptr` 最坏情况下需要分配两次内存：

`shared_ptr` 内部主要维护两部分：

1. **控制块（control block）**
    - 保存：
        - 引用计数（use_count）
        - 弱引用计数（weak_count）
        - 删除器（deleter）
        - 分配器（allocator）
            
    - 控制对象的生命周期。

```c
+----------------+         +----------------------+
|  user object   |  <--->  | control block        |
|                |         | ref_count (atomic)   |
|                |         | weak_count (atomic)  |
+----------------+         | deleter, allocator   |
                           +----------------------+
```
        
2. **对象本体（T）**
    
    - 也就是你 `new Obj()` 创建的对象。
    

比如：

```cpp
auto p = std::shared_ptr<Obj>(new Obj());
```

- `new Obj()` 分配对象 → 1 次分配
    
- `shared_ptr` 分配控制块 → 1 次分配  
✅ 总共 **2 次分配**


 `make_shared` 优化

`std::make_shared` 做了一个关键优化：

- **控制块和对象合并分配**，只用一次内存
    
- 内存布局大致是：
    
```c
	+-------------------+
	| 控制块 + 对象本体 |
	+-------------------+
 
```
    
- 优点：
    
    - 减少分配次数 → 提高性能
        
    - **减少内存碎片**
        
    - 对象和控制块总是一起分配 → 释放更简单
    
但有两种情况，即使用 `make_shared`，仍然可能退化到两次分配：

- **自定义删除器**
    
- **启用 `weak_ptr`**，因为控制块需要额外空间存储弱引用计数。
    

>即使使用 `make_shared`，有些情况仍然不能合并：

## 3.2自定义删除器（deleter）
```c++

auto p = std::shared_ptr<Obj>(new Obj(), custom_deleter);
//make_shared
//custom_deleter 是任意类型的可调用对象（lambda、函数对象、函数指针……）

```


- 控制块需要存储 **删除器对象**，大小未知
    
- `make_shared` 内部无法在一次分配中保证删除器的存储
    
- 编译器会退化成两次分配：
    
    - 分配控制块 + deleter
        
    - 分配对象本体
        
 1️⃣ `D deleter_` 是什么

在 Boost `shared_ptr` 控制块里：

```cpp
template <typename P, typename D>
class sp_counted_impl_p : public sp_counted_base {
public:
    P ptr_;       // 对象指针
    D deleter_;   // 删除器对象

    void dispose() override {
        deleter_(ptr_);  // 调用删除器销毁对象
    }
};
```

- `D` 是模板参数，由你在构造 `shared_ptr` 时传入的删除器类型决定：
    
    ```cpp
    boost::shared_ptr<Obj> p(new Obj(), [](Obj* o){ delete o; });
    ```
    
    - 这里 `D = lambda type`
        
- `deleter_` 就是 **类型为 D 的对象成员**，存储在控制块里。
    

---

### 2️⃣ `D deleter_` 的大小

- **取决于类型 `D`**
    
    - **普通函数指针**：大小固定，通常是 8 字节（64 位系统）
        
    - **函数对象 / lambda**：大小 = lambda 捕获变量大小 + 编译器生成的函数对象开销
        
        - 如果 lambda 不捕获变量 → 类型为无状态 lambda → 编译器可以优化成空类，大小通常 1 字节（空类的最小大小）
            
        - 如果 lambda 捕获变量 → 大小 = 捕获的成员大小
            
    - **std::function**：大小固定，但可能内部包含指针或 type-erasure 机制 → 可能有额外堆分配
        

示例：

```cpp
auto p1 = std::shared_ptr<int>(new int(42), [](int* p){ delete p; });
// lambda 无捕获，sizeof(D) = 1 字节

int x = 100;
auto p2 = std::shared_ptr<int>(new int(42), [x](int* p){ delete p; });
// lambda 捕获 x，sizeof(D) >= sizeof(int)
```



### 1️⃣ Lambda 本质

```cpp
int x = 42;
auto f = [x](int* p){ delete p; };
```

- 这个 lambda 会捕获变量 `x`
    
- 编译器会为它生成一个**匿名的函数对象类**（closure type）：
    

```cpp
class __lambda {
public:
    int x;   // 捕获变量存储在类成员中

    void operator()(int* p) const { delete p; }
};
```

- `f` 就是 `__lambda` 类型的对象
    
- 这个对象有 **内存大小**，由捕获变量决定
    


### 3️⃣ 为什么这会影响 `make_shared`

- `make_shared` 想把 **控制块 + 对象** 放在一块连续内存里
    
- **控制块大小 = sizeof(use_count + weak_count + deleter_)**
    
- 如果 deleter_ 大小不固定或未知（lambda 捕获、函数对象模板）  
    → 编译器无法保证一次分配就能容纳对象和控制块  
    → 必须分开两次分配
    


# 四 intrusive_ptr 的典型应用场景

。

---

## 1️⃣ 对象自管理生命周期

- **Ceph 示例**：`ObjectCacher` 中缓存的对象、`OSDMap` 等
    
- **实现方式**：
    
    - 对象类继承自 `RefCounted`
        
    - 内部维护引用计数，`intrusive_ptr` 只负责调用 `add_ref`/`release`
        
- **优势**：
    
    - 对象和引用计数紧密存储
        
    - 不需要额外控制块 → 内存开销小
        

```cpp
class Object : public RefCounted {
    // 数据内容
};
boost::intrusive_ptr<Object> obj(new Object());
```

---

## 2️⃣ 高性能 / 低开销

- **Ceph 示例**：
    
    - OSD 内部 `bufferlist`、消息对象
        
    - `ObjectCacher` 中频繁读写对象
        
- **原因**：
    
    - `shared_ptr` 每次会额外分配控制块
        
    - Ceph 对延迟和吞吐量敏感
        
- **优势**：
    
    - 避免额外堆分配 → 高性能
        

---

## 3️⃣ 循环引用场景

- **Ceph 示例**：
    
    - `ObjectCacher` 中对象之间可能互相引用，例如对象持有快照指针
        
- **实现**：
    
    - 对象自己维护引用计数
        
    - `intrusive_ptr` 调用对象的 `release` 时，若计数为 0，则对象析构
        
- **优势**：
    
    - 循环引用可以通过对象自定义析构策略解决
        
    - 避免 `shared_ptr` + `weak_ptr` 的复杂管理
        

```cpp
struct CephNode : public RefCounted {
    boost::intrusive_ptr<CephNode> parent;
    std::vector<boost::intrusive_ptr<CephNode>> children;
};
```

---

## 4️⃣ 与内存池 / 资源管理结合

- **Ceph 示例**：
    
    - `ObjectCacher` 复用对象池
        
    - `OSDMap` 和 `PG` 对象在内存池中创建
        
- **实现**：
    
    - 对象 + 引用计数在同一块内存中
        
    - `intrusive_ptr` 只调用对象自身的 `release` → 回收对象到池中
        
- **优势**：
    
    - 高效回收 / 重用对象
        
    - 内存布局紧凑，减少碎片
        

---

### ✅ 总结：Ceph 中 `intrusive_ptr` 价值

|场景|Ceph 示例|优势|
|---|---|---|
|对象自管理生命周期|`ObjectCacher` 缓存对象|控制块开销低，缓存友好|
|高性能 / 低开销|`bufferlist`、OSD 消息对象|避免额外堆分配，性能高|
|循环引用|对象互相引用（快照/PG）|自定义释放策略，避免 weak_ptr|
|内存池 / 资源管理|对象池复用|内存紧凑，可复用对象，减少碎片|

---

如果你需要，我可以画一张 **Ceph 中 intrusive_ptr 应用示意图**，把 **对象、引用计数、内存池、循环引用**的关系直观展示出来，能更容易理解。

你希望我画吗？

## 总结

- **性能核心点**：  
    `intrusive_ptr` 比 `shared_ptr` 快，主要因为 **缓存一致性**，而不是原子更新。
    
- `intrusive_ptr`**不会自动解决循环引用**，和 `shared_ptr` 一样会泄漏。想解决只能靠业务逻辑



## 参考
- http://m.blog.chinaunix.net/uid-20499155-id-1663246.html