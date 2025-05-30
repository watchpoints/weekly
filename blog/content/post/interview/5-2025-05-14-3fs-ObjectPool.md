---
title: 从青铜到王者系列：一行代码解千愁：看DeepSeek-Ai/3FS对象池如何消除80%锁竞争
date: 2024-08-07
description: 高性能C++对象池的设计与实现
draft: false
tags:
  - 阅读之美
categories:
  - do book
---
<font color="#245bdb">分享干货，主打一个真实</font>


各位老师好，我是小义。

本文主要描述 DeepSeek-ai/3FS对象池（ObjectPool）双层缓存架构的实现

![图1-了解需求](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514230017.png)

阅读本文我将获得以下收益，希望对你也有帮助：

✅ 利用C++标准库提供的vector、unique_ptr等工具，可以快速开发一个高效优雅的组件，一行代码搞定最难部分，足够简单。
![图2：抽象就是简单](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250515160251.png)


✅ 利用C++提供的thread_local标记，能否模拟CPU三级缓存设计（L1、L2单核独有，L3多核共享）？

能实现：
1. C++保证不同线程创建静态局部对象时，全局只有一个实例（俗称单例模式）。一行代码解决问题：`static T l3`，不需要锁。
2. C++保证不同线程创建局部线程私有对象时，有多少个线程就有多少个实例：`thread_local tls l2(l3)`，同样一行代码搞定，不需要锁。
   ![图3：分级存储](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514201055.png)
✅ 单元测试的重要性不容忽视。


让我们开始吧。

风和日丽的中午，老王来到小义办公桌前说：

"最近上线的Ceph集群SWAP使用率偏高，导致读写业务延迟增加。原来128G内存（见下图）已经增加到256G作为临时解决方案。根据公司会议安排，请你预研一下内存池分配方案。"

```shell
free -h
              total        used        free      shared  buff/cache   available
Mem:           128G        115G        3.2G        512M         9.8G         12G
Swap:           16G        8.0G        8.0G
```

小义迅速记录了要点。

![图1-了解需求](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514230017.png)

如何实现这个需求？根据已有资料，小义整理了几种思路：

|      | 方向                | 预计耗时    |
| ---- | ----------------- | --------- |
| 思路1 | 学习gcc-mirror/gcc  | 1年，结果未知  |
| 思路2 | 学习google/tcmalloc | 半年，结果未知 |
| 思路3 | 学习STL源码剖析       | 3个月，结果未知 |
| 思路4 | 学习经典数据库内存设计  | 1个月，结果未知 |

老王看到后直接否决："这太复杂了，投入这么多精力但结果未知，我怎么评估？回去继续调查，我不会批准的。越简单越好！"

小义开始自我鼓励：
1. <font color="#245bdb">我做不到的事情，别人一定想到了解决办法</font>
2. [深入理解ThreadLocal变量](https://mp.weixin.qq.com/s/HyepTWcogQRpdLXkLfrKHA)文章提到C++11新特性：thread_local，这与多线程相关
3. deepseek-ai/3FS（一个为AI训练和推理工作负载设计的高性能分布式文件系统）肯定遇到过类似情况，看看他们怎么解决的
4. 代码位置：https://github.com/deepseek-ai/3FS/blob/main/src/common/utils/ObjectPool.h

**划重点：在阅读代码之前，应该问自己三个问题**
1. 采用了什么数据结构？整体架构是什么？
2. 使用了什么算法？get、set流程是怎样的？
3. 有哪些常用但自己不熟悉的技术细节？

##  自问：采用什么数据结构

### 1.1 分层缓存架构（类似CPU缓存）

在高并发系统中，采用线程局部缓存的对象池相比直接使用全局池通常能提供：
- 10-100倍的吞吐量提升
- 显著降低的延迟（特别是尾部延迟）
- 更好的扩展性（随线程数增加而线性提升）

![分级存储](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514201055.png)

类图：
```js
+--------------------------------------+
|           ObjectPool<T>              |
+--------------------------------------+
| 线程本地存储 (TLS)      | 全局存储     |
| +----------------------+ +----------+ |
| | first_batch          | | global_  | |
| | (主缓存队列)          | | (批次队列)| |
| +----------------------+ +----------+ |
| | second_batch         | | mutex_   | |
| | (溢出队列)            | |          | |
| +----------------------+ +----------+ |
+--------------------------------------+
        ↑                     ↑
        |                     |
+---------------+   +------------------+
| Ptr (智能指针) |   | Storage (存储块) |
| with Deleter  |   +------------------+
+---------------+
```

可视化：
```js
进程内存空间
├── 共享区域
│   └── static ObjectPool instance  (单个实例，共享)
│       ├── mutex_
│       └── global_
│
├── 线程1私有区域
│   └── thread_local TLS tls  (线程1专属)
│       ├── first_  (线程1专属批次)
│       └── second_ (线程1专属批次)
│
├── 线程2私有区域
│   └── thread_local TLS tls  (线程2专属)
│       ├── first_  (线程2专属批次)
│       └── second_ (线程2专属批次)
└── ...
```

### 为什么无需同步？

原因在于：
1. 每个线程操作的first_和second_批次是线程私有的
2. 线程私有数据不会被其他线程访问
3. 没有共享访问，就不需要同步机制

只有在两种情况下才需要同步：

1. 线程的本地缓存耗尽，需要从全局池获取新批次
2. 线程的本地缓存溢出，需要归还批次到全局池

##  自问：基本操作是什么

### 2.1 创建get和释放put操作流程图

![申请读取操作](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250515123233.png)

文本绘制流程图：
```js
	┌─────────────────┐
	│ 调用 ObjectPool::get() │
	└──────────┬──────┘
	           ↓
	┌─────────────────┐
	│ 获取线程本地存储 TLS │
	└──────────┬──────┘
	           ↓
	┌─────────────────┐
	│检查 second_ 批次 │
	└──────────┬──────┘
	           ↓
	┌───────────────────────┐
	│    second_ 有对象?    │
	└───────────┬───────────┘
	      Yes   │     No
	      ┌─────┘     └────┐
	      ↓                ↓
	┌──────────────┐  ┌──────────────┐
	│从second_取对象│  │检查first_批次│
	└───────┬──────┘  └───────┬──────┘
	        │                 │
	        ↓                 ↓
	┌───────────────┐  ┌───────────────────┐
	│ 返回构造的对象 │  │   first_ 有对象?  │
	└───────────────┘  └────────┬──────────┘
	                      Yes   │    No
	                      ┌─────┘    └────┐
	                      ↓               ↓
	               ┌──────────────┐ ┌───────────────┐
	               │从first_取对象│ │尝试从全局获取批次│
	               └───────┬──────┘ └───────┬───────┘
	                       │                │
	                       ↓                ↓
	               ┌───────────────┐ ┌───────────────┐
	               │ 返回构造的对象 │ │  获取成功?    │
	               └───────────────┘ └───────┬───────┘
	                                    Yes  │   No
	                                    ┌────┘   └────┐
	                                    ↓           ↓
	                           ┌────────────────┐ ┌────────────────┐
	                           │从批次获取并返回│ │创建kThreadLocalMaxNum个对象│
	                           └────────────────┘ └─────────┬──────┘
	                                                        ↓
	                                              ┌────────────────┐
	                                              │从批次获取并返回│
	                                              └────────────────┘
```

![普通操作](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250515123545.png)

文本绘制流程图：

```js
┌─────────────────┐
│  对象销毁触发   │
│ Deleter::operator() │
└──────────┬──────┘
           ↓
┌─────────────────┐
│   调用对象析构   │
└──────────┬──────┘
           ↓
┌─────────────────┐
│ 获取线程本地存储 TLS │
└──────────┬──────┘
           ↓
┌─────────────────┐
│  first_ 满了?   │
└───────────┬─────┘
      No    │    Yes
      ┌─────┘    └────┐
      ↓               ↓
┌──────────────┐ ┌──────────────┐
│放入first_批次│ │放入second_批次│
└──────────────┘ └───────┬──────┘
                         ↓
                 ┌───────────────────┐
                 │second_ 达到阈值?  │
                 └────────┬──────────┘
                    No    │    Yes
                    ┌─────┘    └────┐
                    ↓               ↓
                    │      ┌──────────────────┐
                    │      │整批移入全局缓存  │
                    │      └─────────┬────────┘
                    │                ↓
                    │      ┌──────────────────┐
                    │      │清空并预留second_ │
                    │      └──────────────────┘
                    ↓
            ┌───────────────┐
            │     结束      │
            └───────────────┘
```

加锁全局操作：

![全局操作](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250515123928.png)

### 2.2 get和put接口实现

```cpp
using Ptr = std::unique_ptr<T, Deleter>; // 唯一指针

static Ptr get() { 
    // 无需加锁
    return Ptr(new (tls().get().release()) T); 
}

// 伪代码表示
void* memory = get();
ptr = operator new(memory) T
std::unique_ptr(ptr)
```

1. 获取线程本地存储 TLS：

```cpp
static auto &tls() {
    static ObjectPool instance; 
    // 疑问1：线程安全吗？多个线程同时调用tls()在第一次初始化时？
    // 是 
    thread_local TLS tls{instance};
    // 疑问2：TLS 不同线程创建副本，如果里面成员变量是全局的，不是创建多个吗？
    // TLS.parent 是指针类型 ，多浪费空间大小8字节 没关系？线程数量有限的
    return tls;
}

class TLS {
public:
    TLS(ObjectPool &parent): parent_(parent) {}
private:
    ObjectPool &parent_; // 指针类型，占用空间大小8字节，占用空间大小8字节 没关系 类虚函数之类的
    Batch first_;  // 向量，线程独有
    Batch second_; // 向量，线程独有
};
```

划重点：
1. `static ObjectPool instance`：不要为初始化编写自己的**双重检查锁定**
2. `thread_local TLS`：不同线程使用不同资源

线程A调用 get()：
- 调用 tls() → 获取线程A专属的TLS实例
- 调用 tls().get() → 从线程A的本地缓存获取对象
- 使用placement new构造对象
- 返回智能指针Ptr

线程B同时调用 get()：
- 调用 tls() → 获取线程B专属的TLS实例（不同于线程A的实例）
- 调用 tls().get() → 从线程B的本地缓存获取对象
- 使用placement new构造对象
- 返回智能指针Ptr

疑问：
1. 如何确保一个get()函数能为不同线程创建不同的私有数据？
2. 答案在于tls()函数使用了thread_local关键字保证了这一点，用户无需考虑这个细节。

```cpp
// 线程A
Ptr get() {
  // 1. 获取线程A的TLS
  TLS& threadLocalStorage = tls();  // 返回线程A专属的TLS
  
  // 2. 从线程A的本地缓存获取对象
  std::unique_ptr<Storage> storage = threadLocalStorage.get();
  // 内部流程：
  //   - 先检查second_批次（线程A专属）
  //   - 再检查first_批次（线程A专属）
  //   - 如缓存为空，才从全局instance获取新批次
  
  // 3. 构造并返回对象
  return Ptr(new (storage.release()) T(...));
}

// 线程B
Ptr get() {
  // 1. 获取线程B的TLS
  TLS& threadLocalStorage = tls();  // 返回线程B专属的TLS
  
  // 2. 从线程B的本地缓存获取对象
  std::unique_ptr<Storage> storage = threadLocalStorage.get();
  // 内部流程：
  //   - 先检查线程B的second_批次
  //   - 再检查线程B的first_批次
  //   - 如线程B的缓存为空，才访问全局instance
  
  // 3. 构造并返回对象
  return Ptr(new (storage.release()) T(...));
}
```

get操作的具体实现：

```cpp
// 从线程本地缓存或全局缓存获取对象
std::unique_ptr<Storage> get() {
    // 从second批次中弹出
    if (!second_.empty()) {
        auto item = std::move(second_.back());
        // 疑问：为什么要std::move？因为vector存放的是unique_ptr
        second_.pop_back(); 
        return item; // 类似CPU的L1缓存
    } // using Batch = std::vector<std::unique_ptr<Storage>>;
    
    // 当本地和全局缓存都为空时分配一个批次
    // L3需要加锁
    if (first_.empty() && !parent_.getBatch(first_)) {
        // 处理空缓存情况
    }

    // 从first批次中弹出
    // 类似CPU的L2缓存
    auto item = std::move(first_.back());
    first_.pop_back();
    return item;
}

// 从全局获取
bool getBatch(Batch &batch) {
    auto lock = std::unique_lock(mutex_);
    // 从全局缓存中弹出一个批次
    batch = std::move(global_.back());
    global_.pop_back();
    return true;
}
```

#### 释放put接口（执行析构，但不释放内存）

```
创建                                    销毁
  │                                      │
  ▼                                      ▼
┌────────┐    ┌─────────┐    ┌────────┐    ┌────────┐    ┌─────────┐
│ get()  │───►│ 使用对象 │───►│ ~Ptr() │───►│Deleter │───►│ put()   │
└────────┘    └─────────┘    └────────┘    └────────┘    └─────────┘
     │                                                        │
     ▼                                                        ▼
┌────────┐                                              ┌─────────┐
│tls.get()│                                             │tls.put()│
└────────┘                                              └─────────┘
```

当智能指针释放时调用：

```cpp
struct Deleter {
    constexpr Deleter() = default;
    
    void operator()(T *item) {
        item->~T();  // 调用析构函数
        // 将内存重新解释为Storage并归还池中
        tls().put(std::unique_ptr<Storage>(reinterpret_cast<Storage *>(item)));
    }
};

// 疑问1：为什么要用reinterpret_cast？
// 答：using Storage = std::aligned_storage_t<sizeof(T), alignof(T)> 解决了对齐问题

// 疑问2：item->~T() 可以单独抽出来，不执行free会内存泄漏吗？
// 答：不会，因为内存被重用而不是释放
```

划重点：
1. 不同线程释放各自的存储数据：tls().put()
2. reinterpret_cast本身不支持对齐，std::aligned_storage_t解决了这个问题
3. 这种设计实现了完美的内存复用，避免了重复的堆分配/释放操作，同时确保了内存对齐和类型安全。虽然使用了底层技术，但通过良好的封装使风险可控，是性能与安全的平衡之选。

#### 2.3 单元测试

单元测试确保了代码的正确性和稳定性。

```c++
TEST(TestObjectPool, AllocateAndRelease) {

	constexpr auto N = 1000000;
	
	static size_t constructTimes = 0;
	
	static size_t destructTimes = 0;
	
	struct A {
	
		A() { ++constructTimes; }
		
		//call?
		
		~A() { ++destructTimes; }
		
		//call?
	};
	
	//百万级别的分配/释放操作验证了系统在高压下的稳定性
	
	for (auto i = 0; i < N; ++i) {
	
		ObjectPool<A>::get();
		
		//get 调用构造函数，返回一个智能指针,
		
		//智能指针在析构时会调用析构函数
		
		//单线程环境 thread_local TLS tls{instance}
		
		//tls 在每个线程中是唯一的，因此可以保证在单线程环境下的线程安全
	
	}
	
	ASSERT_EQ(constructTimes, N);
	
	ASSERT_EQ(destructTimes, N);
	//如果存在内存泄漏，析构次数将小于构造次数

}

```

## 别人知道而我不知道的技术细节

### 3.1 C++智能指针的自定义销毁器

**为什么要自定义：std::unique_ptr默认的销毁策略是std::default_delete**

```cpp
// https://en.cppreference.com/w/cpp/memory/unique_ptr
// https://en.cppreference.com/w/cpp/memory/default_delete
template<
    class T,
    class Deleter = std::default_delete<T> // 默认参数
> class unique_ptr;
```

默认的delete操作：调用析构函数并释放内存
```cpp
// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h

/** 
 * default_delete的主模板，
 * 被unique_ptr用于单个对象
 *
 * @headerfile memory
 * @since C++11
 */
template<typename _Tp>
struct default_delete {
    void operator()(_Tp* __ptr) const
    {
        delete __ptr;
        // 为避免资源泄漏，
        // 每次动态分配必须有相应的释放操作
    }
};
```

划重点：delete操作执行了什么？

```cpp
string *ps = new string("Memory Management");
// 等价于：
void *memory = operator new(sizeof(string));
string::string("Memory Management");  // 调用构造函数

delete ps;
// 等价于：
ps->~string();       // 调用对象的析构函数
operator delete(ps); // 释放内存
```

### 应用1：关闭文件/网络套接字/数据库连接

仅释放相关句柄占用的空间是不够的，还需要执行close操作

```cpp
template<typename Ty>
class Deleter {
public:
    void operator()(Ty *ptr) const {
        cout << "Call a custom method !!!!! " << endl;
        fclose(ptr);  // 关闭文件
    }
};

// 如何使用unique_ptr的自定义销毁器成员？
int main() {
    // lambda也可以作为销毁器
    std::unique_ptr<FILE, Deleter<FILE>> ptr(fopen("data.txt", "w"));
    return 0;
}
```

### 场景2：内存释放时打印日志

```cpp
class FreeMemory {  
public:  
    void operator()(int* p) {  
        std::cout << "delete memory" << std::endl;  
        delete p;  
    }  
};

std::unique_ptr<int, FreeMemory> up1(new int(0)); 
```

### 场景3：自定义分配器（Allocator）—— 只执行析构函数，不释放内存

最佳实践：
- Effective C++（第三版）Item 08：定制new和delete

```cpp
struct Deleter {
    void operator()(T *item) {
        item->~T();  // 只执行析构函数，不释放内存
        tls().put(std::unique_ptr<Storage>(reinterpret_cast<Storage *>(item)));
    }
};
```

### 3.2 为什么使用reinterpret_cast？

| 场景                 | reinterpret_cast | static_cast | const_cast | dynamic_cast |
| ------------------- | ---------------- | ----------- | ---------- | ------------ |
| 指针↔整数转换        | ✅               | ❌          | ❌         | ❌           |
| 不相关类型指针互转    | ✅               | ❌          | ❌         | ❌           |
| 函数指针↔数据指针互转 | ✅               | ❌          | ❌         | ❌           |
| 多重继承无偏移转换    | ✅               | ❌（自动偏移）| ❌         | ❌           |
| 硬件地址直接操作      | ✅               | ❌          | ❌         | ❌           |

- 原文：Converts between types by reinterpreting the underlying bit pattern.

- 简单理解：
  1. 在C++的四种显式转换运算符中，`reinterpret_cast`提供了对底层位模式进行"生硬"重解释的能力。它能在不做任何安全检查或运行时/编译时调整的情况下，将整型、枚举、各种指针、指向成员的指针和引用等类型任意互相转换，这些恰好是其他转换运算符所不支持的场景。

- 优点：不调用构造函数。`reinterpret_cast`只是"重新解释"已有对象或地址的比特位，不会在内存中开辟新空间，也不会触发任何构造或析构操作。

- 风险：**不做对齐修正**。它不负责调整指针对齐，转换后指针如果不满足目标类型的对齐要求，后续解引用可能导致**未定义行为**。

## 小结

ObjectPool设计做到了足够简单且高效，通过巧妙利用C++11特性实现了无锁化的高性能对象池。

--------------------END--------------------------

### 我是谁

**刚刚好，是最难得的美好**

我就在这里，我刚刚好。

![如需帮助，请联系我：wang_cyi](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)

我正在做的事情：

#### **1. 目标：拿百万年薪**
- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。

#### **2. 现状：缺乏实战，渴望提升动手能力**
- 公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 想通过阅读优秀源码，提高代码能力，从"不会写"到"敢写"，提升C++编程自信。
- 需要掌握高效学习和实践方法，弥补缺乏实战经验的短板。

#### **3. 价值：成为优秀完成任务的人，成为团队、公司都认可的核心骨干**
**优秀地完成任务 = 高效能 + 高质量 + 可持续 + 可度量**

**错误示范**：
- 不少同学工作很忙，天天加班，做了很多公司的事情，但不是本团队事情，不是本部门事情，领导不认可，绩效不高。
- 做低优先级的任务，绩效不高，随时可能被优化。

如果您觉得阅读本文对您有帮助，
请点一下"**点赞，转发**"按钮，
您的"**点赞，转发**"将是我最大的写作动力！




