---
title: 如何利用线程局部变量 实现一个分级存储的缓存结构
date: 2025-05-23
description: do book
draft: false
tags: 
categories:
  - 系统之美
---
坚持思考，就会很酷。

大家好，这是<font color="#245bdb">进入大厂面试准备--基础知识 第8篇文章</font>


这是跟着oceanbase 学c++语法第2篇文章，如果有误，请指针


 ### **读者提问**

看到 ob代码使用thread_local，
请问 thread_local
- 优点是什么？
- 缺点是什么？ 
- 针对缺点如何规避？
- 使用thread_local设计支持多线程的分级存储的cache

![如何利用线程局部变量 实现一个分级存储的缓存结构](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514230017.png)
 
 提示：
 ![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523211425.png)

 ### **为了帮助 通过角色扮演 对话方式**
小义：新手，初学者，候选人
老王：充当面试官，项目经理


 ### **老王：在你了解 thread_local之前，还了解哪些内容** 
小义： 不解释，直接看内容，大概了解一下就可以



![参考:csapp和coolshell](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523150522.png)


- 存储层次：越离CPU近就越小，速度也越快，越离CPU远，速度也越慢

![存储层次 来源csapp](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523151624.png)


- 对同一个地址纯计算任务，6个线程居然跑不过1个线程，所有的线程都会对这个 Cache Line 进行写操作，导致所有的线程都在不断地重新同步 `result[]` 所在的 Cache Line，所以，导致 6 个线程还跑不过一个线程的结果 
- 例子：来源：https://coolshell.cn/articles/20793.html
- 什么是 cache_line

CPU从内存中读取数据的时候是一次读一个cache_line到 cache中以提升效率，
一般情况下cache_line的大小是64 byte（64Bytes也就是16个32位的整型）
这就是CPU从内存中捞数据上来的最小数据单位

>CPU会以一个Cache Line 64Bytes最小时单位加载，也就是16个32bits的整型


![多核cpu](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523151807.png)


 ### **老王：你工作中使用过thread_local吗？怎么使用的** 

小义：

**线程局部存储**（英语：Thread-local storage，缩写：**TLS**）是一种存储持续期（storage duration），
- 对象的存储是在线程开始时分配
- 线程结束时回收
- 每个线程有该对象自己的实例。

根据程序内存布局 **变量的 存储持续期**（生命周期） 分为下面几个类型：
- 局部变量：仅限于函数内部,栈，自动释放。
- 全局(static)变量：伴随着这个程序
- 堆变量：动态分配 
- thread_local ： 不在栈上 Stack，在TLS block中
- TLS 的实现方式（语言相关）。在c++中 （java不是这样实现的，使用完ThreadLocal一定要调用remove，不然会内存泄漏）

代码：https://godbolt.org/z/EaGvq6T88

![cpu指令完成，非内核或者其他框架完成](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523161423.png)

mov edx, DWORD PTR fs:a@tpoff

1.  `fs`： 是 x86_64 架构下用于访问 **线程局部存储（TLS）区域的段寄存器**（Linux/glibc 下）。
2. `a@tpoff` 是编译器在生成代码时的伪操作数，表示变量 `a` 在 TLS block 中的 **偏移量**
3. 每个线程的 `fs` 寄存器指向该线程的 TCB（Thread Control Block）或其 TLS block 的起始位置

>划重点：cpu 在不同线程进行切换的时候，fs寄存器 存储的是 对应不同线程的TLS block 

 CPU上下文切换--线程上下文切换---线程的独立存储

- 中断上下文切换是内核态发生的切换
-  中断不会和进程上下文切换同时发生
    中断事件有着比进程间切换更高优的优先级，所以不会和进程切换同时发生

### **老王：我问的用法，不是语法？** 

小义：太 紧张，没听清楚，也可能是没想清楚，战略的喝口水，缓冲一下。



![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523183103.png)


例如： 统计每个线程申请多少内存

 - 伪代码如下：

```cpp

class ObTenantTxDataOpAllocator : public ObIAllocator

private:
	static thread_local int64_t local_alloc_size_;

//用于跟踪每个线程通过ObTenantTxDataOpAllocator分配的内存总量
thread_local int64_t ObTenantTxDataOpAllocator::local_alloc_size_ = 0;
  
void *ObTenantTxDataOpAllocator::alloc(const int64_t size)
{
	void * buf = alloc(size, abs_expire_time);	
	local_alloc_size_ += size; //不同线程 相关隔离，互补影响，不需要加锁。
	return buf;
}
//查询：不考虑那个线程事情
int64_t get_local_alloc_size() { return local_alloc_size_; }
```

- 如果声明全局变量，需要维护 线程 与线程申请内存关系，必然加锁。
- 如果声明局部变量，无法线程内共享。


### **老王：thread_local 有什么缺点呢，如何优化** 


小义：

转换问题：thread_local 读写其他线程不可见，如果想可见呢怎么办?其他我不考虑
 ![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250523211425.png)

##### TCMalloc is faster than the glibc 2.3 malloc 


- 不装逼，看不懂，跳过
- 了解：c语言提供了 pthread_key_create功能

| **特性​**​       | ​**​C++11 `std::thread`​**​                     | ​**​POSIX `pthread`​**​                                           |
| -------------- | ----------------------------------------------- | ----------------------------------------------------------------- |
| ​**​设计理念​**​   | 面向对象，类型安全，RAII（自动资源管理）                          | 函数式编程风格，需显式管理资源（如手动创建/销毁线程、锁）                                     |
| ​**​可移植性​**​   | 跨平台（标准 C++ 支持，如 Windows、Linux、macOS）            | 主要在类 Unix 系统（Linux、macOS）原生支持；Windows 需第三方库（如 pthreads-win32）<br> |
| ​**​错误处理​**​   | 通过异常机制抛出错误（如 `std::system_error`）               | 返回整数错误码（如 `EAGAIN`、`EINVAL`）                                      |
| ​**​同步机制​**​   | RAII 封装（如 `std::mutex`、`std::lock_guard` 自动加解锁） | 手动初始化/销毁同步对象（如 `pthread_mutex_init()`、`pthread_mutex_destroy()`）  |
| ​**​线程局部存储​**​ | 通过 `thread_local` 关键字声明线程局部变量                   | 使用 `pthread_key_create()`、`pthread_setspecific()` 等函数管理           |
| ​**​线程取消​**​   | 协作式（需通过共享标志位或原子变量控制线程退出）                        | 支持强制终止（`pthread_cancel()`），但需配合清理函数（`pthread_cleanup_push()`）     |
| ​**​API 风格​**​ | C++ 风格，支持 lambda、成员函数、`std::bind` 等灵活调用方式       | C 风格函数接口（如 `pthread_create()` 需传递 `void*` 类型函数指针）                 |
| ​**​线程创建​**​   | 直接构造 `std::thread` 对象，自动启动线程                    | 需调用 `pthread_create()` 并手动管理 `pthread_t` 句柄                       |
####  3FS 对象池

##### 从线程局部变量获取空间地址


![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514201055.png)

数据结构定义：

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


// 从全局获取  加锁 
bool getBatch(Batch &batch) {  
	auto lock = std::unique_lock(mutex_);    // 从全局缓存中弹出一个批次                  batch = std::move(global_.back()); 
	global_.pop_back();    
}

// 从线程本地缓存或全局缓存获取对象
std::unique_ptr<Storage> get() {
    // 从second批次中弹出
    if (!second_.empty()) {
        auto item = std::move(second_.back());
        second_.pop_back(); 
        return item; // 类似CPU的L1缓存
    } 
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
```


### **老王：** class unique_ptr  管理存储池对象，为什么自定义 析构函数？


```cpp
using Ptr = std::unique_ptr<T, Deleter>; // 唯一指针  
  
static Ptr get() {  
  return Ptr(new (tls().get().release()) T);} 

1. tls().get().release() 获取地址空间，可能线程缓存，也可能全局
2. new(addr)T
3. get() 调用 Deleter 释放函数
```



```cpp
struct Deleter {
    void operator()(T *item) {
        item->~T();  // 只执行析构函数，不释放内存
        tls().put(std::unique_ptr<Storage>(reinterpret_cast<Storage *>(item)));
    }
};
```


### 老王：我们在回来看看ob怎么释放一个变量的地址 ，也是这样吗？


小义： 

- 释放
```cpp

template <typename T>
//先别考虑为什么是T**类型
void ob_delete(T *&ptr)
{
	if (NULL != ptr) 
	{
		ptr->~T(); //析构函数，
		ob_free(ptr);//回收地址 不是直接调用C标准库的free()
		ptr = NULL;
   }
}

inline void ob_free(void *ptr)
{
	auto *allocator = lib::ObMallocAllocator::get_instance();
	allocator->free(ptr); //allocator 定义回收方式
	ptr = NULL;
}

inline void ObTCBlock::delete_block(ObTCBlock *blk)
{
	blk->~ObTCBlock(); //析构函数，
	ob_free(blk);
}


void ObMallocAllocator::free(void *ptr)
```

 - 
```cpp
HazptrTLCache& HazptrTLCache::get_instance() {
	thread_local HazptrTLCache tl_cache;
	return tl_cache;
}
```

## 总结
1. 知识：计算机提供分级存储设计，寄存器--缓存--内存--磁盘
2. 作为一个业务开发人员，更考虑是 利用c++11 提供特性：只能指针 和 线程局部变量，写支持多级缓代码，更容易维护代码，**更底层看不懂，不要硬看**
3. 都遇到 同一个问题：多线程  如何隔离 如何共享，如何释放一个地址，从简单例子开始。

多级缓存释放的优势

减少锁竞争:
- 线程本地缓存无需加锁
- 尺寸类级别的锁粒度小，减少全局锁竞争
内存复用效率高:
- 对象首先放入空闲列表，便于快速重用
延迟释放策略:
- 不会立即释放回系统，而是保留在各级缓存中

- 只有当块完全空闲且系统内存压力大时才真正释放

批量释放机制:

- 当缓存过大时触发批量释放


这种多级缓存释放机制使OceanBase能够在高并发环境下高效管理内存，减少系统调用开销，提高内存分配和释放的性能，同时保持较低的内存碎片率
--------------------——END--------------------------




### 我是谁


最动人的作品，为自己而写，刚刚好打动别人

如果您觉得阅读本文对您有帮助，
请点一下“**点赞，转发**” 按钮，
您的“**点赞，转发**” 将是我最大的写作动力！




