---
title: "TiKV Contributors day1"
date: 2024-07-08
description: "Tidb"
draft: false
tags: ["Tidb"]
---

#



[TiKV](https://github.com/tikv/tikv) 是一个支持事务的分布式 Key-Value 数据库，目前已经是 [CNCF 基金会](https://www.cncf.io/projects/) 的顶级项目。

作为一个新同学，需要一定的前期准备才能够有能力参与 TiKV 社区的代码开发，

- 不限于学习 Rust 语言，

- 理解 TiKV 的原理
- 在前两者的基础上了解熟悉 TiKV 的源码。



本文是TiKV （版本7.0 ）源码阅读，成为 TiKV Contributors 第一篇文章

主要介绍 gdb如何调试tikv，执行一个单元测试。

目标：小白如何学习一个开源项目，就是让它跑起来。



**开发环境：gitpod**  

主要电脑配置太低了，开始使用Gitpod 

Gitpod 是一个开源 Kubernetes 应用程序（GitHub 仓库地址 https://github.com/gitpod-io/gitpod)，

适用于可直接编写代码的开发环境，可为云中的每个任务提供全新的自动化开发环境。





编译（不用考虑魔法）：

~~~rust
# rust
curl https://sh.rustup.rs -sSf | sh
export PATH="/workspace/.cargo/bin:$PATH"
source  ~/.bashrc
rustc -V
rustc 1.77.0-nightly (89e2160c4 2023-12-27)

编译
make build
~~~



























# help

【1】 https://maiyang.me/post/2018-08-02-rust-guide-by-tikv/

【2】 https://www.iocoder.cn/TiKV/build-debugging-environment-first/

【3】三十分钟成为 Contributor | 为 TiKV 添加 built-in 函数

















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
