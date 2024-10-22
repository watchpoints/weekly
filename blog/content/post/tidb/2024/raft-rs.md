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


### 任务01：把项目运行起来，运行examples提供例子 

- https://github.com/watchpoints/raft-rs/blob/master/examples/five_mem_node/main.rs

- TiKV 依赖的周边库 [raft-rs](https://github.com/pingcap/raft-rs) 是参照 ETCD 的 RAFT 库编写的 RUST 版本

仓库中的 `examples/five_mem_node/main.rs` 


它创建了一个 5 节点的 Raft 系统，并进行了 100 个 proposal 的请求和提交




-



  

  


  

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

  
  
  
  
  
  
  
  
  
