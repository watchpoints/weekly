---
title: 源码解析：
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---


# DeepSeek 3FS 源码解读——磁盘 IO（纳秒） 与网络IO（微妙）设计哲学


先提出一个问题：

为什么 网络IO 有epoll 处理，文件IO没有，

在具体点就是
- 文件IO处理模式  顺序处理: 发起读取请求，等待处理完成。批量处理
- 网络IO模式 异步处理。一次处理多个请求。

## 一、青铜：学会基本使用


#### 1.1 参数配置：storage_main.toml

```shell
[server.aio_read_worker]
	enable_io_uring = true
	ioengine = 'libaio'
	num_threads = 32
	queue_size = 4096
```

>敲重点：项目经验 + 10 **多走一步，配置完毕后 想想** ，异步I/O引擎libaio 与io_uring 区别是什么？

- 零拷贝，零系统调用
-   Linux-native asynchronous I/O：  Libaio提供了一套不同于POSIX接口的异步I/O接口，其目的是更加高效的利用I/O设备
- 与io_uring 主要是为了替代libaio，目前主要应用在存储的场景中
- 在某些场景下使用io_uring + Kernel NVMe的驱动，效果甚至要比使用SPDK 用户态NVMe 驱动更好
- SPDK是由Intel发起、用于加速使用NVMe SSD作为后端存储的应用软件加速库。该软件库的核心是用户态、异步、轮询方式的NVMe驱动

### 1.2 客户端发起读请求

[Client Request] → [RPC Handler] → [生成BatchReadJob] → [AioReadWorker队列]

`AioReadWorker.h` 是DeepSeek 3FS存储服务中异步I/O处理的核心组件，
主要实现以下功能：

异步批量读取管理
- 通过线程池管理多个I/O工作线程
- 使用BoundedQueue实现任务队列（队列大小由 config_.queue_size 控制）
- 峰值吞吐量：≈5GB/s（基于4KB块大小）
- 延迟：平均<2ms（99分位<10ms）

>敲重点：项目经验 + 10 **线程池 +队列方式 IO怎么工作的**？


- 创建n个线程，每个线程 使用各自的io_uring
-
```c++

// folly::CPUThreadPoolExecutor executors_;

Result<Void> AioReadWorker::start(const std::vector<int> &fds, const std::vector<struct iovec> &iovecs) {
  uint32_t numThreads = config_.num_threads();
  for (auto i = 0u; i < numThreads; ++i) {
    executors_.add([&]()
    {
      AioStatus aioStatus;
      IoUringStatus ioUringStatus;
      {
        auto aioInitResult = aioStatus.init(config_.max_events());
     auto ioUringResult = ioUringStatus.init(config_.max_events(), fds, iovecs);
      }
      run(aioStatus, ioUringStatus);
    });
  }
```


阅读笔记

1.  executors_ 是 folly::CPUThreadPoolExecutor executors_;  没有3fs没有自己写线程池，而是直接用第三方库，提高写代码效率
2. 《Folly库代码赏析》7）：Executor
### 1.3 工作线程循环处理

批量处理
![](https://cor3ntin.github.io/posts/iouring/uring.svg)

- 单个读取：最简单例子来说 读取文件

```c++ 
/**
* 读取文件
**/
#include <bits/stdc++.h>
#include <liburing.h>
#include <unistd.h>

char buf[1024] = {0};

int main() {
  int fd = open("1.txt", O_RDONLY, 0);
  
  io_uring ring;
  io_uring_queue_init(32, &ring, 0); // 初始化
  auto sqe = io_uring_get_sqe(&ring); // 从环中得到一块空位
  io_uring_prep_read(sqe, fd, buf, sizeof(buf), 0); // 为这块空位准备好操作
  io_uring_submit(&ring); // 提交任务
  
  io_uring_cqe* res; // 完成队列指针
  
  io_uring_wait_cqe(&ring, &res); // 阻塞等待一项完成的任务
  assert(res);
  std::cout << "read bytes: " << res->res << " \n";
  std::cout << buf << std::endl;
  io_uring_cqe_seen(&ring, res); // 将任务移出完成队列
  io_uring_queue_exit(&ring); // 退出
  return 0;
}
```


> 划重点 项目经验，多想一步 **从 io_uring 的demo 例子和 AioReadWorker有什么区别**？status 采用数据结构和算法怎么设计的 完成发起读请求，等待完成 在线程完成 没有异步，这么简单？提示：io_uring类

```c
Result<Void> AioReadWorker::run(AioStatus &aioStatus, IoUringStatus &ioUringStatus) {

  while (true) {
    // 1. 尝试获取批量读取作业
    auto it = queue_.dequeue();  // 等待
    do {
      // 2. 收集一批读取作业 ，
      status.collect();
      //io_uring_prep_read
      // struct io_uring_sqe *sqe = ::io_uring_get_sqe(&ring_);
      // ::io_uring_sqe_set_data(sqe, &job);
      //submittingJobs_.push_back(&job)
      // 3. 提交一批读取作业
      status.submit();
      //io_uring_submit(&ring_);
      
      // 4. 等待一批事件完成,发起多少个请求
      while (status.inflight()) {
        status.reap(config_.min_complete());
        //io_uring_wait_cqes 等待完成
      };
    } while (status.hasUnfinishedBatchReadJob());
  }

}
```


- 是不是代码很清楚  读取，返回一起处理，没有回调处理，

- `collect()`：聚合待处理 I/O 请求
- `submit()`：提交请求到内核
- `inflight()`：检查进行中的 I/O 数量
- `reap()`：收割已完成事件（支持最小完成数阈值）

```c++
//接口类  java 用interface表示
class IoStatus {
public:
  virtual void collect() = 0; //纯需函数，准备数据 非系统调用
  virtual void submit() = 0; //纯需函数 已经非系统调用

protected:
  AioReadJobIterator iterator_; //指向当前正在调度的批量读取任务
  uint32_t maxEvents_ = 0;//当前允许的最大并发提交数
  
  uint32_t inflight_ = 0; //当前正在执行的异步任务数量

}

class IoUringStatus : public IoStatus {

private:
 //数据结构
  struct io_uring ring_ {}; // 实例，用于内核和用户空间之间的提交/完成队列
  std::vector<AioReadJob *> submittingJobs_; 
  //读取任务,struct io_uring_sqe *sqe
};

https://man7.org/linux/man-pages/man7/io_uring.7.html

```

- **io_uring** is a Linux-specific API for asynchronous I/O.  It allows
       the user to submit one or more I/O requests, which are processed
       asynchronously without blocking the calling proces
- 顺序处理请求

### 1.4  IO出来过程 三步走

收集读取请求：
- IoUringStatus::collect 方法负责收集读取请求并准备提交队列条目：
  算法：循环读取读请求 iterator++
  数据结构：存放放到  submittingJobs_.push_back(&job)

提交请求：

- IoUringStatus::submit 方法负责将收集的请求提交到内核：
  
- 算法：io_uring_submit(&ring_);
 
- IoUringStatus::reap 方法负责等待并处理完成队列事件：



```c++
    1. 等待一批事件完成,发起多少个请求
      while (status.inflight()) {
        status.reap(config_.min_complete());
        //io_uring_wait_cqes 等待完成
        //一个一个等待？
      };
```

# 二、 王者： 发现问题，解决问题 
### 2.1 问题：

发起请求--等待完成（poll机制） 在一个循环处理，
这个容易理解，这个是性能慢呢？等待等待处理完成才能继续下个。

### 2.2 解决办法：

**设计权衡结论**

该顺序处理设计是**性能与复杂度的最佳平衡**：

1. 充分利用硬件特性（FIFO队列+缓存局部性）
2. 避免回调乱序导致的系统复杂性 【最简单代码，实现复杂功能。最难部分不处理】
### 3.3 原因分析


第一性原理是什么：看到新概念，不是陷入别人说多厉害还，第一个最大疑惑和以前 网络io相比，不是一切都是文件吗？**为什么文件AIO不能像epoll一样？**

| **维度**    | **文件AIO**  | **epoll**      |
| --------- | ---------- | -------------- |
| **设计目标**  | 最大化磁盘吞吐量   | 最小化网络延迟        |
| **性能瓶颈**  | 磁盘寻道时间     | 网络数据到达时间       |
| **最佳批大小** | 大批量（MB级）   | 小批量（KB级）       |
| **错误处理**  | 必须显式处理资源不足 | 错误通常来自socket本身 |
 **文件AIO必须循环提交**，因为：

- 磁盘操作没有"就绪状态"概念
- 内核资源限制可能导致部分提交（`-EAGAIN`）
- 需要显式管理请求生命周期

2. **epoll的"一次性处理"不适用**，因为：

- 网络I/O是事件驱动的（数据到达即就绪）
- 不需要预先分配操作资源

3. **现代方案（如io_uring）**
#### 获取文件io结果

```c++
//一次只处理一个请求
void AioStatus::reap(uint32_t minCompleteIn) {
  //函数获取异步 IO 操作结果
  int ret = ::io_getevents();
	for (int i = 0; i < ret; ++i) 
	{
	  auto &event = events_[i];
	  availables_.push_back(event.obj);
	  setReadJobResult(event.data, event.res);
	}

```





存储系统（特别是使用异步I/O的场景）
与网络编程（使用epoll）的核心差异：
前者关注操作提交的资源管理，后者关注事件通知的效率。

**与epoll的性能对比**

| **指标** | **epoll**      | **io_uring** |
| ------ | -------------- | ------------ |
| 系统调用次数 | 每次`epoll_wait` | 零（SQPOLL模式）  |
| 事件延迟   | 微秒级            | 纳秒级（轮询）      |
| 内存拷贝   | 需要             | 可避免          |
| 最大吞吐量  | 约1M QPS        | 可达5M QPS     |
|        |                |              |
**为什么比epoll更高效？**

1. **零系统调用**

SQPOLL模式下内核线程自动处理提交队列。

2. **无状态切换**

用户态直接访问共享环形队列，避免内核-用户态切换。

3. **请求融合**

单个SQE可组合多个操作（如`POLL+READ`）。

4. **精准批处理**

通过`io_uring_peek_batch_cqe`一次获取多个事件，减少遍历开销。

---

通过这种设计，`io_uring`既能实现`epoll`的事件驱动特性，又能发挥存储IO的批处理优势，适用于需要同时处理高并发网络和磁盘IO的场景（如数据库、存储引擎）。
## 总结

 带你全面了解Linux原生异步 IO 原理与使用和 AIO 实现（Native AIO）（超级详细

> Linux-2.6.0 版本内核源码

- 一般来说，使用 Linux 原生 AIO 需要 3 个步骤：
1. 调用 `io_setup` 函数创建一个一般 IO 上下文。
2. 调用 `io_submit` 函数向内核提交一个异步 IO 操作。
3. 调用 `io_getevents` 函数获取异步 IO 操作结果。


> Linux 原生 AIO 实现在源码文件 `/fs/aio.c` 中。



在 `io_uring` 异步 I/O 框架中，一个完整的读操作通常包含以下三个核心步骤：
**1. 收集 (Collect)**

- **目的**：准备 I/O 
- **关键点**：
- 从提交队列（SQ）获取空闲的 `sqe` 结构
- 填充文件描述符、缓冲区、偏移量等参数

**2. 提交 (Commit/Submit)**

- **目的**：将请求提交到内核
- int ret = io_uring_submit(ring); // 提交请求到内核
- - 调用 `io_uring_submit()` 将 SQ 中的请求批量提交
- 内核开始异步执行 I/O 操作
- 返回实际提交的请求数量（非阻塞

**3. 收割 (Reap)**

- **目的**：获取完成结果
-  使用 `io_uring_wait_cqe()` 或 `io_uring_peek_cqe()` 获取完成事件
- 从 `cqe->res` 获取操作结果（负值为错误码）
- 必须调用 `io_uring_cqe_seen()` 释放完成队列项