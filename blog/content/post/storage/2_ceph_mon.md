---
title: "日拱一卒：ceph-mon模块"
date: 2023-06-02
description: "you can"
draft: false
tags: ["Cephfs"]
categories: ["Cephfse"]
---



http://localhost:1313/post/storage/2_ceph_mon/







# Mon常用命令



```bash
systemctl stop ceph-mon.target
配置：/var/lib/ceph/mon

```

# Paxos算法



论文：https://sedna.cs.umd.edu/818/papers/paxosMadeSimple.pdf

阅读文档：https://zhuanlan.zhihu.com/p/408735316



 Prepare->promise->propose->accept->learn

1. 提议者（Proposer）

2. 接受者（Acceptor）

3. 学习者（Learner）

   



#  Mon选举流程

## [ceph monitor----初始化和选举](https://www.cnblogs.com/yi-mu-xi/p/10364797.html)



## 代码分析





##  文件：MDSMonitor.cc







