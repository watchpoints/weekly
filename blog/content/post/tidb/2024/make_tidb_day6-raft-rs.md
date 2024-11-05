---
title: "成为Tidb贡献者第四天: raft-rs最佳实践"
date: 2024-10-25
description: Tidb
draft: false
tags:
  - Tidb
---

TiKV 源码解读

## 一、 别想太多，只管去提问,所有问题，都会有答案

> 
> 一个项目需求，一个客户反馈，学习上遇到问题 都是提问来源

> 把问题说清楚，就等于把问题解决了一半，
 但是不要抱怨说整个大环境都这样这样问题，
 作为程序员完全一切权力在客户，公司，测试，运维，
 改变他们不是你目的，加速公司100%发展你也没这个能力，也不是责任
 基于这样假设提问 就是无效沟通
 别人根本不接受，你在这方面根本没有经过他们考验
 你思考漏洞百出，拿不出更好方案，拿不出任何成果。
 相反他们在这样模式下构建百万千万项目，构建高楼大厦，构建成功案例
>
转变思路，你想自己提问，看看自己如何解决。

>


###  1.1 我遇到的问题：直接看TIkv BatchSystem模块有难度，看不懂呀，
### 1.2 别人给出答案:

> 思路1  `TIKV` 的 `BatchSystem` 是实现 `MultiRaft` 的关键模块。在阅读本文章前，大家可以先参

TiKV 依赖的周边库 [raft-rs](https://github.com/pingcap/raft-rs) 是参照 ETCD 的 RAFT 库编写的 RUST 版本
	
翻译：
~~~
	When we began to build TiKV, we researched and investigated many Raft implementations. We eventually decided to go with etcd’s Raft implementation and built our own [Raft](https://github.com/pingcap/raft-rs) using [Rust](https://www.rust-lang.org/),
	
	Although etcd’s Raft implementation is written in Go, it has a simple API with no specific Go feature, thus can be easily ported to Rust.
~~~
	
> 思路2：Talent Plan 实战竞技赛之分布式数据库 TinyKV 开发

    
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




### 1.3 我采取行动：2个事情只能同时做一个


-放弃思路2： 报名Talent Plan 实战竞技赛之分布式数据库 TinyKV 开发 但是你根本时间，最后完不成因此放弃，比赛放弃，但是 TinyKV难度相对低，资源多，从边做一题边写文章方式
https://competition.atomgit.com/competitionInfo?id=234192bbf9b96466d67f25c585661d90
https://github.com/watchpoints/tinykv/tree/course

- 采取思路1：写一篇 raft-rs最佳实践，
   https://github.com/watchpoints/raft-rs

-

# 二、当作项目一样管理 ，说出，那些功能，实现方案 ，需要代码量，多少人月完成 提供整体视图。


> 根本无法说出什么功能，
> 跟无法预估学习多次时间，
> 完全行不通，也无法保证截至日期完成任务。
> 
> 这是看到哪里拿，完全小作坊模式，最后被预期投入很多时间，也完不成。
> 比赛是1个月，但是上班根本没时间，里面大纲参考
> https://competition.atomgit.com/competitionInfo?id=234192bbf9b96466d67f25c585661d90
> https://learn.pingcap.cn/learner/course/390002
> 
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


## 任务02:通过代码准备最少rust语法知识【60分钟】


Q1：vector 为什么不实例化就可以调用new


~~~rust

let (mut tx_vec, mut rx_vec) = (Vec::new(), Vec::new());

impl<T> Vec<T> {

    /// Constructs a new, empty `Vec<T>`.

    ///

    /// The vector will not allocate until elements are pushed onto it.

    ///

    /// # Examples

    // 方法的参数并没有 &self
    pub const fn new() -> Self {

        Vec { buf: RawVec::NEW, len: 0 }

    }
~~~


 
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


>Q3:Rust创建线程中 move || 是什么含义

https://course.rs/advance/concurrency-with-threads/thread.html


- c++ lambda capture

|     |               |
| --- | ------------- |
| [=] | 以值的形式捕获所有外部变量 |
| [&] | 以引用形式捕获所有外部变量 |
- rust ||  一个固定参数 类似c++捕获方式
 - thread::spawn(move ||
  在线程闭包中使用 move

>Q4:  Ok(Msg::Raft(m)) ok是什么用法？

- Understanding Rust Option and Result enums
~~~rust
pub enum Result<T, E> {

    /// Contains the success value

    #[lang = "Ok"]

    #[stable(feature = "rust1", since = "1.0.0")]

    Ok(#[stable(feature = "rust1", since = "1.0.0")] T),

  

    /// Contains the error value

    #[lang = "Err"]

    #[stable(feature = "rust1", since = "1.0.0")]

    Err(#[stable(feature = "rust1", since = "1.0.0")] E),

}

~~~

## 任务03: 描述five_mem_node功能，然后绘制流程图[耗时120分钟]






**应用启动**

Node 是使用 RAFT 的外部应用，代表 RAFT 的一个节点应用程序

	
**相关结接口**


## 任务04: How to use raft-rs

-  Step 1: Create the Raft node
- Step 2: Drive and Run the Raft node
- Step 3: Process the `Ready` State




##   任务04 根据执行结果看代码怎么运行的




  

## 参考
信息来源：
- TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-2/
- TiKV 源码解析系列文章（六）raft-rs 日志复制过程分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-6/
-  etcd-raft 源码学习笔记
- raft-rust 初体验

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


[11] # 浅谈Raft配置变更(成员变更)策略 [10%]
- https://zhuanlan.zhihu.com/p/375059508
- https://www.inlighting.org/archives/raft-membership-change
  之前一直没有深入了解过 Raft 的成员变更，实现也就是在 TinyKV 中搞了一个单步成员变更，以至于在面试的时候，甚至想当然以为成员变更一定要被 apply 后才生效，结果就被挂了。故这里重新梳理一遍，内容是到处扒来的，不一定正确
  
  
  
  
  
  
