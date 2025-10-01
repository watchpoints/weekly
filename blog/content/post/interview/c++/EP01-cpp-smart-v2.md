---
title: 90%的程序员都答错了！为什么boost::intrusive_ptr比std::shared_ptr快
date: 2025-07-26
description: 深入解析C++智能指针性能差异，揭秘boost::intrusive_ptr比std::shared_ptr快的真正原因
draft: false
tags:
  - C++
  - 智能指针
  - 性能优化
  - 面试题
categories:
  - C++进阶
---

# 🚀 大厂面试题：为什么boost::intrusive_ptr比std::shared_ptr快？
- 90%的程序员都答错了！为什么boost::intrusive_ptr比std::shared_ptr快


![boost:intrusive_ptr vs std::shared_ptr](https://s2.loli.net/2025/09/01/nh3IQ6DxJs5SNAG.png)

## 📋 本文核心问题

### ❓ 问题1：为什么boost::intrusive_ptr比std::shared_ptr快？
- **核心答案**：缓存一致性，而非原子操作本身

### ❓ 问题2：boost::intrusive_ptr 构造，怎么能调用直接用户自定义的 intrusive_ptr_add_ref 接口,这个2个类非继承，非组合关系？【万恶的c++语法】
- **关键机制**：ADL（参数依赖查找）+ 友元函数
- **设计哲学**：解耦引用计数逻辑与智能指针
-  intrusive_ptr_add_ref 接口函数 ​​并不是 intrusive_ptr 类的成员函数​​，而是独立的全局函数（或友元函数）
- 在 用户自定义类 的设计中，intrusive_ptr_add_ref 为什么必须声明友元函数 可以通过 friend 关键字让非成员函数访问类的私有成员

### ❓ 问题3：std::make_shared为什么在自定义删除器时会退化？
- **根本原因**：删除器大小未知，无法合并内存分配

| 删除器类型             | `sizeof(D)` | 说明         |
| ----------------- | ----------- | ---------- |
| 函数指针              | 8（64 位系统）   | 大小固定       |
| 无捕获 lambda        | 1（空类最小大小）   | 编译器优化空类    |
| 有捕获 lambda / 函数对象 | 捕获成员大小      | 编译器生成的函数对象 |
| `std::function`   | 固定（24~32字节） | 内部可能堆分配    |
---

## 🎯 第一部分：为什么boost::intrusive_ptr比std::shared_ptr快

### 1.1 数据结构对比

**设计哲学**：
- **智能指针只负责所有权管理**
- **引用计数逻辑由用户定义**
- 这种设计使其成为唯一能直接管理**侵入式对象**的C++智能指针

| 特性 | `std::shared_ptr` | `boost::intrusive_ptr` |
|------|------------------|----------------------|
| **引用计数存储** | 单独的控制块 | 引用计数嵌入对象本身 |
| **内存分配** | 至少两次分配 | 一次分配（对象+引用计数） |
| **原子操作** | 控制块中维护 | 对象内部维护 |
| **缓存一致性** | 访问两块内存→可能cache miss | 大概率命中同一cache line |
| **侵入性** | 不需要修改对象定义 | 需要在对象中定义接口 |
| **循环引用** | 同样存在问题 | 同样存在问题 |
| **典型场景** | 应用层、通用库 | 系统内核、性能敏感路径 |

> 🎯 **核心结论**：boost::intrusive_ptr比shared_ptr快，核心原因是**缓存一致性**，而不是原子更新本身。

### 1.2 实际使用示例

```cpp
// 使用intrusive_ptr智能指针管理ObjectCacher::BufferHead类
boost::intrusive_ptr<ceph::BufferHead> bh(new ceph::BufferHead);

namespace ceph {
    // 用户自定义BufferHead类继承RefCountedObject
    // BufferHead类具备引用计数能力
    class BufferHead : public RefCountedObject {
    public:
        BufferHead() : nref(0) {}
        void read_data();
        void write_data();
    };
    
    // 基类
    class RefCountedObject {
    public:
        RefCountedObject() : nref(0) {}
        mutable std::atomic<int> nref;
    };
    
    // 用户定义的函数 - Boost如何找到这些函数？
    friend inline void intrusive_ptr_add_ref(RefCountedObject* p) noexcept {
        p->nref.fetch_add(1, std::memory_order_relaxed);
        // 用户可以自定义原子操作策略
    }
    
    friend inline void intrusive_ptr_release(RefCountedObject* p) noexcept {
        if (p->nref.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete p;
    }
} // namespace ceph
```

**关键点**：
- 函数必须定义在`ceph`命名空间内
- Boost在编译时看到`intrusive_ptr<ceph::BufferHead>`，会自动去`ceph`命名空间查找函数
- `intrusive_ptr_add_ref`和`intrusive_ptr_release`**并不是**`intrusive_ptr`类的成员函数，而是独立的全局函数（或友元函数）

---

## 🔍 第二部分：boost::intrusive_ptr如何调用用户自定义的接口？

### 2.1 什么是ADL？

> 💭 **画外音**：C++语法优雅但不好理解，这就是它的魅力所在！

**Scott Meyers**（C++顶级权威）：
- 为世界各地客户提供培训和咨询服务
- 发表畅销的Effective C++系列图书
- 提出了ADL（Argument-Dependent Lookup），后来称为"Koenig lookup"

**ADL（参数依赖查找）**：
- 是C++中一种特殊的名称查找规则
- 允许编译器在**函数参数所属的命名空间**中查找未限定的函数名
- 这一机制是C++标准库和Boost等框架实现灵活性的关键

### 2.2 intrusive_ptr构造函数调用过程

**当定义intrusive_ptr变量时，会自动调用`intrusive_ptr_add_ref(p)`**

1. 编译器首先在**`boost`命名空间**查找该函数（因为`intrusive_ptr`定义在`boost`中）
2. 如果没找到，**ADL会被触发**，编译器会检查实参`p`的类型`T`所属的命名空间
3. 如果你在`T`的命名空间（如`ceph`）中定义了`intrusive_ptr_add_ref`，编译器就能成功找到它

**关键点**：
- 在`intrusive_ptr`的设计中，友元函数是实现引用计数的核心机制
- 可以通过`friend`关键字让非成员函数访问类的私有成员
- 这是Boost设计上的关键点，目的是解耦引用计数逻辑和智能指针本身

### 2.3 源码分析

```cpp
// 代码位置：https://github.com/steinwurf/boost/blob/master/boost/smart_ptr/intrusive_ptr.hpp

template<class T>
class intrusive_ptr {
private:
    T* ptr_; // 只有托管的指针，没有引用计数
    
public:
    // 构造时增加引用计数
    intrusive_ptr(T* p) : ptr_(p) {
        // intrusive_ptr_add_ref不是intrusive_ptr的成员函数
        // 模板在实例化时才会被替换
        if (ptr_) intrusive_ptr_add_ref(ptr_);
    }

    // 析构时减少引用计数
    ~intrusive_ptr() {
        if (ptr_) intrusive_ptr_release(ptr_);
    }
};
```

**为什么`intrusive_ptr_add_ref`不是成员函数？**

1. **访问控制**：智能指针无法访问对象私有引用计数
2. **类型泛化**：不同对象可能有不同的引用计数实现，不能统一写在指针类里
3. **设计哲学**：侵入式引用计数让对象自己维护生命周期，智能指针只是调用接口

---

## ⚡ 第三部分：性能真相 - 缓存一致性

### 3.1 为什么intrusive_ptr更快？

> 🎯 **核心原因：缓存一致性，不是原子操作！**

**真正的原因是缓存局部性**：

- **`shared_ptr`**：
  - 对象在一块内存（位置A）
  - 控制块在另一块内存（位置B）
  - 引用计数的修改操作会导致**两次缓存行访问**

- **`intrusive_ptr`**：
  - 引用计数嵌入对象（位置A）
  - **大概率命中同一缓存行**，减少总线同步

### 3.2 多核架构的影响

**以Redis为例说明CPU结构对性能的影响**

**CPU的NUMA架构对Redis性能的影响**：

假设Redis有2个线程：
- 一个线程读取请求，记录请求到内存A
- 一个线程处理请求，读取A记录，处理业务
- 2个线程在不同内核上

![NUMA架构影响](https://s2.loli.net/2025/09/01/4Un3FO9XCDRhNMS.png)

- 网络中断处理程序被绑定在CPU Socket 1的某个核上
- Redis实例被绑定在CPU Socket 2上
- 网络中断处理程序读取到的网络数据保存在CPU Socket 1的本地内存中
- 当Redis实例要访问网络数据时，需要Socket 2通过总线把内存访问命令发送到Socket 1上，进行远程访问，时间开销比较大

### 3.3 原子操作不是瓶颈

很多人会误以为`intrusive_ptr`更快是因为原子更新更高效，但这是误解：

- 两者的原子操作都是通过CPU指令（如`lock xadd`）实现，**性能几乎一样**
- Benchmark结果显示，单线程情况下，两者差距不到5ns
- 真正导致性能差异的，是**cache line竞争**和**控制块访问**

> 💡 **结论**：原子更新不是瓶颈，**缓存局部性才是王道**！

---

## 🔧 第四部分：shared_ptr的二次分配问题

### 4.1 内存分配场景对比

| 场景 | 分配次数 | 原因 |
|------|----------|------|
| `shared_ptr<T>(new T)` | 2 | 对象 + 控制块 |
| `make_shared<T>()` | 1 | 控制块和对象合并 |
| `make_shared<T>(custom_deleter)` | 2 | 控制块需要存删除器，无法合并对象内存 |
| `make_shared<T>() + weak_ptr` | 1~2 | 现代实现一般1次，但复杂deleter或allocator可能退化 |

### 4.2 shared_ptr内部结构

`shared_ptr`内部主要维护两部分：

1. **控制块（control block）**
   - 保存：引用计数（use_count）、弱引用计数（weak_count）、删除器（deleter）、分配器（allocator）
   - 控制对象的生命周期

```c
+----------------+         +----------------------+
|  user object   |  <--->  | control block        |
|                |         | ref_count (atomic)   |
|                |         | weak_count (atomic)  |
+----------------+         | deleter, allocator   |
                           +----------------------+
```

2. **对象本体（T）**
   - 也就是你`new Obj()`创建的对象

**示例**：
```cpp
auto p = std::shared_ptr<Obj>(new Obj());
```
- `new Obj()`分配对象 → 1次分配
- `shared_ptr`分配控制块 → 1次分配
- ✅ 总共**2次分配**

### 4.3 make_shared的优化

`std::make_shared`做了一个关键优化：

- **控制块和对象合并分配**，只用一次内存
- 内存布局大致是：

```c
+-------------------+
| 控制块 + 对象本体 |
+-------------------+
```

**优点**：
- 减少分配次数 → 提高性能
- **减少内存碎片**
- 对象和控制块总是一起分配 → 释放更简单

**但有两种情况，即使用`make_shared`，仍然可能退化到两次分配**：
- **自定义删除器**
- **启用`weak_ptr`**，因为控制块需要额外空间存储弱引用计数

### 4.4 自定义删除器的影响

```cpp
auto p = std::shared_ptr<Obj>(new Obj(), custom_deleter);
// custom_deleter是任意类型的可调用对象（lambda、函数对象、函数指针等）
```

**问题分析**：

1. **控制块需要存储删除器对象**，大小未知
2. `make_shared`内部无法在一次分配中保证删除器的存储
3. 编译器会退化成两次分配：
   - 分配控制块 + deleter
   - 分配对象本体

#### 删除器类型大小对比

| 删除器类型 | `sizeof(D)` | 说明 |
|------------|-------------|------|
| 函数指针 | 8（64位系统） | 大小固定 |
| 无捕获lambda | 1（空类最小大小） | 编译器优化空类 |
| 有捕获lambda/函数对象 | 捕获成员大小 | 编译器生成的函数对象 |
| `std::function` | 固定（24~32字节） | 内部可能堆分配 |

#### Lambda的本质

```cpp
int x = 42;
auto f = [x](int* p){ delete p; };
```

这个lambda会捕获变量`x`，编译器会为它生成一个**匿名的函数对象类**：

```cpp
class __lambda {
public:
    int x;   // 捕获变量存储在类成员中
    void operator()(int* p) const { delete p; }
};
```

`f`就是`__lambda`类型的对象，这个对象有**内存大小**，由捕获变量决定。

**为什么这会影响`make_shared`？**

- `make_shared`想把**控制块 + 对象**放在一块连续内存里
- **控制块大小 = sizeof(use_count + weak_count + deleter_)**
- 如果deleter_大小不固定或未知（lambda捕获、函数对象模板）
  → 编译器无法保证一次分配就能容纳对象和控制块
  → 必须分开两次分配

---

## 🎯 第五部分：intrusive_ptr的典型应用场景

### 5.1 对象自管理生命周期

**Ceph示例**：`ObjectCacher`中缓存的对象、`OSDMap`等

**实现方式**：
- 对象类继承自`RefCounted`
- 内部维护引用计数，`intrusive_ptr`只负责调用`add_ref`/`release`

**优势**：
- 对象和引用计数紧密存储
- 不需要额外控制块 → 内存开销小

```cpp
class Object : public RefCounted {
    // 数据内容
};
boost::intrusive_ptr<Object> obj(new Object());
```

### 5.2 高性能/低开销

**Ceph示例**：
- OSD内部`bufferlist`、消息对象
- `ObjectCacher`中频繁读写对象

**原因**：
- `shared_ptr`每次会额外分配控制块
- Ceph对延迟和吞吐量敏感

**优势**：
- 避免额外堆分配 → 高性能

### 5.3 循环引用场景

**Ceph示例**：
- `ObjectCacher`中对象之间可能互相引用，例如对象持有快照指针

**实现**：
- 对象自己维护引用计数
- `intrusive_ptr`调用对象的`release`时，若计数为0，则对象析构

**优势**：
- 循环引用可以通过对象自定义析构策略解决
- 避免`shared_ptr` + `weak_ptr`的复杂管理

```cpp
struct CephNode : public RefCounted {
    boost::intrusive_ptr<CephNode> parent;
    std::vector<boost::intrusive_ptr<CephNode>> children;
};
```

### 5.4 与内存池/资源管理结合

**Ceph示例**：
- `ObjectCacher`复用对象池
- `OSDMap`和`PG`对象在内存池中创建

**实现**：
- 对象 + 引用计数在同一块内存中
- `intrusive_ptr`只调用对象自身的`release` → 回收对象到池中

**优势**：
- 高效回收/重用对象
- 内存布局紧凑，减少碎片

### 5.5 总结：Ceph中intrusive_ptr的价值

| 场景 | Ceph示例 | 优势 |
|------|----------|------|
| 对象自管理生命周期 | `ObjectCacher`缓存对象 | 控制块开销低，缓存友好 |
| 高性能/低开销 | `bufferlist`、OSD消息对象 | 避免额外堆分配，性能高 |
| 循环引用 | 对象互相引用（快照/PG） | 自定义释放策略，避免weak_ptr |
| 内存池/资源管理 | 对象池复用 | 内存紧凑，可复用对象，减少碎片 |

---

## 📝 总结

### 🎯 核心要点

1. **性能核心点**：
   - `intrusive_ptr`比`shared_ptr`快，主要因为**缓存一致性**，而不是原子更新

2. **循环引用问题**：
   - `intrusive_ptr`**不会自动解决循环引用**，和`shared_ptr`一样会泄漏
   - 想解决只能靠业务逻辑

### 💡 面试技巧

- **不要只说"缓存一致性"**，要能解释为什么
- **要理解ADL机制**，这是C++的高级特性 
- **要明白内存分配的影响**，这是性能优化的关键
- **要结合实际项目经验**，比如Ceph、Redis等

---

> 🚀 **记住：在C++的世界里，性能优化往往藏在细节中，理解底层机制才能写出高效的代码！**

