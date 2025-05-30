---
title: 面试官最爱问的自旋锁，你真的懂了吗？一文带你从入门到精通
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---

面试官：你知道什么是自旋锁？

<font color="#245bdb">我能说不知道吗，我只能说知道</font>

面试官：伸手一摆，说说你理解

<font color="#245bdb">大部分项目都使用互斥锁，遇到阻塞时不会占用CPU，所以很少使用自旋锁...</font>

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250516220535.png)

各位老师好，我是小义，喜欢研究一些底层技术知识。

本文是大厂面试拆解第6篇文章，主要详细描述：
1. 三种不同类型自旋锁的实现和使用场景
2. 自旋锁的主要技术要点：原子操作和公平性，并结合C++11用户态的实现进行分析

![自旋锁发展](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250519123945.png)
希望在无锁编程方面对你有帮助。

## 一、整体面试回顾

时间：2025年4月28日
岗位：数据库开发工程师
形式：线下

一面

基础面试：
1. 谈谈你对自旋锁的理解
2. 进程通信的方式
3. 线程局部存储
4. read/write IO过程

项目面试：
1. 干了这么多年，你的角色是什么，是开发吗？
2. 假如让你独立开发C++特性，能不能做？
3. 熟悉英语吗？开源社区参与中文的还是英文的，英文资料阅读能力如何？
4. 你学校主任是谁？
5. 在之前公司主要负责什么项目？

## 二、青铜（工作1-3年）怎么回答的：谈谈你对自旋锁的理解

这个题目之前我准备过，参考了以下书籍：
- UNIX环境高级编程
- UNIX网络编程
- 深入理解Linux内核
- 深入理解计算机操作系统
- 陈硕老师Linux高性能服务器编程
- Linux多线程服务端编程：使用muduo C++网络库

当初学习时，快速翻页，囫囵吞枣，现在很多细节都记不清楚了。
不过现在不是后悔的时候，需要快速调整心态。

回答如下：

自旋锁常用于高并发场景下，相比互斥锁：
1. 通过循环占用CPU，为了防止中断，需要禁用中断
2. 没有锁队列，仅用一个整数表示当前是否加锁状态
3. 在多线程竞争情况下，保证原子性
4. 了解C++11提到的新特性Compare-and-Swap (CAS)，但理解不够深入，没有展开说明

> 点评：这个题目有陷阱，"谈谈xxx"不是发散题目，
> 而是需要聚焦使用场景和采用的技术。看到资料都是10年前的，
> 而且学习过程中没有完全理解，对10年后的发展了解不够。

多个人竞争一个自旋锁时存在以下问题：
- CPU性能差异导致的不公平：就像跑步比赛，腿长腿短会影响结果（CPU性能差异）
- 缓存命中率影响：就像排队时玩手机，注意力不集中就会错过叫号（缓存未命中）
这些问题如果处理不好就会影响系统性能，需要合理解决。

###### Linux自身发展

![自旋锁发展历史](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250512220646.png)

![这个技术适用的场景](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250519123945.png)

1. 传统的自旋锁（Spinlock）满足比较并交换(Compare and Swap，CAS)/TAS（Test and Set）特性。Linux 2.0.0（1996年6月9日）从单CPU发展到对称多处理（SMP），用于解决并发竞争问题。缺点是无法保证公平性，可能导致饥饿现象。

2. 票证自旋锁（Ticket Spinlock）于2008年12月17日发布，在2.6.26版本中正式将票据自旋锁作为默认可选的公平锁机制。其业务原理采用取号、叫号机制，通过C++11提供的内存模型和原子操作实现。缺点是当CPU数据量巨大且竞争激烈时，自旋锁状态数据对应的cacheline会在不同CPU间频繁跳动，从而影响性能。就像银行叫号系统，当2号客户业务办理完成后，需要叫3号客户，而所有排队的客户（4号、5号...甚至20号）都需要监听叫号。

3. 可扩展锁MCS（Mellor‑Crummey and Scott）是一种基于链式队列的自旋锁算法，最早发表于1991年。它解决了传统自旋锁在多处理器系统中因所有处理器自旋在同一缓存行而导致的缓存抖动和可扩展性差等问题。Linux内核从3.12版本开始在x86_64平台采用**qspinlock**（queued spinlock），其慢路径即基于MCS算法。

现代高性能并发库和语言运行时（如Java并发包中的`AbstractQueuedSynchronizer`）也普遍采用MCS或类似的链式队列锁实现，以提升多核环境下的锁性能。

###### 不同开源平台如何使用自旋锁（用户态）

应用示例：
1. 在3FS代码库中，使用自旋锁(folly::SpinLock)用于统计和监控相关的场景

## 二、王者（工作5-10年）怎么回答的：谈谈你对自旋锁的理解

思考要点：
1. 即使了解底层实现，也不会主动从汇编和内核角度回答，除非面试官特别提问。避免给自己制造麻烦。
2. 从内核原理、业务框架和C++特性的统一性角度来回答。

我的理解如下：

自旋锁是一种低延时场景下的多线程同步机制，采用自旋等待（Busy‑Waiting）方式。

技术要点：
1. 采用比较并交换等原子技术设置状态变化，由C++内存模型保证原子性
2. 在加锁失败情况下，通过退避算法/Ticket Lock/Queue Lock避免CPU空转
3. 优化方式：
   - **纯用户态实现**：与`std::mutex`等基于futex的互斥量不同，自旋锁完全通过原子指令实现，无需进入内核态
   - **无阻塞上下文切换**：由于没有阻塞或唤醒操作，不存在内核调度开销，适用于对延迟敏感且持锁时间极短的场景

### 从cpu角度解释：自旋锁怎么实现的（非系统调用 指令层面实现）

https://compiler-explorer.com/z/8v9E5vjYv

![汇编解释](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250516230555.png)

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

```shell
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
####  源码解读：用户态 folly::SpinLock实现

Folly **是一个由Facebook 开源的C++ 库，旨在为大规模服务器开发提供高效的工具和组件**

folly::MicroSpinLock`  是在 Folly 中为超细粒度锁场景设计的极小自旋锁，

1. 实现为一个 POD 结构，仅占用一个字节，它通过在用户态对一个字节标志位执行**原子比较-交换**（CAS）来尝试获取锁，
2. 失败后使用 `detail::Sleeper` 进行"退避"自旋，从而避免任何系统调用或内核阻塞，保证极低的延迟和良好的**跨平台可移植性**。

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
    // 释放锁  write
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
  } //A read-modify-write operation with this memory order is both an _acquire operation_ and a _release operation_
};

// 编译时断言：类型必须满足标准布局且平凡类型，以保证 POD 特性
static_assert(
    std::is_standard_layout<MicroSpinLock>::value &&
    std::is_trivial<MicroSpinLock>::value,
    "MicroSpinLock must be kept a POD type.");

```

- MySQL InnoDB存储引擎 spin-lock 加锁失败遇到冲突后 也是采取sleep方式在尝试，避免空耗cpu

### 用c++11 实现一个Ticket spinlocks

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
 

c++ 内存模型

| 顺序                     | 语义保证                               | 开销  |
| ---------------------- | ---------------------------------- | --- |
| `memory_order_relaxed` | 仅保证原子性，不保证与其他操作的顺序或可见性             | 最低  |
| `memory_order_acquire` | 保证此操作后续的读写不会在指令序上移到此操作之前           | 较低  |
| `memory_order_release` | 保证此操作前的读写不会在指令序上移到此操作之后            | 较低  |
| `memory_order_acq_rel` | 同时具备 acquire 和 release 语义，对应读写双向屏障 | 中等  |
| `memory_order_seq_cst` | 全局顺序一致性；在所有线程中可见操作顺序完全一致           | 最高  |
|                        |                                    |     |
|                        |                                    |     |
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

### 总结
| Linux 版本 | 时间    | 主要进展             | 自旋锁相关说明                                |
| -------- | ----- | ---------------- | -------------------------------------- |
| 2.0      | 1996  | 支持 SMP，BKL 引入    | 全局大内核锁，串行化所有内核操作，无需细粒度锁                |
| 2.2      | 1999  | 引入基础自旋锁机制        | 初步在某些子系统中使用 `spinlock_t`，开始去BKL化       |
| 2.4      | 2001  | 更广泛使用自旋锁         | 自旋锁与 `cli/sti`（关中断）结合使用，锁粒度更细          |
| 2.6      | 2003  | 高度并发与抢占内核        | 引入 `spin_lock_irqsave()` 等多种锁版本，应对复杂场景 |
| 3.x      | 2011  | BKL 基本移除         | 通过细粒度自旋锁和 mutex 替代 BKL                 |
| 4.2      | 2015  | 引入 **qspinlock** | MCS 队列自旋锁替代原始的 ticket lock，提高扩展性       |
| 5.x      | 2018+ | NUMA、多核优化        | 更复杂的锁策略：退避、统计锁竞争、结合 RCU 等              |

 **锁竞争激烈、CPU 浪费严重**
- **问题**：多个 CPU 自旋等待同一个锁时，会浪费大量 CPU 周期，尤其在临界区较长或锁被频繁争用的场景下。
- **解决策略**：
    - **自旋退避（backoff）策略**：在等待失败后加入延时，避免一直争抢总线。
    - **TTAS（Test-and-Test-and-Set）**：减少 cache line 抖动。
    - **MCS 自旋锁 → qspinlock（Linux 4.2）**：将等待线程链入队列，避免活锁并提高公平性
 
 **NUMA 架构下锁竞争代价高**
- **问题**：在 NUMA 系统中，访问跨节点共享数据时，自旋锁导致 cache 频繁失效，性能下降。
- **解决策略**：
    - MCS 队列锁：按顺序获取，减少 cache 抖动。
    - qspinlock（从 4.2 起默认使用）：结合 MCS 与 ticket lock 的优点，兼顾低延迟与扩展性。

## 课后作业 无锁队列的实现

参考：https://coolshell.cn/articles/8239.html

--------------------——END-----------

如果您觉得阅读本文对您有帮助，
请点一下"**点赞，转发**" 按钮，
您的"**点赞，转发**" 将是我最大的写作动力！

## 参考
1.  票证自旋锁https://mfukar.github.io/2017/09/08/ticketspinlock.html
2. A simple correctness proof of the MCS contentionfree lock



