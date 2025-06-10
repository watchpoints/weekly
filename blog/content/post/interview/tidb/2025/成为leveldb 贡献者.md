---
title: 成为leveldb 贡献者
date: 2024-05-29
description: Tidb
draft: false
tags:
  - Tidb
---


## day5

https://github.com/tikv/tikv/pull/18467



https://youjiali1995.github.io/categories/#tikv

https://youjiali1995.github.io/storage/leveldb-write/


## 第一天


整理资料
- https://hardcore.feishu.cn/wiki/LUEtw4SxniCYsoklKF8cypFunab 手写 kv
- https://hardcore.feishu.cn/wiki/XHOfwDMfuiAWAHkyVBFcZ45hnpc
- https://hardcore.feishu.cn/docs/doccn4w8clvork96K3dqQnJRh9g
- https://hardcore.feishu.cn/wiki/QKmowjMW7iQx8wkV867cAaHhnCb
- https://hardcore.feishu.cn/wiki/WHqKwm5EdiAan8kMaC7c6NUcnbg


## 第一天 v1.0 源码解析系列1-leveldb基本数据结构



###


思考一个问题：

KV数据结构是如何存储在磁盘上的？

- 具体来说 我们不关系在磁盘上如何表示的，这个操作系统提供高级抽象。
- 我们关系的是内存数据采用什么数据结构表示，
- 文件 IO 栈是什么。


SST文件是LSM用来存储持久化kv的文件，
其设计充分考虑了持久化，读写性能，存储空间三者的权衡。

