---
title: "TiKV Contributors day1"
date: 2024-07-08
description: "Tidb"
draft: false
tags: ["Tidb"]
---



[TiKV](https://github.com/tikv/tikv) 是一个支持事务的分布式 Key-Value 数据库，目前已经是 [CNCF 基金会](https://www.cncf.io/projects/) 的顶级项目。

需要一定的前期准备才能够有能力参与 TiKV 社区的代码开发，

- 不限于学习 Rust 语言，

- 理解 TiKV 的原理
- 在前两者的基础上了解熟悉 TiKV 的源码。



本文是TiKV （版本7.0 ）源码阅读，成为 TiKV Contributors 第一篇文章

目标：

- 小白如何学习一个开源项目，先让它跑起来。

- 主要介绍 gdb如何调试tikv，运行单元测试。

- 搭建一个单机TiKV 服务。

  

**开发环境：gitpod**  



主要电脑配置太低了，开始使用Gitpod 

Gitpod 是一个开源 Kubernetes 应用程序（GitHub 仓库地址 https://github.com/gitpod-io/gitpod)，

适用于可直接编写代码的开发环境，可为云中的每个任务提供全新的自动化开发环境。



**通过Rustlings学习Rust语法**

[Rustlings](https://github.com/rust-lang/rustlings) 是一个由 Rust 团队维护的开源项目，旨在帮助你通过调试代码来学习 Rust。它也会一路为你提供提示。

这个是我的练习

https://github.com/cicvedu/rustlings-semester-4-watchpoints





**成为贡献者需要掌握那些知识**

- [Contributing to TiKV](https://github.com/watchpoints/tikv/blob/master/CONTRIBUTING.md)

- [A map that guides what and how contributors can contribute](https://github.com/pingcap/tidb-map/blob/master/maps/contribution-map.md#tikv-distributed-transactional-key-value-database)

- https://github.com/pingcap/talent-plan

  

  ~~~shell
  这个我没有看呢，有一起的吗
  TP 101: Introduction to open source software
  TP 102: How to use Git and GitHub
  TP 103: Build a welcoming community
  TP 201: Practical Networked Applications in Rust
  TP 202: Distributed Systems in Rust
  TP 301: TinySQL, a distributed relational database in Go
  TP 302: TinyKV, a distributed key value database in Go
  TP 401: Deep Dive into TiDB(WIP)
  TP 402: Deep Dive into TiKV(WIP)
  ~~~

  

  

**TiKV 调试环境搭建：**

~~~rust
//01-rust
curl https://sh.rustup.rs -sSf | sh
export PATH="/workspace/.cargo/bin:$PATH"
source  ~/.bashrc
rustc -V
rustc 1.77.0-nightly (89e2160c4 2023-12-27)

//02--Rust toolchain
rustup toolchain install nightly
//  nightly version 

rustup component add rustfmt
// rustfmt 是 Rust 的官方代码格式化工具，它可以根据 Rust 社区的代码风格指南自动格式化你的 Rust 代码。

rustup component add clippy
//clippy 是 Rust 的一个静态代码分析工具  cargo clippy 
// Run Rustfmt
make format
//Run Clippy (note that some lints are ignored, so `cargo clippy` will give many false positives)
make clippy

// 03-编译 https://github.com/watchpoints/tikv
make build //

cargo install cargo-watch
cargo watch -s "cargo check"  
//使用 cargo watch 的好处是，
//它可以帮助你在开发过程中快速发现编译错误，而不必每次手动运行 cargo check 或 cargo build

// 当你准备测试修改的代码，可以使用 dev 指令，它将格式化你的代码库，在启用 clippy 的情况下构建，并运行测试
make dev  //执行全部单元测试，有点慢



运行特定单元测试
cd /workspace/tikv/tests/integrations/server
执行特定单元测试
cargo test test_rawkv  //LLVM ERROR: IO failure on output stream: No space left on device 30空间没有了。
//是否包含debug信息
RUSTFLAGS=-Cdebuginfo=2 make dev
RUSTFLAGS=-Cdebuginfo=2 cargo build
//RUSTFLAGS=-Cdebuginfo=2 (for full debuginfo)

~~~



### Rust LLDB 调试入门

这个gdb方式实现，后面完善。



### 单节点集群部署 这个网上很多例子

https://tikv.org/docs/7.1/deploy/install/test











# help

【1】 https://maiyang.me/post/2018-08-02-rust-guide-by-tikv/

【2】 https://www.iocoder.cn/TiKV/build-debugging-environment-first/

【3】三十分钟成为 Contributor | 为 TiKV 添加 built-in 函数

【4】 https://xuanwo.io/2021/03-how-to-contribute-to-tikv-copr/

【5】https://github.com/YunaiV/Blog/tree/master/TiKV

 [6] https://www.cnblogs.com/jiangshifu/p/16365201.html tikv调试环境

[7]https://kaige.org/2019/06/04/Learning-TiDB-4/

[8] [`rust-gdb`是什么？](https://github.com/fucking-translation/blog/blob/main/src/lang/rust/14-%E4%BD%BF%E7%94%A8GDB%E8%B0%83%E8%AF%95Rust%E5%BA%94%E7%94%A8.md)

【9】https://tikv.org/docs/7.1/deploy/install/test/#install-binary-manually

​      https://tikv.org/docs/7.1/deploy/install/test/#install-binary-manually

https://docs.pingcap.com/tidb/stable/tiup-playground











  























# todo01：搭建tikv环境

-------------------------------------------------------------

### task01：搭建tikv环境（容器版本，和源码版本）



~~~
在CentOS上进行TiDB/PD/TIKV编译分享

路径：
umount /dev/nvme1n1
mkdir -p /data/kv
mkfs.ext4 /dev/nvme1n1
mount /dev/nvme1n1  /data/kv


cd /data/kv

cd tikv && make && mkdir -p ./bin && cp ./target/release/tikv-server bin

brew install rust
https://senzhangai.github.io/programming/build-tidb-by-source
https://tidb.net/blog/7e4c77a0


~~~





## tod02:学习资料



- 阅读文章：https://asktug.com/t/topic/693645

- 【源码合集】TiKV 源码阅读三部曲

  https://mp.weixin.qq.com/s/HgflwnZZMHXaIsFV3PdGAg

- 阅读  Rust 参考手册 中文版
  https://rustwiki.org/zh-CN/reference/expressions/if-expr.html

- TiKV 源码阅读三部曲（一）重要模块

本小节将简单介绍 KVService 及其启动流程，并顺带介绍 TiKV 若干重要结构的初始化流程

- TiKV 源码阅读三部曲（二）读流程
