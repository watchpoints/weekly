---
title: 成为tikv贡献者第四天：通过 TiKV 入门 Rust
date: 2024-05-29
description: Tidb
draft: false
tags:
  - Tidb
---



成为facebook/rocksdb 贡献者


## day1

阅读：https://github.com/facebook/rocksdb/wiki/RocksDB-Contribution-Guide

https://groups.google.com/g/rocksdb?pli=1

https://stackoverflow.com/questions/tagged/rocksdb

开发环境搭建（1天）


```
# 克隆代码库
git clone --recurse-submodules https://github.com/facebook/rocksdb.git
# 编译 Debug 版本（便于调试）
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWITH_TESTS=1
make -j4
# 运行单元测试
make check
```