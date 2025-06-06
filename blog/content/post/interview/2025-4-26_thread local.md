---
title: 面试必备之深入理解thread local【面经复盘】
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
坚持思考，就会很酷。

大家好，这是<font color="#245bdb">进入大厂面试准备--基础知识 第4篇文章</font>，草稿版本

- 草稿版本：意思是内容太多，包含思考，调查 和分析 全部过程，我还无法压缩到一个ppt内完整描述整个事情。
- 演示版本：汇报给领导 ，清楚描述一个事情解决方案。

## 一、面试回顾

时间：2025年 4月28 
岗位：数据库开发工程师
公司：vmare/阿里云/kangao数据库
形式：线下
面试：自己感觉回答很好，但是估计过不了

一面

基础面试：
1. 谈谈你对自旋锁理解
2. 进程通信那个方式
3. 线程局部存储
4. read,write io过程。

项目面试：
1. 干了这么多年， 你角色是什么，一个开发吗？
2. 假如入你独立开发c特性，能不能做？
3. 熟悉英语吗？开源社区 参与中文的还是英文的，英文资料行不行
4. 你学校主任是谁，
5. 在之前公司干什么事情？说的项目，结果 判断不是核心人员。（太武断了）
## 二、谈谈你对ThreadLocal理解

### 2.1 青铜被虐（工作0-5年）：

思考：
1. 听说过没接触过，不知道 怎么实现，然后陷入慌乱，之前线程，进程。
2. 根本想不起来基础知识   局部变量、全局变量、堆、堆栈、静态变量区别 和这个有关系

>划重点：
>1. c++ 不会凭空造一个新概念，都是基于原有基础上发展的
>2. c++ 特性都是依赖编译器，gcc，甚至操作系统。

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514011850.png)
![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514011937.png)


面试官反问：c++中 thread_local 一定是安全的吗？


第一个感觉：是呀绝对完全

无锁情况，多线程安全读写共享同一个变量，

例如 
t0时刻一个元素当前值10，该线程-1之后，期望是9
t1时刻在读取可能不9了，不其他线程修改。
可能导致资源重复释放core情况。
### 实现手段
1. **锁（Mutex/锁）**：串行化访问关键资源。
2. **无锁（Lock‑free/Wait‑free）**：基于原子指令实现，例如比较并交换（CAS）、Fetch‑and‑add 等
3. **线程局部存储**：将状态隔离到各线程自己的副本。




### 2.2 王者归来（5-10年）

#### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题


在Linux系统中使用C/C++进行多线程编程时

- 对于局部变量来说，其不存在线程安全问题
- 全局变量来说 各个线程都可以访问的共享变量，因此它们存在多线程读写问题,采用原子或者锁来解决。

- 如果不采用锁，依然满足在一个线程内部的各个函数调用都能访问、但其它线程不能访问的变量，这就需要新的机制来实现，需要线程局部存储（TLS: Thread-Local Storage）

|​**​维度​**​|​**​栈变量​**​|​**​堆变量​**​|​**​`thread_local` 变量​**​|
|---|---|---|---|
|​**​生命周期​**​|函数作用域|手动管理|线程生命周期|
|​**​访问效率​**​|高（直接栈寻址）|低（需指针间接访问）|高（寄存器或 TLS 直接寻址）|
|​**​线程安全​**​|天然隔离|需同步机制|天然隔离|
|​**​适用场景​**​|短期局部数据|动态分配的长周期数据|线程私有状态（跨函数共享）|

- 在 C++11 之前，开发者需依赖平台特定的 API（如 Linux 的 `pthread_key_create`）实现线程局部存储
- 在c++11中,thread_local用于声明变量具有线程局部存储期。这意味着每个线程都拥有此变量的一个独立实例，每个线程只能访问自己的实例，不同线程间的实例互不影响






####  二、 这个技术适用的场景。任何技术都有其适用的场景

3FS项目中的thread_local使用展示了几个关键模式：

- 用于高效的线程局部缓存以避免重复计算

- 消除线程间的锁竞争，提高并发性能

- 优化内存分配模式，特别是在对象池和工作队列中

- 提供线程安全的随机数生成和状态跟踪


#### 三、技术的组成部分和关键点

| ​**​特性​**​    | ​**​C++ `thread_local`​**​                 | ​**​Java `ThreadLocal`​**​   |
| ------------- | ------------------------------------------ | ---------------------------- |
| ​**​存储结构​**​  | 编译器通过`fs/gs`寄存器直接访问TLS，无哈希表开销              | 基于线程内部的`ThreadLocalMap`哈希表存储 |
| ​**​内存管理​**​  | 线程结束时自动析构（RAII机制）                          | 需手动调用`remove()`，否则可能内存泄漏     |
| ​**​性能​**​    | 接近直接内存访问（1~3周期）                            | 哈希表查询（约10~100周期）             |
| ​**​类成员支持​**​ | 必须为`static`（如`static thread_local int x;`） | 非`static`，通过实例关联             |

### 四、技术的底层原理和关键实现

```c
https://github.com/watchpoints/master-cpp/blob/main/cpp/chapt-3-thread-local/therad1.cpp

/*

* @Date: 2025-05-13 21:09:48

* @Author: 后端开发成长指南   watchpoints

* @FilePath: 知识地图--高频面试题

* @Git: https://github.com/watchpoints/master-cpp
  

#include <iostream>

#include <thread>

#include <memory>
  

//用CPU的视角，解读 thread local 的工作原理

//g++ -S -O2 thread1.cpp -o thread.s

//https://godbolt.org/z/o6njsaan9

thread_local int x = 1; // 线程局部变量（全局作用域）

  

void thread_func() {

std::cout << "Thread " << std::this_thread::get_id()

<< ": x address = " << &x << std::endl;

}
int func()

{

thread_local int b = 2; // 线程局部变量（函数局部作用域）

return b;

// mov eax, DWORD PTR fs:func()::b@tpoff

}


int main()

{

std::thread t1(thread_func); // 创建线程1

std::thread t2(thread_func); // 创建线程2

//同一个变量为什么不同线程，看到不同地址？

//栈也实现，为什么不栈代替

// Thread 140289368270400: x address = 0x7f97a9f6263c

// Thread 140289359877696: x address = 0x7f97a976163c

t1.join();

t2.join();

return 0;

}

```

![代码](https://s2.loli.net/2025/05/14/OcdT2DCEFZlVf5j.png)
在线程创建时，运行时（glibc + dynamic linker）会为每个线程分配两个独立区域：

1. **TLS 块（Thread-Local Storage area）**：包含所有静态和动态 TLS 对象的私有副本，以及线程控制块（TCB）。
2. **线程栈（Thread Stack）**：独立的堆栈区域，用于函数调用、局部变量等。
    
这两个区域通常是分开 mmap／分配的，且在 x86-64 上通过段寄存器（FS 或 GS）来访问 TLS，而通过 RSP/RBP 访问栈

| 区域    | 地址来源             | 访问方式                           |
| ----- | ---------------- | ------------------------------ |
| TLS 块 | FS（或 GS）段基址 + 偏移 | `mov eax, DWORD PTR fs:offset` |
| 线程栈   | RSP/RBP 寄存器      | 通过常规栈寻址（`[rbp-8]`、`[rsp+16]`   |


![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514010349.png)

![fs寄存器切换](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514005618.png)
![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514010504.png)

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514005743.png)
 
 
 #### Local‑Exec TLS 访问模型

##### 编译/链接阶段
- 编译器将所有静态已初始化的 TLS 对象收集到 ELF 的 `.tdata` 节，并由链接器在最终的可执行文件或共享库的 PT_TLS 段内生成一个“TLS 模板”。
- 每个 TLS 符号都会被赋予一个固定的节内偏移 `st_value`，对应重定位类型 `@tpoff`。
##### 运行阶段
1. **线程创建**
    - glibc／动态链接器在 `pthread_create` 时，为新线程 `mmap` 出一块内存，复制主 TLS 模板（`.tdata`＋`.tbss`）并在尾部附加 TCB 结构。
    - 通过 `arch_prctl(ARCH_SET_FS, tcb_address)` 将 `fs` 段基址设置到刚分配的 TCB 处。
2. **直接访问**
    - 之后的 `mov eax, DWORD PTR fs:offset` 便能在单条指令内读出该线程的私有变量，速度媲美普通全局变量读取。
### 五、对比java怎么实现的？

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514012510.png)


### 5.2 3fs实现自己的TLS

src/common/utils/ObjectPool.h



从架构师角度分析：ObjectPool 存储结构 （数据结构） 和读写操作（算法），输出可视化（文本，图片，ppt）



```
┌─────────────────────────────────────────────────────────────┐
│                       对象池内存架构                          │
└─────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────────┐
│                           全局共享区                               │
│                                                                   │
│  ┌─────────────────────────────────────────────────────────────┐  │
	│  │                全局ObjectPool单例 (instance)                 │  │
│  │                                                             │  │
│  │  ┌───────────┐ ┌───────────┐ ┌───────────┐    ┌───────────┐ │  │
│  │  │  批次 #1   │ │  批次 #2   │ │  批次 #3   │... │  批次 #n   │ │  │
│  │  │ (64个对象) │ │ (64个对象) │ │ (64个对象) │    │ (64个对象) │ │  │
│  │  └───────────┘ └───────────┘ └───────────┘    └───────────┘ │  │
│  │                                                             │  │
│  │  访问需要获取mutex_锁                                        │  │
│  └─────────────────────────────────────────────────────────────┘  │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘

                            ↑ 批量交换 ↓
                           (低频、需加锁)

┌───────────────────┐     ┌───────────────────┐     ┌───────────────────┐
│    线程1本地存储    │     │    线程2本地存储    │     │    线程3本地存储    │
│  thread_local TLS  │     │  thread_local TLS  │     │  thread_local TLS  │
│                    │     │                    │     │                    │
│  ┌──────────────┐  │     │  ┌──────────────┐  │     │  ┌──────────────┐  │
│  │    first_    │  │     │  │    first_    │  │     │  │    first_    │  │
│  │ (首选缓存批次) │  │     │  │ (首选缓存批次) │  │     │  │ (首选缓存批次) │  │
│  └──────────────┘  │     │  └──────────────┘  │     │  └──────────────┘  │
│                    │     │                    │     │                    │
│  ┌──────────────┐  │     │  ┌──────────────┐  │     │  ┌──────────────┐  │
│  │   second_    │  │     │  │   second_    │  │     │  │   second_    │  │
│  │ (溢出缓存批次) │  │     │  │ (溢出缓存批次) │  │     │  │ (溢出缓存批次) │  │
│  └──────────────┘  │     │  └──────────────┘  │     │  └──────────────┘  │
│                    │     │                    │     │                    │
│   无锁快速访问      │     │   无锁快速访问      │     │   无锁快速访问      │
└───────────────────┘     └───────────────────┘     └───────────────────┘

```

```css
┌─────────────────────────────────────────────────────────────┐
│                        对象分配流程                           │
└─────────────────────────────────────────────────────────────┘

┌──────────────┐                                           用户代码
│  用户请求对象  │  ObjectPool<T>::get()
└───────┬──────┘
        │
        ▼
┌──────────────┐                                           静态访问点
│ 访问tls()函数 │  static auto &tls()
└───────┬──────┘
        │                             ┌────────────────────┐
        ▼                             │                    │
┌──────────────┐  第一次访问          │  static ObjectPool  │
│ 获取TLS实例   │◄──────创建──────────┤     instance        │
└───────┬──────┘                     │                    │
        │                             └────────────────────┘
        │                                   全局单例
        ▼
┌────────────────────┐
│ TLS.get() 获取内存  │
└─────────┬──────────┘
          │
          ▼
     ┌─────────┐     有                  快速路径
     │second_  │────────┐   ~5纳秒
     │ 有对象? │        │
     └─────────┘        │
          │             │
          │ 无          │
          ▼             │
     ┌─────────┐        │
     │ first_  │        │
     │ 有对象? │        │
     └─────────┘        │
          │             │
      无  │             │
          ▼             │
┌─────────────────┐     │                  中速路径
│ 尝试从全局池     │     │   ~50纳秒
│ 获取一批对象     │     │
└────────┬────────┘     │
         │              │
     失败 │              │
         ▼              │
┌─────────────────┐     │                  慢速路径
│ 创建新的一批对象  │     │   ~200纳秒
└────────┬────────┘     │
         │              │
         │              │
         ▼              │
┌─────────────────┐     │
│ 从first_取出对象 │◄────┘
└────────┬────────┘
         │
         ▼
┌──────────────────────┐
│ 在内存上构造T类型对象  │  placement new
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ 返回对象智能指针给用户 │  std::unique_ptr<T, Deleter>
└───────────────────────┘


┌─────────────────────────────────────────────────────────────┐
│                        对象释放流程                           │
└─────────────────────────────────────────────────────────────┘

┌──────────────┐                                           用户代码
│ 智能指针析构  │  ~Deleter()
└───────┬──────┘
        │
        ▼
┌──────────────┐                                           调用自定义删除器
│ 调用对象析构  │  item->~T()
└───────┬──────┘
        │
        ▼
┌──────────────┐                                           线程局部归还
│ TLS.put()    │  
└───────┬──────┘
        │
        ▼
     ┌─────────┐     可以                  快速路径
     │ first_  │────────┐   ~3纳秒
     │ 未满?   │        │
     └─────────┘        │
          │             │
          │ 已满        │
          ▼             │
┌─────────────────┐     │                 
│ 放入second_     │     │   
└────────┬────────┘     │
         │              │
         ▼              │
     ┌─────────┐  否    │
     │second_  │────────┘
     │ 已满?   │        
     └─────────┘        
          │             
          │ 是          
          ▼             
┌─────────────────┐                        慢速路径
│ 整批移至全局池   │      ~100纳秒(需获取锁)
└────────┬────────┘     
         │              
         ▼              
┌─────────────────┐     
│ 清空second_     │     
└────────┬────────┘     
         │              
         ▼              
┌─────────────────┐     
│ 放入second_     │    
└─────────────────┘     

```


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250514201055.png)



```cpp

char buf[sizeof(Foo)]
Foo* foo = new(buf) Foo(100); 
```

--------------------——END--------------------------



### 我是谁


**刚刚好，是最难得的美好**

我就在这里 ，我刚刚好。


![如果需要帮忙 call we wang_cyi](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


我正在做的事情是
#### **1. 目标：拿百万年薪**

- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
#### **2. 现状：缺乏实战，渴望提升动手能力**

- 公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 想通过阅读优秀的源码，提高代码能力，从"不会写"到"敢写"，提升C++编程自信。
    
- 需要掌握高效学习和实践的方法，弥补缺乏实战经验的短板。

####  3. 价值：成为优秀完成任务，成为团队、公司都认可的核心骨干。

**优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量**
 **错误示范**：
- 不少同学工作很忙，天天加班，做了很多公司的事情。
  但是 不是本团队事情，不是本部门事情，领导不认可，绩效不高
- 做低优先级的任务，绩效不高，随时被优化

如果您觉得阅读本文对您有帮助，
请点一下“**点赞，转发**” 按钮，
您的“**点赞，转发**” 将是我最大的写作动力！



### 参考：第一手资料

- ThreadLocal是Java中实现线程本地存储（TLS）的核心类，通过为每个线程维护独立的变量副本，解决多线程环境下的数据竞争问题
- https://www.bilibili.com/video/BV1Lo4y1g787/
- https://en.cppreference.com/w/cpp/language/storage_duration
- https://www.cnblogs.com/curiositywang/p/18231696
- Facebook的folly库中的ThreadLocal实现
- https://github.com/lefticus/cpp_weekly
- https://www.youtube.com/watch?v=q9_vljSaBDg
- https://www.youtube.com/watch?v=vlskUyjBSQA
- https://github.com/nilaoda/BBDown 【done】
- https://maskray.me/blog/2021-02-14-all-about-thread-local-storage 
1. C++ thread_local adds additional features to `__thread`: dynamic initialization before first-use and destruction on thread exit.
2. Java Concurrency in Practice
3. https://klose911.github.io/html/jvm/thread_safe.html
4. ##### [What is thread-safe initialization of local static objects](https://www.eevblog.com/forum/programming/what-is-thread-safe-initialization-of-local-static-objects/msg4996078/#msg4996078) done





