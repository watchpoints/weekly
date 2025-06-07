---
title: 成为tikv贡献者第四天：通过 TiKV 入门 Rust
date: 2024-05-29
description: Tidb
draft: false
tags:
  - Tidb
---
https://github.com/facebook/rocksdb/wiki/RocksDB-Contribution-Guide
## 前提条件：已经学会TIKV编译和环境搭建


```SHELL

环境:
window11 +  WSL（Windows Subsystem for Linux）

git clone git@github.com:watchpoints/tikv.git


## 配置你的 Rust 工具链
rustc 是 rust 的编译器，cargo 是 rust 的包管理器
Rust 中的发布有三种：
- Nightly 
- Beta
- Stable

rustc -V
rustc 1.77.0-nightly (89e2160c4 2023-12-27）

编译：
		
make build


参考：https://maiyang.me/post/2018-08-02-rust-guide-by-tikv/

```



## 方法1：我不提交代码 我看别人代码

https://github.com/tikv/tikv/pull/18329

https://github.com/tikv/tikv/pull/18227









