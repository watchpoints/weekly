---
title: "成为Tidb贡献者第四天: raft-rs最佳实践"
date: 2024-10-25
description: Tidb
draft: false
tags:
  - Tidb
---


## 动机
> 动机分类 ：一个项目需求，一个客户不bug反馈，可能是拆解一个复杂任务。
> 你不能做什么
> 
> 不要说价值百万项目必须完成，不要说vip客户问题必须解决，不要说这么简单事情还不会
> 都是别人责任 ，
> 你想一个人把公司推送100发展，发展为他们未来，你也没有这个权力。
> 公司方式固定下来 他们业务是什，客户 是什么，怎么开发，怎么沟通，完全系统。根本不会改正。并且维持运转，就是最好证明。根本不可能事情。
> 因为你提出建议之前：你提出方法不对，内容不对，自己抱怨一下。
> 根本不完成，漏洞百出。
> 你项目需求，设计，研发，解决，根本没有让别人信任能力。
> 别人设计，不停完成一个一个任务。才是研发责任。
> 就是螺丝钉。整个环节如此。


> 你能做什么。
> 把一切转换一个项目管理步骤。
> 你判断是：前进 选择优先级价值更大事情前进，只要是事情 无论多大多难就干
> 你判断是：放弃，缺失各自资源，暂停。
> 
> 只要到说出合理原因 时间不够，功能多，等就延迟在延迟。
>  把一切转换一个项目管理步骤




1. 我遇到的问题：直接看TIkv BatchSystem模块有难度，看不懂呀，
2. 别人给出答案:
    1. `TIKV` 的 `BatchSystem` 是实现 `MultiRaft` 的关键模块。在阅读本文章前，大家可以先参考一些官方博客：
    
3.  采取行动1--rust：决定Raft-rs库开始降低开始。
4. 采取行动2--golang：报名 Talent Plan 实战竞技赛之分布式数据库 TinyKV 开发
     https://competition.atomgit.com/competitionInfo?id=234192bbf9b96466d67f25c585661d90

  
	TiKV 依赖的周边库 [raft-rs](https://github.com/pingcap/raft-rs) 是参照 ETCD 的 RAFT 库编写的 RUST 版本
	
	翻译：
	
	When we began to build TiKV, we researched and investigated many Raft implementations. We eventually decided to go with etcd’s Raft implementation and built our own [Raft](https://github.com/pingcap/raft-rs) using [Rust](https://www.rust-lang.org/),
	
	Although etcd’s Raft implementation is written in Go, it has a simple API with no specific Go feature, thus can be easily ported to Rust.


|                                     |                                                                                                                                                          |             |                        |         |
| ----------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------- | ---------------------- | ------- |
| **程名称**                             | **主要内容**                                                                                                                                                 | **课时**      | **课程时间**               | **主讲人** |
| TinyKV 项目概述                         | 1. 概要介绍 TinyKV 内容与课程安排  <br>2. 介绍初赛规则  <br>3. TinyKV 开发演示                                                                                                | 45min～90min | 2024/10/22 19:00-20:30 | 董菲      |
| Project1 辅导 - Standalone KV 实现      | 1. 理解 KV 数据库的工作模式  <br>2. 了解 Standalone KV 程序框架                                                                                                          | 45min～90min | 2024/10/23 19:00-20:30 | 张擎洋，张家坤 |
| Project2 辅导 - Part1: Raft 算法实现      | 1. 理解领导人选举、日志复制等功能的实现方法  <br>2. 了解 Tiny KV 项目中 Raft 算法的框架  <br>3. 了解 Client 层与 Raft 层间消息的收发和处理流程。                                                        | 45min～90min | 2024/10/24 19:00-20:30 | 杨洪宇，张驭云 |
| Project2 辅导 - Part2: Snapshot 实现    | 1. 理解 TinyKV 中 Store、Peer 和 Region 的关系  <br>2. 了解指令传递、处理和数据持久化的流程。  <br>3. 了解 Snapshot 相关内容，包括日志压缩、快照生成、快照收发和快照应用。  <br>4. 了解 TinyKV 中对于 Snapshot 的处理流程。 | 45min～90min | 2024/10/25 19:00-20:30 | 贾然钧，叶泽坤 |
| Project3 辅导 - Part1: Multi- Raft 实现 | 1. 理解 Muti-Raft  <br>2. 理解 TransferLeader、ConfChange 和 Split 行为  <br>3. 了解 TinyKV 中 TransferLeader、ConfChange 和 Split 的处理流程                              | 45min～90min | 2024/10/26 19:00-20:30 | 黄必胜     |
| Project3 辅导 - Part2: Scheduler 实现   | 1. 理解 Scheduler 的作用和工作原理  <br>2. 了解心跳信息的处理流程  <br>3. 了解调度处理流程                                                                                            | 45min～90min | 2024/10/27 19:00-20:30 | 黄必胜     |
| Project4 辅导 - 事务实现                  | 1. 理解 MVCC 原理并掌握代码实现要点。  <br>2. 理解分布式事务两阶段提交原理。  <br>3. 了解 TinyKV 中两阶段提交的实现要点与执行流程                                                                       | 45min～90min | 2024/10/28 19:00-20:30 | 翁思扬     |
| TinyKV 项目总结                         | 1. 回顾之前课程主题要点  <br>2. 集中总结常见问题  <br>3. 介绍后期安排                                                                                                            | 45min～90min | 2024/10/29 19:00-20:30 | 董菲      |


## 任务01:在gitpod.io 创建项目运行环境  [耗时5分钟]

- 代码地址：
   https://github.com/watchpoints/raft-rs/blob/master/examples/five_mem_node/main.rs



仓库中的 `examples/five_mem_node/main.rs` 

它创建了一个 5 节点的 Raft 系统，并进行了 100 个 proposal 的请求和提交


环境搭建：gitpod.io


![image.png](https://s2.loli.net/2024/10/22/wKpRHMfThSZx6i7.png)

- 明白一些命令

~~~
curl https://sh.rustup.rs -sSf | sh
export PATH="/workspace/.cargo/bin:$PATH"
source  ~/.bashrc
rustc -V

//02--Rust toolchain
rustup toolchain install nightly

rustc 1.80.0 (051478957 2024-07-21)
~~~

`Raft` is built using the latest version of `stable` Rust, using [the 2018 edition](https://doc.rust-lang.org/edition-guide/rust-2018/). Minimum supported version is `1.44.0`.

Using `rustup` you can get started this way:

```shell
rustup component add clippy
rustup component add rustfmt
```

In order to have your PR merged running the following must finish without error:

```shell

cargo run  //首先对项目进行编译， 然后再运行 cargo build
cargo run --release 提高运行速度  
./target/release/world_hello

当项目大了后，cargo run 和 cargo build 不可避免的会变慢，那么有没有更快的方式来验证代码的正确性呢？
cargo check //快速的检查一下代码能否编译通过

cargo test --all && \  
cargo clippy --all --all-targets -- -D clippy::all   && \ 
//用于捕捉常见错误和改进 Rust 代码
// Clippy 是一个 Rust 代码的静态分析工具，
//用于检测代码中的潜在错误、不规范的代码风格以及其他问题

//`--all`：这个选项告诉 Clippy 检查所有的包，包括依赖的包
//`--all-targets`：这个选项指示 Clippy 不仅检查库代码，还会检查所有的二进制目标（

// 并禁用 Clippy 的所有默认 lints，例如当开发者想要使用 Clippy 检查代码，但又不想受到 Clippy 默认 lints 的限制时
cargo fmt --all -- --check    //通过 rustfmt 自动格式化
```

You may optionally want to install `cargo-watch` to allow for automated rebuilding while editing:

```shell
cargo watch -s "cargo check"

gitpod /workspace/raft-rs (master) $ cargo watch -s "cargo check"
[Running 'cargo check']
    Checking crossbeam-utils v0.8.20
   Compiling raft-proto v0.7.0 (/workspace/raft-rs/proto)
    Checking crossbeam-channel v0.5.13
    Checking slog-async v2.8.0
    Checking slog-envlogger v2.2.0
    Checking raft v0.7.0 (/workspace/raft-rs)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 2.76s
[Finished running. Exit status: 0]
```


## 任务02:通过代码准备最少rust语法知识






 **代码1：**

 ~~~rust
 
  let (tx_stop, rx_stop) = mpsc::channel();
 
  pub fn channel<T>() -> (Sender<T>, Receiver<T>)

 ~~~
原文：

Creates a new asynchronous channel, returning the sender/receiver halves. All data sent on the [`Sender`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html "struct std::sync::mpsc::Sender") will become available on the [`Receiver`](https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html "struct std::sync::mpsc::Receiver") in the same order as it was sent, and no [`send`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html#method.send "method std::sync::mpsc::Sender::send") will block the calling thread (this channel has an “infinite buffer”, unlike [`sync_channel`](https://doc.rust-lang.org/std/sync/mpsc/fn.sync_channel.html "fn std::sync::mpsc::sync_channel"), which will block after its buffer limit is reached). [`recv`](https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html#method.recv "method std::sync::mpsc::Receiver::recv") will block until a message is available while there is at least one [`Sender`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html "struct std::sync::mpsc::Sender") alive (including clones).  

翻译：

创建一个新的异步通道，返回发送方/接收方的一半。 [`Sender`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html "struct std::sync::mpsc::Sender")发送的所有数据将按照发送顺序在[`Receiver`](https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html "struct std::sync::mpsc::Receiver")上可用，并且任何[`send`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html#method.send "method std::sync::mpsc::Sender::send")都不会阻塞调用线程（此通道具有“无限缓冲区”，与[`sync_channel`](https://doc.rust-lang.org/std/sync/mpsc/fn.sync_channel.html "fn std::sync::mpsc::sync_channel")不同，后者在其缓冲区限制达到后将阻塞）达到）。在至少有一个[`Sender`](https://doc.rust-lang.org/std/sync/mpsc/struct.Sender.html "struct std::sync::mpsc::Sender") （包括克隆）存在的情况下， [`recv`](https://doc.rust-lang.org/std/sync/mpsc/struct.Receiver.html#method.recv "method std::sync::mpsc::Receiver::recv")将阻塞，直到消息可用为止。

原文：

>pub fn try_recv(&self) -> Result<T, TryRecvError>
  
Attempts to return a pending value on this receiver without blocking.

This method will never block the caller in order to wait for data to become available.
Instead, this will always return immediately with a possible option of pending data on the channel.

翻译： 同步阻塞 IO vs 同步非阻塞 IO vs异步 io
类比：ssize_t read(int fd, void *buf, size_t len);


![](https://www.cyhone.com/img/noblocking-io/block-read.png)

|      |                     |                      |
| ---- | ------------------- | -------------------- |
| read | 阻塞下                 | 非阻塞                  |
| 无数据  | 一直等待                | 立马返回-1，error置为EAGAIN |
| 有数据  | 返回数据的大小（不超过指定的读入大小） | 返回数据的大小（不超过指定的读入大小）  |
| cpu  | 不占用过多cpu            | 不停重试占用过多cpu          |
|      |                     |                      |
o_uring 是 2019 年 Linux 5.1 内核首次引入的高性能 异步 I/O 框架，能显著加速 I/O 密集型应用的性能。

$ cat /etc/ceph/ceph.conf
[osd]
bluestore_ioring = true
...
**代码2** Arc::clone



```
 let proposals: Arc<Mutex<VecDeque<Proposal>>> = Arc::clone(&proposals);

  
  Stack                    Heap
  -----                    ----


  annas:
+--------+               +------------+
| ptr: o-|-------------->| count: 3   |
+--------+    ^          | data: 🐢   |
              |          +------------+
 peters:      |
+--------+    |
| ptr: o-|----+
+--------+    ^
              |
  bobs:       |
+--------+    |
| ptr: o-|----+
+--------+
```


## 任务03: 描述five_mem_node功能，然后绘制流程图

## 任务04: 通过例子看懂如何使用raft-rs（不是如何实现）


##   任务04 根据执行结果看代码怎么运行的

信息来源：
- TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-2/
- TiKV 源码解析系列文章（六）raft-rs 日志复制过程分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-6/
-  etcd-raft 源码学习笔记
- raft-rust 初体验




  


  

## 参考

**2024-10-25**

-  Implement Raft in Rust
 https://www.pingcap.com/blog/implement-raft-in-rust/

- # raft-rs 示例程序源码解读
   https://tidb.net/blog/c7b79ced
- TIKV BatchSystem 概述
    https://tidb.net/blog/bdf740f2
### # TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析

 参考资料
 - Raft-rs 最佳实践与使用
 - https://github.com/watchpoints/raft-rs
 - https://tidb.net/blog/e95e5c6b

-  TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析
    https://cn.pingcap.com/blog/tikv-source-code-reading-2/


- TiKV 源码解析系列文章（十九）read index 和 local read 情景分析

- TiKV 源码解析系列文章（十）Snapshot 的发送和接收

- PointGet的一生

- Rust 参考手册 中文版

- TiKV RocksDB读写原理整理

- TinyKV Course: RaftStore执行流程

- TiDB 技术内幕 – 说存储

- TiKV 源码解析系列 - Raft 的优化

  
  
  
  
  
  
  
  
  
