---
title: "TiKV Contributors day3："
date: 2024-07-31
description: "Tidb"
draft: false
tags: ["Tidb"] 
---

1. 跟着启动看，抛开一切的worker，只看主流程。看看怎么启动的。
2. 了解batchsystem，router scheduler这些，只看这个是怎么工作的，不用看业务逻辑。  
    接下来就串起来就行了。  
    大概说下：  
    batchsystem有2个部分：一个是raft相关的，处理raft消息，状态。一个是apply相关的，raftbatchsystem处理完后生成applytask，applybatchsystem应用完后回调。

batchsystem的消息入口来自于grpc，grpc收到后也不是直接调用接口，就是用上面说的router、scheduler这些东西传递消息给batchsystem

 成为tikv贡献者第三天：存储引擎



本文是TiKV 源码阅读第三篇，

目标：snapshot进行进一步理解，一边理解rust语法，一边抽象模块之间关系。



**阅读完你将收益融如下**

1. 容易题不丢分，先吃小甜点+1分，增加学习动力 掌握一个语法知识
    std::move()实现原理 
  - 为什么支持传入的是左值还是右值这2个参数是&&原因吗
  -  为什么返回的都是右值引用，是static_cast功劳吗？
  - forward怎么做到区分类型的，他做不到 还是依赖remove_reference吗？
  -  了解rust 用引用 生命周期用法
    不管什么raft，什么rockdb，本来不明白，完全当作黑盒，  完全是程序员思维。


代码路径：

git@gitee.com:wang_cyi/note_tikv.git


## **回顾：**

# Raft-rs最佳实践
## 任务01:在gitpod.io环境，运行five_mem_node例子 。

- 代码地址：
   https://github.com/watchpoints/raft-rs/blob/master/examples/five_mem_node/main.rs

- TiKV 依赖的周边库 [raft-rs](https://github.com/pingcap/raft-rs) 是参照 ETCD 的 RAFT 库编写的 RUST 版本

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

##   任务02 根据执行结果看代码怎么运行的

信息来源：
- TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-2/
- TiKV 源码解析系列文章（六）raft-rs 日志复制过程分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-6/
-  etcd-raft 源码学习笔记
- raft-rust 初体验




  


  

## 参考

### # TiKV 源码解析系列文章（二）raft-rs proposal 示例情景分析

 参考资料
 - Raft-rs 最佳实践与使用
 - https://github.com/watchpoints/raft-rs

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

  
  
  
  
  
  
  
  
  
