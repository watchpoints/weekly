---
title: 成为ob贡献者(07):重新理解OB模块设计
date: 2024-10-19
description: 自我驱动学习
draft: false
categories:
  - oceanbase
---


记录是为了更好的思考 为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。
本文分为两个部分

1. 捕捉想法:  
  无数个问题同时出现，让思路更加混乱，但是描述不出来具体是什么问题。
  因此 集中精力 阅读  PALF: Replicated Write-Ahead Logging for Distributed Databases 开始。

2. 动手验证
  > 任务1：翻译：PALF: Replicated Write-Ahead Logging for Distributed Databases
  > 任务2：做练习题：#Paxos 试题 看看你能得多少分
  



```
你是一位计算机专业翻译专家，掌握丰富的计算知识并且转化小白理解的语言，我希望你能帮我将以下英文视频的中文翻译结果重新意译和润色。

规则：
- 这些字幕包含计算机等专业知识相关，注意翻译时术语的准确性
- 保留特定的英文术语、数字或名字，并在其前后加上空格
- 基于直译结果重新意译，意译时务必对照原始英文，不要添加也不要遗漏内容，并以让翻译结果通俗易懂，符合中文表达习惯

英文原文：
{ 英文原文 }

直译结果：
{ 第一直译的结果 }

重新意译和润色：
{ 润色的结果 }

语法分析：
{ }

结合专业知识 给出自己理解 ### 整体理解

```

## TODO1: 翻译PALF（个人理解，并非翻译全文）


2.2 Redesigned Architecture  重新设计架构设计


To address these challenges, the internal architecture of version
为了应对这些挑战，版本的内部架构
4.0 of the OceanBase database was redesigned [47]. A new component, Stream, has been proposed, which consists of several data
重新设计了OceanBase数据库的4.0版本[47]。已经提出了一个新的组件Stream，它由几个数据组成
partitions, a replicated write-ahead logging system, and a transaction engine
分区、复制的预写日志系统和事务引擎


The key insight of the Stream is that tables in a database
Stream的关键见解是数据库中的表
are still partitioned, but the basic unit of transaction and logging is
仍然是分区的，但事务和日志记录的基本单元是
a set of partitions in a Stream, rather than a single partition
流中的一组分区，而不是单个分区



The key insight of the Stream is that tables in a database
Stream的关键见解是数据库中的表
are still partitioned, but the basic unit of transaction and logging is
仍然是分区的，但事务和日志记录的基本单元是
a set of partitions in a Stream, rather than a single partition.
流中的一组分区，而不是单个分区。


A table partition simply represents a piece of data stored in the storage engine. 
分区只存储数据

The transaction engine generates redo logs for recording
modifications of multiple partitions within a Stream and stores logs
in the WAL of the Stream.

多个分区修改存储在一个日志流，并且用redo logs 形式

![[Pasted image 20241201162733.png]]

Multiple replicas of a Stream are created on different servers.

流的多个副本 分布在不同的服务器


Only one of them will be elected as the leader and serve data writing requests.
他们中只有一人会当选为领导人

The number of replication groups

in a cluster can be reduced to the number of servers to eliminate

the overhead incurred by massive replication groups.

集群中的服务器数量可以减少到消除

并处理数据写入请求。复制组的数量
大量复制组产生的开销。



## TODO2:Paxos 试题，看看你能得多少分



## 参考

一天一行，一周一段，一月一篇

### [1] Oceanbase PaxosStore 源码阅读

https://zhuanlan.zhihu.com/p/395197545

开源数据库OceanBase代码导读（13）


-  https://github.com/oceanbase/oceanbase-doc
- [OBCP V3.0 培训教材](https://www.oceanbase.com/training/obcp)
- https://console.oceanbase.com/instances
- [ OceanBase数据库实践入门——了解总控服务](https://mp.weixin.qq.com/s?__biz=MzU3OTc2MDQxNg==&mid=2247483885&idx=1&sn=52bf5a7c8cbea04671f2327455fd16da&chksm=fd6079a2ca17f0b456055a20d3b17ba2125704a2fb4a81d08d375c1ea69f2d4ce93c5f8fe04f&scene=21#wechat_redirect)


微信：
- https://mp.weixin.qq.com/s/L0uBV8s5gXagiylz4vYyOg # OceanBase 独立部署高级玩法三：副本数调整
-  OceanBase 独立部署高级玩法二 ：2C8G版
- OceanBase 源码解读（十四）：集群位置信息的存储与汇报
https://open.oceanbase.com/blog/10900275
-  OceanBase 数据库 V4.0 版本中有关 bootstrap 问题的排查
 https://www.oceanbase.com/knowledge-base/oceanbase-database-1000000000781056






记录是为了更好的思考



http://localhost:1313/post/oceanbase/2024/07_make_ob_leader/


### 沟通步骤

1. 准备一个图片，一段话，可以放在三页doc，三页ppt 描述你方案

   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比


2. 反馈：鸡蛋里挑骨头，经过客户，领导 ，公司认证，做事情价值和意义。听到否定愤怒反对 说明自己对这个事情还不了解。

3. 最后融合别人建议，然后完善你项目



https://zhuanlan.zhihu.com/p/268558189