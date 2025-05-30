---
title: 面试必备之深入理解自旋锁【草稿版本】
date: 2024-08-07
description: do book
draft: false
tags:
  - LINUX系统调用
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
## 二、谈谈你对自旋锁理解

### 2.1 青铜被虐（工作0-5年）：

自旋锁是相对互斥锁而言的，

对liunx内核实现我不清楚，回答犹犹豫豫，给面试官不好印象

- 后记：没有区分角色 普通开发人员想内核开发人员事情，普通开发人员视角是什么，上来气势就输了。

根本不知道面试回答什么，大海捞针呀？
里面很多概念我哪里开始？

- 后记：你相信自己在基础知识方面我最厉害的，虽然面试无数人积累丰富经验，他们不会像我一样花费时间投入研究，让自己回答满意，自己考虑欠缺什么。

我全部回答出来了，为什么还是不满意。

- 后记：面试官也是程序员每天大量任务，面试时间一个题目给2-3分钟回答。简要回答，像汇报领导方式，让不不懂人了解核心，不是讲故事方式。


这个题目之前我准备过。
- UNIX环境高级编程，UNIX网络编程
- 深入理解Linux内核
- 深入理解算计操作系统
-  陈硕老师Linux高性能服务器编程

当初学习时候，快速翻页，囫囵吞枣学习，现在根本记不清楚了，
不是后悔时候，快速调整心态后

回答如下






### 2.2 青铜反思：

记住这痛苦时刻，因为平时根本没有机会让自己清醒时刻

1. 工作10年和回答是刚毕业时候用法，根本不行，我面试核心岗位，一个题目价值百万，仅仅刚毕业了解回答根本不行。【太简单不行】
2. 天然的从最底层原理考虑，让自己直接吓退，也不行，面试官他们自己也没研究明白，这样回答他们不感兴趣【太难不行，都是在应用】
3. 已经回答清楚了，，面试官反问 是真的吗？或者还有呢，自己可能是错误的 还是沉迷错误继续回答，还可能是回答没问题，就是不自信，提供更多证据【面试看着要有大局观 同时关键细节把握】

必须盯住第一波攻击，谈谈xx理解，实现功能都是烟雾弹，这个三天三夜完毕，因此最后还是通用基础考察，不同追问，追问 ，在追问，如果回答不出来，马上标记pass。


划重点：看别人怎么使用技术的。

- 源头：项目中需要实现功能，Liunx已经提供了，了解他们机制。其他开源产品。
- 模仿：这些机制被不同公司包装不同产品（轮子），让用户使用，甚至提供api服务
- 应用：说自己提供xxx服务，多厉害，他们更多用户需求，更更多案例，都是基本流程，基本场景大量问题，不是自己搞一套从来没有东西。
- 我该怎么:这样自己思路豁然开朗。不是高不可攀，直接被吓退，我可以的，我可以的，我可以的.书本概念，到源码实现，到产品封装，最后解决用户问题。日常生活中出发 打破砂锅问到底。


**下面是基本思路** 

 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题
 二、这个技术的优势和劣势分别是什么 

 三、这个技术适用的场景。任何技术都有其适用的场景

 四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

 六、已有的实现和它之间的对比




**搜集相关提问，读题目分类**

- 自旋锁和互斥锁区别  
- 自旋锁优点  
- 自旋锁永远不死锁吗
- 自旋锁和二阶信号量的差别；  
- 中断中用自旋锁还是互斥量；
- 互斥锁和自旋锁（while）哪个快？（自旋锁快）
- 临界区概念，同步技术  
- 锁知道那些？  +2
- 互斥锁和自旋锁区别 +2
- 讲一下自旋锁？+1 
- 自旋锁怎么阻止别的线程调用？+1
- 什么情况下会用自旋锁？+1
- 了解哪些锁？互斥锁 读写锁 自旋锁 自旋锁CPU高为什么还用自旋锁？
- 从零开始自己动手写自旋锁
- - 自旋锁，怎么解决多 CPU 下同时访问自旋锁的性能问题+1
- 可重入锁和自旋锁有什么区别
- volitate 底层原理 说了一些面试官不满意 +1 

### 2.3 王者归来（5-10年）

#### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

![自旋锁发展历史](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250512220646.png)

#### **什么是 自旋锁**

自旋锁（spin lock）是一种在**高并发场景下多线程环境中用于同步的机制**，
它允许线程在尝试获取一个公共资源时，如果公共资源暂时不可用，线程不会进入睡眠状态，
而是在一个循环中不断尝试获取资源，直到成功为止。这种方式被称为“自旋”


自旋锁的特点是适用于锁的持有时间非常短的场景，
因为它在等待期间不会主动放弃 CPU，
而是不断尝试获取锁，
这在多核系统中可以避免由于线程调度带来的上下文切换开销

- spinlock优点：
没有昂贵的系统调用，一直处于用户态，执行速度快 
- spinlock缺点：
- 一直占用cpu，而且在执行过程中还会锁bus总线，锁总线时其他处理器不能使用总线 
- mutex优点：不会忙等，得不到锁会sleep mutex
- 缺点：sleep时会陷入到内核态，需要昂贵的系统调用


## 一、发展历程：结合 Linux 版本看自旋锁演进

| Linux 版本 | 时间    | 主要进展             | 自旋锁相关说明                                |
| -------- | ----- | ---------------- | -------------------------------------- |
| 2.0      | 1996  | 支持 SMP，BKL 引入    | 全局大内核锁，串行化所有内核操作，无需细粒度锁                |
| 2.2      | 1999  | 引入基础自旋锁机制        | 初步在某些子系统中使用 `spinlock_t`，开始去BKL化       |
| 2.4      | 2001  | 更广泛使用自旋锁         | 自旋锁与 `cli/sti`（关中断）结合使用，锁粒度更细          |
| 2.6      | 2003  | 高度并发与抢占内核        | 引入 `spin_lock_irqsave()` 等多种锁版本，应对复杂场景 |
| 3.x      | 2011  | BKL 基本移除         | 通过细粒度自旋锁和 mutex 替代 BKL                 |
| 4.2      | 2015  | 引入 **qspinlock** | MCS 队列自旋锁替代原始的 ticket lock，提高扩展性       |
| 5.x      | 2018+ | NUMA、多核优化        | 更复杂的锁策略：退避、统计锁竞争、结合 RCU 等              |
### 1. **锁竞争激烈、CPU 浪费严重**
- **问题**：多个 CPU 自旋等待同一个锁时，会浪费大量 CPU 周期，尤其在临界区较长或锁被频繁争用的场景下。
- **解决策略**：
    - **自旋退避（backoff）策略**：在等待失败后加入延时，避免一直争抢总线。
    - **TTAS（Test-and-Test-and-Set）**：减少 cache line 抖动。
    - **MCS 自旋锁 → qspinlock（Linux 4.2）**：将等待线程链入队列，避免活锁并提高公平性

### **中断嵌套导致死锁**

- **问题**：在中断和进程上下文中使用相同锁且未正确关中断，可能造成死锁。
- **解决策略**：
    - Linux 提供不同版本的锁接口：
        - `spn_lock()`：基本版本
        - `spin_lock_irq()`：关本地中断
        - `spin_lock_irqsave()`：保存并关闭中断
        - `spin_lock_bh()`：禁止软中断
    - 开发者根据上下文选择合适版本，保证中断嵌套安全。

### 4. **NUMA 架构下锁竞争代价高**
- **问题**：在 NUMA 系统中，访问跨节点共享数据时，自旋锁导致 cache 频繁失效，性能下降。
- **解决策略**：
    - MCS 队列锁：按顺序获取，减少 cache 抖动。
    - qspinlock（从 4.2 起默认使用）：结合 MCS 与 ticket lock 的优点，兼顾低延迟与扩展性。




####  二、 这个技术适用的场景。任何技术都有其适用的场景



In low-level programming, busy-waits may actually be desirable. It may not be desirable or practical to implement interrupt-driven processing for every hardware device, particularly those that are seldom accessed.

Sometimes it is necessary to write some sort of control data to hardware and then fetch device status resulting from the write operation, 

status that may not become valid until a number of machine cycles have elapsed following the write

大内核锁”（Big Kernel Lock，BKL）是 Linux 早期为快速支持 SMP 而引入的一种全局自旋锁。它的主要缺点包括：
1. **严重的可扩展性瓶颈**
    - BKL 会把整个内核（或大部分子系统）当作一个临界区，一次仅允许一个 CPU 执行内核代码。随着 CPU 数量增加，锁竞争剧增，系统整体吞吐量几乎线性下降。
    - 在多核系统上，绝大多数并发工作都会被串行化，造成 CPU 空转和资源浪费。
2. **高延迟和响应不佳**
    - 当一个 CPU 持有 BKL 时，其他 CPU 即便是在用户态空闲，也必须忙等（自旋）或被阻塞，导致中断和系统调用的响应时间大幅波动。
    - 对实时或延迟敏感的任务不友好，无法满足更严格的服务质量需求。
3. **影响内核抢占与中断处理**
    - 使用 BKL 时往往需要禁止内核抢占（preemption）或关中断，以避免死锁，这进一步降低了内核对高优先级中断或实时任务的处理能力。
    - 禁用抢占/关中断的时间越长，对系统延迟的负面影响越明显。


BKL 最大的问题在于“粗粒度、全局化”
——它通过最简单的方式保证并发安全，却牺牲了多核并行性能与内核可预期的响应特性



![这个技术适用的场景](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250519123945.png)


分类
1. 传统的自旋锁（Spinlock）,满足比较并交换(compare and swap，CAS)/TAS（test and set）特性 ，Linux 2.0.0（1996 年 6 月 9 日）从单个cpu发展对称多处理（SMP），解决并发竞争问题，缺点：无法保证公平性，可能导致饥饿（为何传统的自旋锁存在不公平现象）
2. 票证自旋锁 Ticket Spinlock , 2008 年 12 月 17 日发布，正式将票据自旋锁作为默认可选的公平锁机制合并入主，2.6.26 版本，业务原理：取号，叫号机制 实现方式c++11提供内存模型和原子操作,缺点：当CPU数据巨大并且竞争激烈的时候，自旋锁状态数据对应的cacheline会在不同cpu上跳来跳去，从而对性能产生影，还是用银行叫号来类比，假设现在2号客户的业务办理完了，接下来就该在大厅里叫3号，然后3号客户去办理，但是所有排号的，4号、5号……哪怕是20号，也得听一下叫的号。
3.  可扩展锁 MCS（Mellor‑Crummey and Scott）锁是一种基于链式队列的自旋锁算法，最早发表于 1991 年，旨在解决传统自旋锁在多处理器系统中因所有处理器自旋在同一缓存行而导致的缓存抖动和可扩展性差等问题,Linux 内核从 3.12 版本开始在 x86_64 平台采用 **qspinlock**（queued spinlock），其慢路径即基于 MCS 算法

**可扩展性强**：在高核数或分布式共享内存（NUMA）系统中，表现优于测试并设置锁（TAS）和票据锁（Ticket Lock),


https://mfukar.github.io/2017/09/08/ticketspinlock.html
https://lwn.net/Articles/590243/

自旋锁仍然存在一个根本性的问题：每次尝试获取锁都需要将包含该锁的缓存行移至本地 CPU。对于存在争用的锁，这种缓存行弹跳会严重损害性能
通过将自旋锁扩展为每个 CPU 的结构，MCS 锁能够消除简单锁所遇到的大部分缓存行反弹问题，尤其是在竞争情况下。

一旦完成此分配，CPU 2 将在_其自身的_ mcs_spinlock 结构中的 locked 值上自旋，而不是主锁中的值。因此，它的自旋将完全在 CPU 本地进行，完全不影响主锁

因此，MCS 锁比常规自旋锁稍微复杂一些。但这种增加的复杂性消除了竞争情况下的缓存行反弹；它也是完全公平的，按照 CPU 到达的顺序将锁传递给每个 CPU。


论文：https://www.cs.rochester.edu/u/scott/papers/1991_TOCS_synch.pdf?utm_source=chatgpt.com

Linux 内核从 3.12 版本开始在 x86_64 平台采用 **qspinlock**（queued spinlock），其慢路径即基于 MCS 算法

**可扩展性强**：在高核数或分布式共享内存（NUMA）系统中，表现优于测试并设置锁（TAS）和票据锁（Ticket Lock),

资料：https://pdos.csail.mit.edu/6.828/2019/lec/faq-locks.txt


https://coral.googlesource.com/linux-imx/%2B/refs/tags/4-2/kernel/locking/qspinlock.c?utm_source=chatgpt.com



#### 三。 技术的组成部分和关键点

小提示：
- 这地方不需要细节，例如我每天开车，需要对发动机了如指掌吗？
  不知道，只知道有什么型号发动机，让提速等关键效果就可以，陷入细节，让你根本无法说出几个特性？就这简单，有多少个？
- 需要了解是什么 ，整体统计，有哪些组成部分，别遗漏了
- 有整体统计，对自己感兴趣一个部分进行深入研究


**CAS算法** 即compare and swap（比较与交换） 无锁编程基础，
非阻塞同步（Non-blocking Synchronization）







### 2.4 总结：重要几个关键词是什么


### 五、技术的底层原理和关键实现


spinlock是最基础的一种锁，像后面将要介绍的rwlock(读写锁)，seqlock(读写锁)等都是基于spinlock衍生出来的。

就算是mutex，它的实现与spinlock也是密不可分。
因此，本系列文章将首先围绕spinlock展开介绍


**自旋锁的实现原理**
- 自旋锁的实现依赖于原子操作
- spinlock用一个整形变量表示，其初始值为1，表示available的状态。
- 当一个CPU（设为CPU A）获得spinlock后，会将该变量的值设为0
- **CAS**操作（Compare And Swap）


```C
https://github.com/torvalds/linux/blob/master/arch/sh/include/asm/spinlock-cas.h

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
    while (!__sl_cas(&lock->lock, 1, 0));
}

static inline unsigned __sl_cas(volatile unsigned *p, unsigned old, unsigned new)
{
    __asm__ __volatile__("cas.l %1,%0,@r0"
        : "+r"(new)
        : "r"(old), "z"(p)
        : "t", "memory" );
    return new;
}

```
https://compiler-explorer.com/z/8v9E5vjYv

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250516230555.png)




```js
.LBB0_1:
    cmp     dword ptr [rbp - 4], 10000000   ; (1) 比较 i 与 10 000 000
    jge     .LBB0_4                        ; (2) 如果 i >= 10 000 000 跳出

    lea     rdi, [rip + sum1]             ; (3) 取 sum1 地址，放入 rdi 作为 this
    call    std::__atomic_base<int>::operator++()  ; (4) 调用原子自增

    mov     eax, dword ptr [rip + sum2]   ; (5) 读 sum2
    add     eax, 1                        ; (6) eax = eax + 1
    mov     dword ptr [rip + sum2], eax   ; (7) 写回 sum2

    mov     eax, dword ptr [rbp - 4]      ; (8) 读 i
    add     eax, 1                        ; (9) i = i + 1
    mov     dword ptr [rbp - 4], eax      ; (10) 写回 i

    jmp     .LBB0_1                       ; (11) 跳回循环开头

.LBB0_4:
    ; 循环结束，返回/退出

call std::__atomic_base<int>::operator++()
会展开为带 `LOCK` 前缀的读-改-写指令，
例如 `lock inc dword ptr [rip + sum1]`，
确保在多核环境下对该地址的操作是原子的
```
 
 ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250516232418.png)
https://arangodb.com/2021/02/cpp-memory-model-migrating-from-x86-to-arm/?utm_source=chatgpt.com

| 顺序                     | 语义保证                               | 开销  |
| ---------------------- | ---------------------------------- | --- |
| `memory_order_relaxed` | 仅保证原子性，不保证与其他操作的顺序或可见性             | 最低  |
| `memory_order_acquire` | 保证此操作后续的读写不会在指令序上移到此操作之前           | 较低  |
| `memory_order_release` | 保证此操作前的读写不会在指令序上移到此操作之后            | 较低  |
| `memory_order_acq_rel` | 同时具备 acquire 和 release 语义，对应读写双向屏障 | 中等  |
| `memory_order_seq_cst` | 全局顺序一致性；在所有线程中可见操作顺序完全一致           | 最高  |


### 看 folly::SpinLock实现

folly::MicroSpinLock` 是在 Folly 中为超细粒度锁场景设计的极小自旋锁，
实现为一个 POD 结构，仅占用一个字节。

它通过在用户态对一个字节标志位执行**原子比较-交换**（CAS）来尝试获取锁，失败后使用 `detail::Sleeper` 进行“退避”自旋，直至成功，

从而避免任何系统调用或内核阻塞，保证极低的延迟和良好的跨平台可移植性




```cpp

https://github.com/facebook/folly/blob/main/folly/SpinLock.h
SpinLock--->folly::MicroSpinLock 

/*
 * folly::MicroSpinLock
 * ====================
 * A really, *really* small spinlock for fine-grained locking of lots
 * of teeny-tiny data. 
 * 
 * Designed to be POD (Plain Old Data) so it can be
 * packed into other structures without extra overhead.
 *
 * 特性：
 *  1. 极小自旋锁，适用于超细粒度场景
 *  2. POD 类型，可零初始化，相当于调用 init()
 *  3. 用户态实现，无系统调用开销
 *  4. 支持动态检测工具插桩（ThreadSanitizer 等）
 */

struct MicroSpinLock {
  // FREE 表示锁可用，LOCKED 表示锁已被持有
  enum { FREE = 0, LOCKED = 1 };
  //
  // 用一个字节存储锁状态，避免使用 std::atomic<>，保持 POD 特性
  //为什么用uint8_t表示 std::atomic<>？ 
  //这个和序列化有什么关系？怎么实现的
  typedef unsigned char           uint8_t;
  uint8_t lock_; //locked?
  

  /**
   * 尝试获取锁，非阻塞
   * @return true: 获得锁；false: 未获得
   */
  bool try_lock() noexcept {
    // 调用 xchg 将新值置为 LOCKED，返回旧值
    bool ret = (xchg(LOCKED) == FREE);
    // 插桩：记录尝试获取锁的结果，便于动态分析
    annotate_rwlock_try_acquired(
        this, annotate_rwlock_level::wrlock, ret, __FILE__, __LINE__);
    return ret;
  }

  /**
   * 获取锁，阻塞自旋
   * 在持锁竞争时，会进行退避等待以降低总线抖动
   */
  void lock() noexcept {
    detail::Sleeper sleeper;
    // 自旋尝试交换，如果旧值不为 FREE，表示竞争失败
    while (xchg(LOCKED) != FREE) {
      // 进入退避等待循环
      do {
        sleeper.wait();  // 调用 pause/yield 或指数退避
      } while (payload()->load(std::memory_order_relaxed) == LOCKED);
    }
    // 加锁成功，断言当前状态为 LOCKED
    assert(payload()->load() == LOCKED);
    // 插桩：记录获取锁的事件
    annotate_rwlock_acquired(
        this, annotate_rwlock_level::wrlock, __FILE__, __LINE__);
  }

  /**
   * 释放锁
   * 将状态设为 FREE，并使用 release 语义保证可见性
   */
  void unlock() noexcept {
    // 断言当前持锁状态
    assert(payload()->load() == LOCKED);
    // 释放锁 
    payload()->store(FREE, std::memory_order_release);
  }

 private:
  /**
   * 内部：获取指向 lock_ 字段的原子指针
   */
  std::atomic<uint8_t>* payload() noexcept {
    // reinterpret_cast 到 std::atomic<uint8_t>* 类型
    return reinterpret_cast<std::atomic<uint8_t>*>(&this->lock_);
  }

  /**
   * 原子交换操作，将 lock_ 原子地置为 newVal，返回旧值
   */
  uint8_t xchg(uint8_t newVal) noexcept {
    return std::atomic_exchange_explicit(
        payload(), newVal, std::memory_order_acq_rel);
  } //A read-modify-write operation with this memory order is both an _acquire operation_ and a _release operation_
};

// 编译时断言：类型必须满足标准布局且平凡类型，以保证 POD 特性
static_assert(
    std::is_standard_layout<MicroSpinLock>::value &&
    std::is_trivial<MicroSpinLock>::value,
    "MicroSpinLock must be kept a POD type.");


```
用c++11 实现一个Ticket spinlocks



原理：
 ![取号，叫号机制](https://www-x-wowotech-x-net.img.addlink.cn/content/uploadfile/201806/511b1528532887.png)
```c++
参考：https://mfukar.github.io/2017/09/08/ticketspinlock.htm

  now_serving   next_ticket
        |             |
        V             V
... 0 1 2 3 4 5 6 7 0 1 2 ...
         \___________/
           8 threads
           holding one ticket each

struct TicketSpinLock {
    /**
     * Attempt to grab the lock:
     * 1. Get a ticket number
     * 2. Wait for it
     */
    void enter() {
        /* We don't care about a specific ordering with other threads,
         * as long as the increment of the `next_ticket` counter happens atomically.
         * Therefore, std::memory_order_relaxed.
         */
        const auto ticket = next_ticket.fetch_add(1, std::memory_order_relaxed);

        while (now_serving.load(std::memory_order_acquire) != ticket) {
            spin_wait();
        }
    }

    /**
     * Since we're in the critical section, no one can modify `now_serving`
     * but this thread. We just want the update to be atomic. Therefore we can use
     * a simple store instead of `now_serving.fetch_add()`:
     */
    void leave() {
        const auto successor = now_serving.load(std::memory_order_relaxed) + 1;
        now_serving.store(successor, std::memory_order_release);
    }
 
```
 



### 多cpu竞争下的可扩展的自旋锁

参考
1. 英文：https://lwn.net/Articles/590243/
2. Linux中的spinlock机制[二] - MCS Lock
3. Linux中的spinlock机制[三] - qspinlock
4. https://systemsresearch.io/posts/f22352cfc/

以解决在锁的争用比较激烈的场景下，cache line无谓刷新的问题，

其主要思想是让每个微调器在其各自的 per-CPU 变量上旋转，从而避免不同 CPU 之间不断的缓存行跳动。

当所有 CPU 都自旋等待同一个锁变量时，就会发生缓存行跳动，这会导致它们重复读取此变量。当一个 CPU 解锁时，此变量会被修改，从而使所有其他 CPU 的缓存行无效，然后这些 CPU 必须重新读取该变量。

这会导致性能开销。MCS 锁通过让每个 CPU 在其各自的专用变量上旋转来缓解这个问题，从而避免了对单个锁变量的争用。

先来看一下有3个以上的CPU持有或试图获取spinlock时，等待队列的全貌
![每个cpu自选本地缓存](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250519174704.png)

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250519173222.png)

 
 
 ### 六、已有的实现和它之间的对比
 
 #####  C++ 如何实现自旋锁？

在 C/C++ 编程中, 只有 Linux`pthread` 库提供了自旋锁相关接口，而在 C++ 标准库中，并没有直接提供自旋锁（spinlock）的接口。

不过，你可以使用 `std::atomic_flag` 来实现一个简单的自旋锁，
因为 `std::atomic_flag` 是一个轻量级的原子布尔标志，非常适合构建自旋锁。


### 参考：第一手资料
1. https://en.wikipedia.org/wiki/Busy_waiting
2. 《Understanding the Linux Kernel》
3. https://zhuanlan.zhihu.com/p/534680224
4. https://zhuanlan.zhihu.com/p/89058726
5. 宋宝华： 几个人一起抢spinlock，到底谁先抢到？

#### 6. Linux中常见同步机制设计原理 
- http://www.wowotech.net/kernel_synchronization/445.html
- 摘要：

![取号，叫号机制](https://www-x-wowotech-x-net.img.addlink.cn/content/uploadfile/201806/511b1528532887.png)

#### [7] Linux中的spinlock机制[二] - MCS Lock

-  Linux中的spinlock机制[一] - CAS和ticket spinlock
- https://zhuanlan.zhihu.com/p/80727111
-  Intel PAUSE指令变化影响到MySQL的性能，该如何解决？
- https://mp.weixin.qq.com/s/dlKC13i9Z8wjDDiU2tig6Q

- MCS locks and qspinlocks https://lwn.net/Articles/590243/
- 小义疑问：有原子了，为什么还要加锁



#### [8]  Intel PAUSE指令变化影响到MySQL的性能，该如何解决？ ✅
- https://mp.weixin.qq.com/s/dlKC13i9Z8wjDDiU2tig6Q
- 本文将分享一个工作中的实践案例：因Intel PAUSE指令周期的迭代，
- 引发了MySQL的性能瓶颈，美团MySQL DBA团队如何基于这三者来一步步进行分析、定位和优化。希望这些思路能对大家有所启发。
- https://www.intel.cn/content/www/cn/zh/products/details/processors/xeon/scalable/silver.html CPU负载相对较高。
- TPS等吞吐量下降
- 从内核3.10.0-229开始，引入了新的加锁机制，MCS锁。可以降低spinlock的开销，从而更高效地运行。普通spinlock在多CPU Core下，同时只能有一个CPU获取变量，并自旋，而缓存一致性协议为了保证数据的正确，会对所有CPU Cache Line状态、数据，同步、失效等操作，导致性能下降。而MSC锁实现每个CPU都有自己的“spinlock”本地变量，只在本地自旋。避免Cache Line同步等，从而提升了相关性能。不过，社区对于spinlock的优化争议还是比较大的，后续又有大牛基于MSC实现了qspinlock，并在4.x的版本上patch了。

#### [9] # mysqld进程 ut_delay 占用率过高 ✅
- https://blog.csdn.net/qq_42604176/article/details/123487725
- perf top -p 如果看到热点函数是ut_delay或者_raw_spin_lock的话，说明锁争用比较严重
- show variables like '%spin%';
- **nnodb_sync_spin_loops是在放弃CPU时间片之前进行多少次等待，innodb_spin_wait_delay是一次等待的时间。**  
这里直接将`innodb_sync_spin_loops=0`

#### 【10】 深入理解MySQL中的CPU自旋锁及其调优实践 ✅
- perf top 工具观察 系统函数调用情况, ut_delay比较突出，TPS 3-4w/s (不符合预期）
- 接着调整 MySQL 的 `spin_lock` 相关的参数，效果如下，insert 性能提升2倍
- 在 MySQL 系统设计中，特别是 InnoDB 存储引擎使用自旋锁来控制对其内部数据结构的访问，以实现高性能和并发。InnoDB存储引擎具有复杂的并发控制机制，自旋锁在其中扮演了重要角色。
- MySQL关于spin lock的部分代码。如下代码可以看到MySQL默认作了30次（innodb_sync_spin_loops=30）mutex检查后，才放弃占用CPU资源。

MySQL InnoDB存储引擎 spin-lock 加锁失败遇到冲突后 也是采取sleep方式在尝试，避免空耗cpu


#### [11] # 自旋锁的原理与优化 #63
- https://github.com/kunpengcompute/kunpengcompute.github.io/issues/63
- 很多语言都提供了封装后的TAS和CAS调用方法。
- cpu __atomic_test_and_set 【遗留作业】


#### [12] book -C++反汇编与逆向分析技术揭秘

#####【13】BKL: The Big Kernel Lock
- https://litux.nl/mirror/kerneldevelopment/0672327201/ch09lev1sec8.html?utm_source=chatgpt.com
- 它的创建是为了简化从 Linux 原始的 SMP 实现到细粒度锁的过渡
- BKL 是一种递归锁。单个进程可以多次获取该锁，而不会像自旋锁那样发生死锁。
- 这些特性有助于简化从内核 2.0 到 2.2 版本的过渡。在内核 2.0 版本中引入 SMP 支持时
- 引入 BKL 是为了帮助简化向更细粒度的锁定的过渡。当时它是​​一个很好的助手；现在它却成了可扩展性的负担
- 由于大内核锁是一种全局锁，因此在多处理器系统中，它可能会限制内核的可扩展性，因为每个处理器都需要等待锁的释放

#### 【14】 原子操作和自旋锁区别？

- `std::atomic<T>`（C++11）
- https://chatgpt.com/c/6821ef72-dbd4-8012-8756-ebfa83cf6b67

- In the GPU world, where C++11 atomics do not exist yet, the happens-before relationship is created with a threadfence instruction. In CUDA, you would do:  
在 GPU 世界中，C++11 原子操作尚不存在，这种先行发生关系是通过 threadfence 指令创建的。
使用这些方法替换伪代码中的`get_and_set(locked)`，就能快速实现自旋锁。
- **原子操作** 仅作用于**单个变量**或指针更新，无法保证多条指令间的一致性
- **自旋锁** 可保护任意大小的临界区（多条语句、复杂数据结构），在进入锁和退出锁之间的所有操作均被串行化 。
- ### 死锁与上下文限制

- **原子操作** 无锁语义，不会导致死锁；可在几乎任何上下文（包括中断、信号处理等）安全使用 
- **自旋锁** 如果在持锁期间发生阻塞（如意外调用睡眠 API）或中断上下文未禁用中断，可能导致死锁或优先级反转
- https://stackoverflow.com/questions/52337878/why-isnt-cas-compare-and-swap-equivalent-to-busy-wait-loops?utm_source=chatgpt.com

#### [13]  CppCon 2017: C++ atomics, from basic to advanced. What do they really do?” 【没有看明白】 ❎

- https://www.youtube.com/watch?v=ZQFzMfHIxng

#### 【14】# 解读folly库中的自旋锁：MicroSpinLock 和PicoSpinLock



【15】C++ & Linux 后端：进BAT的学习路线
https://leetcode.cn/discuss/post/3151340/c-linux-hou-duan-jin-batde-xue-xi-lu-xia-ktkl/

- https://zhuanlan.zhihu.com/p/512336828
- https://github.com/iceCream1997/muduo_server_learn 大并发服务器课程源代码

#### 【16】票证自旋锁
- https://mfukar.github.io/2017/09/08/ticketspinlock.html?utm_source=chatgpt.com
- 

#####【17】
- # 一行一行源码分析清楚 AbstractQueuedSynchronizer (二)

- https://pdos.csail.mit.edu/6.828/2019/lec/faq-locks.txt?utm_source=chatgpt.com
[5]. 提供各种锁定方案的背景和动机的文章https://zhuanlan.zhihu.com/p/100546935
【6】https://zhuanlan.zhihu.com/p/100546935

# Linux中的spinlock机制[三] - qspinlock

#### 【20】 # Scalability Techniques for Practical Synchronization Primitives  
实用同步原语的可扩展性技术
- https://queue.acm.org/detail.cfm?id=2698990
- 一个明显的例子是共享内存系统的同步原语（锁）的设计和实现
- 随着日益庞大的多核和众核 NUMA（非一致性内存访问）系统的出现，糟糕的锁定实现所带来的性能损失变得愈发明显

【21】 # 第37问：自旋锁 旋着旋着人就糊涂了
追求 MySQL 的性能时，总听说要调整自旋锁的参数: innodb_spin_wait_delay 和 innodb_sync_spin_loops，是真的么？
https://opensource.actionsky.com/20210521-mysql/


【22】 http://mysql.taobao.org/monthly/2017/01/01/

【23】 [The Linux Foundation](https://www.youtube.com/@LinuxfoundationOrg)


	如果您觉得阅读本文对您有帮助，
	请点一下“**点赞，转发**” 按钮，
	您的“**点赞，转发**” 将是我最大的写作动力！
