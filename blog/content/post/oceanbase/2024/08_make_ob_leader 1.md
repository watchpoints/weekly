---
title: "成为ob贡献者(08):PALF: Replicated Write-Ahead Logging for Distributed Databases"
date: 2024-10-19
description: 自我驱动学习
draft: false
categories:
  - oceanbase
---
问题描述：
问题描述：

Mds 通过 dump indo查看 文件 in->snaprealm 存在

通过 ll_get_inode() 查询返回in->snaprealm 不存在。

这个什么情况下发生，现场环境 3万个文件查询，出现8个snaprealm 不存在情况

记录是为了更好的思考 为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。
本文分为两个部分

1. 捕捉想法:  
  无数个问题同时出现，让思路更加混乱，但是描述不出来具体是什么问题。
  因此 集中精力 阅读  PALF: Replicated Write-Ahead Logging for Distributed Databases 开始。

2. 动手验证
  > 任务1：翻译：PALF: Replicated Write-Ahead Logging for Distributed Databases
  > 
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

结合专业知识 给出自己理解 
整体理解:
为了掌握可以给出三个题目进行练习？

```

## TODO1: 翻译PALF（个人理解，并非翻译全文）


### 2.2 Redesigned Architecture  重新设计架构设计


OB4.0调整架构目的是什么 ，大公司大集群模式 不适用于中小公司。
注释：最低8G内存就是一个例证
      "As to" 是一个英语短语，通常用于引入一个特定话题或问题，相当于“关于”、“至于”或“就……而言”。它可以用来引导一个句子或从句
we found that the previous architecture is not as well suited to medium and small enterprises as to large-scale clusters  of large companies



每个节点上有上万个分区，这些分区采用Paxos保证数据一致性，
分区日志的复制占用大量资源。
注释：  Paxos通过 2次RPC ，让多节点写入一个值达成一致
        上万个分区，上万次选举，日志复制 来达成一致 
         这是上篇文章:成为ob贡献者(07):重新理解OB模块设计疑问 在这里找到答案.
One of the problems is the overhead of log  
replication. OceanBase enables users to create tens of thousands  
of partitions in each server. 
This number of Paxos groups consume  
significant resources for no real purpose, therefore raising the bar  
for deployments and operations


同样分区造成的事务问题
一个这样的事务可能跨越数万个分区，这意味着在两阶段提交协议中有数万个参与者，
这将系统不稳定并牺牲性能。

Another challenge is the huge  
transaction problem. One such transaction probably spans tens of  
thousands of partitions, which means that there are tens of thousands of participants in the two-phase commit protocol, which will  
destabilize the system and sacrifice performance


为了解决这些问题，OceanBase 数据库的 4.0 版本对其内部架构进行了重新设计[47]。
引入了一个名为 Stream 的新组件，该组件包括多个数据分区、一个复制的预写日志系统以及一个事务引擎。

To address these challenges, the internal architecture of version  
4.0 of the OceanBase database was redesigned [47]. 
A new component, Stream, has been proposed, which consists of several data  
partitions, a replicated write-ahead logging system, and a transaction engine


Stream 的核心理念在于，尽管数据库中的表仍然被分区，但事务和日志操作的基本单位变成了 Stream 中的一组分区，而非单个分区。表分区本质上是存储在存储引擎中的一块数据。事务引擎负责生成重做日志，用以记录 Stream 内多个分区的修改，并把这些日志存储在 Stream 的预写日志（WAL）中。

注释：这个和 MySQL中的三种日志（Undo Log、Redo Log和Binlog）有关系关系
      stream 概念 流式大数据处理框架。时序数据概念：
      百科定义：流数据是一组[顺序]大量、快速、连续到达的数据序列,一般情况下，流数据可被视为一个随时间延续而无限增长的动态数据集合。

The key insight of the Stream is that tables in a database  
are still partitioned, but the basic unit of transaction and logging is  
a set of partitions in a Stream, rather than a single partition. A table  
partition simply represents a piece of data stored in the storage  
engine. The transaction engine generates redo logs for recording  
modifications of multiple partitions within a Stream and stores logs  
in the WAL of the Stream.




Multiple replicas of a Stream are created on different servers.

流的多个副本 分布在不同的服务器

Only one of them will be elected as the leader and serve data writing requests.
他们中只有一人会当选为领导人

The number of replication groups in a cluster can be reduced to the number of servers to eliminate the overhead incurred by massive replication groups.
集群中的复制组数量可以减少到与服务器数量相等，以消除由大量复制组带来的额外开销。

旁白：

如下图：P1--P4 组成一个 LS1，P4--P8 组成一个 LS2   ，三个节点调整三个LS.




![[Pasted image 20241201162733.png]]



#### 3.1 DESIGN OF PALF(PAL 的设计)

PALF 的设计宗旨在于打造一个能够复制预写日志的系统，这个系统不仅要能够支持 OceanBase 数据库，还要具备足够的通用性，

正是 PALF 的这一宗旨指导了其设计方向：采用分层架构来平衡数据库的特殊需求和日志系统的通用性。针对数据库的具体需求被抽象成 PALF 的基础原语，并被整合进不同的架构层级中。

注释：能举出更多例子提供类似 PALF 的基础原语接口吗？liunx read write接口算不算，
还有分布式存储提供接口

The design purpose of PALF is to provide a replicated write-ahead  
logging system, which should be capable of serving the OceanBase database and be general enough for building other distributed systems. 

This purpose of PALF drove its design: a hierarchical architecture for balancing particularity of database and generality  
of the logging system. Database-specific requirements have been  
abstracted as PALF primitives and integrated in different layers.

PALF 作为 OceanBase 数据库的复制 WAL 系统，承担着记录所有事务日志并在多个副本之间复制这些日志以保证数据持久性和容错能力的重要职责。PALF 提供的接口是数据库与日志系统交互的桥梁，它们允许事务引擎有效地与 WAL 系统协同工作，执行事务提交和数据恢复等操作。

当然一致性协议必然包含在其中
the implementation of the consensus  
protocol is described in detail

### 3.1 Replicated WAL Model

回顾：像redis，mysql采用WAL（ Write Ahead Log ）保证单机故障恢复，如果多个节点呢，

![[Pasted image 20241208170248.png]]

翻译：

分层设计 1， 应用层，2 事务引擎 和3 PAlF层

 事务引擎 修改内存变更 通过AOF方式追加文件中，通过PALF同步到其他副本节点

如图 1 所示，

- 步骤 2-3  事务可以直接在内存存储引擎中修改数据。
- 步骤 4  生成的日志记录会被追加到 PALF 中
- 步骤 5 在领导者节点上，事务引擎将 PALF 当作本地的日志文件系统来处理，它只关心日志记录是否已经被刷新到磁盘。PALF 的职责是将领导者节点上执行的修改复制到其他跟随者节点上

- 步骤 6 一旦 PALF 提交了日志，领导者节点就会通知事务引擎操作的结果

-  步骤 7-8随后，跟随者节点会重放领导者节点执行过的所有修改


原文：
~~~
In OceanBase database, the replicated logging system is abstracted  as an append-only log file, 

transaction engine interacts with PALF  
much as it interacts with local files.

As depicted in Fig. 1, transactions modify data in the in-memory storage engine directly (step  
2-3). 

Therefore, the upper limit of a transaction’s size is greatly  
expanded and is bounded only by the capacity of the storage engine. Log records are then generated and appended to PALF (step  
4). The transaction engine of the leader treats PALF as a local log  
file system, and it is only concerned with whether log records have  
been flushed. The responsibility of PALF is to replicate modifications performed in the leader to followers (step 5). If a log has been  
committed by PALF, the leader will inform the transaction engine  
of the results (step 6), and followers will replay modifications that  
the leader has performed to itself (step 7-8)
~~~

## 3.2 PALF Architecture

![[Pasted image 20241213084724.png]]

原文：
As depicted in Fig. 2, PALF is a replicated logging system consisting  
of multiple replication groups called PALF groups.

In each PALF  
group, multiple PALF replicas are placed on different servers for  
fault tolerance. 

The transaction engine can append logs to a PALF  
group and read logs from it, just like a normal append-only file

翻译：

如图2所示，PALF是一个分布式复制日志系统，由多个称为PALF组的复制组构成。

每个PALF组中都部署了多个PALF副本在不同的服务器上，以此来提高系统的容错能力。
事务引擎能够向PALF组中追加日志，并且可以从中读取日志，
操作方式类似于处理一个普通的只写入文件。

啥意思：

 transaction engine 通过PALF接口，像写本地日志文件一样，将数据安全复制其他节点。

原文：
PALF consists of three main layers: the interface layer, the PALF  
replicas layer, and the PALF runtime environment. The lower two  
layers take charge of log replication, reconfiguration, and log storage; the upper one provides user interfaces and coordinates the  
states of PALF and the transaction engine.

翻译：
PALF系统由三个核心层次构成：接口层、PALF副本层以及PALF运行环境。
其中，较低的两层负责处理日志复制、配置变更和日志存储等任务；
而最上层则提供用户界面，并负责协调PALF系统与事务引擎之间的状态。

啥意思：核心的配置变更，存储，复制都PALF 隐藏起来了

原文：
For each PALF group, records generated by the transaction engine are first appended to the leader. The log sequencer will assign a monotonically increasing log sequence number (LSN) to  
each record, which uniquely identifies a log entry within the PALF  
group. Records will be encapsulated as log entries and replicated  
to and persisted by other PALF replicas (followers) in the order of  
LSN through the Paxos protocol

翻译：

在PALF group，事务引擎生成的日志记录首先会被发送到领导者节点。
随后，日志序列号器会为每条记录分配一个递增的日志序列号（LSN），这个LSN在PALF组内是唯一的，用于标识特定的日志条目。
这些记录将被封装成日志条目，并通过Paxos协议按照LSN的顺序复制到其他PALF副本（跟随者节点）中，并在那里持久化存储。


啥意思：
1. 描述PALF系统中领导者节点在日志记录处理中的作用。
2. 解释LSN在PALF系统中如何确保日志记录的唯一性和顺序性。
3. 讨论Paxos协议在PALF系统中如何实现日志记录的复制和持久化，并分析其对系统可靠性的影响



翻译：关键模块

> leader election  和  log replication 是分开的 ,非绑定在一起

- 配置管理复制成员变更

- PALF runtime environment 就是RPC


原文：
Unlike some  Paxos variants that bind the leader election and log replication  
together[6, 24, 32, 34], the leader candidate in PALF is elected by an  
independent election module. The reconfiguration module manages  
membership of the PALF group 



原文：
We generalize the interaction between PALF and the transaction  
engine into the interface layer. This isolates the impact of database  
features on PALF and improves PALF’s generality. The log notifier in the leader informs the transaction engine of whether logs  
are committed. The log replayer in followers replays mutations  
recorded in log entries to the transaction engine. If the role of a  
PALF replica has been switched (i.e., leader to follower or follower  
to leader), it will throw a role-changing signal to the role coordinator, the role coordinator forwards signals to transform the role of  
the transaction engine

啥意思：
![[Pasted image 20241213084724.png]]

这段描述了一个分布式系统中 PALF 组件与事务引擎通过接口层进行交互。
接口层有三部分组成

The log notifier：leader节点上日志通知模块告诉事务引擎 append请求执行结果
log replayer: follower回访日志
role coordinator：通知leader发生了转变。


剩余部分是解释这些函数功能


~~~
int append(Record r, CSN refcsn, AppendCb *cb, LSN &lsn, CSN &csn);  
int mirror(LSN lsn, LogEntry l);  

int read(LSN lsn, Record &r);  
//The read method enables random access to log entries by a given  
LSN
//根据LSN随机读位置

int locate(CSN csn, LSN &lsn);  
//A locate method is provided to map the change sequence  
number to a log sequence number.

int monitor_tail(TailCb *cb);  
int monitor_role(RoleCb *cb);  

int trim(LSN lsn);  
//the trim method is designed to indicate useless log entries before the given LSN

class AppendCb  
{ virtual int success() = 0;  
virtual int failure() = 0; }  

class TailCb  
{ virtual int tail(LSN lsn) = 0; }  

When new logs are committed, PALF replicas  
invoke the tail method to notify log consumers of current tail of  
logs

class RoleCb  
{ virtual int to_leader() = 0;  
virtual int to_follower() = 0; }
~~~

### 3.4 Implementation of Consensus



原文：

The Paxos protocol and its variants [6, 24, 26, 27, 32, 34] are widely  
recognized for resolving consensus in distributed systems [5, 9, 14,  
23, 42, 48]. Raft[33] is a typical implementation of Paxos, which  
offers good understandability and builds a solid foundation for  
practical systems. PALF implements Paxos with a strong leader  
approach, it keeps the log replication of Raft for simplicity. Different from Raft, PALF decouples leader election from the consensus  
protocol to manipulate the location of the database leader without  
sacrificing availability. More differences are summarized in §7

翻译


- PALF通过选举leader 强一致性实现Paxos，并保留了Raft的日志复制机制以简化操作。
- 与Raft不同，PALF将领导者选举与共识协议分开

关键单词：

keeps（保留）
it keeps the log replication of Raft for simplicity 

#### Demands for Leader Election

原文：

In distributed databases, the  
location of the leader affects almost all functions

To address the problem, PALF decouples leader election from  
the consensus protocol. Users own the flexibility to specify the  
priorities of replicas elected as the leader

If the  previous leader recovers from failure and its priority is still higher  
than current leader’s, leadership can be automatically transferred  
back to the recovered replica


翻译：

用户可用控制选举的leader节点分布。

帮助理解：
：primary zone='z1,z2,z3"  leader副本的分布
> 帮忙查看一下集群部署，主副本是怎么在observer节点分布的？
> https://ask.oceanbase.com/t/topic/35614045
> 

#### Role Transition（角色转变）

![[Pasted image 20241214153152.png]]


一共有4个状态（比raft多了一个pending follower）

At any given time, each replica is in one of  
four roles: leader, follower, candidate, or pending follower.


疑问：为什么多了pending follower状态
从leader到pending follower状态，


If the leader finds that the candidate is not itself anymore or the  
leader receives messages from a new leader, 
it will revoke its leadership and switch to pending follower. The reason for switching to  
pending follower rather than follower is that the transaction engine  
may have appended some logs to the leader before the leadership  
transfer. To determine the replication results of these pending logs,  
the previous leader must enter the pending follower role and wait  
for logs from the new leader (§4.1). After the status of all pending  
logs is clear, the replica will switch to follower.
、


## TODO2:Paxos 试题，看看你能得多少分



## 参考

https://leehao.me/%E5%9B%BE%E8%A7%A3-Paxos-%E7%AE%97%E6%B3%95/


从做一个难而正确事情开始
第一步 定投10分钟，把事情简化，
一天写一行文字，一周一段，一月一篇文章，
选标题，写大纲，填充内容
第二步：排除无关不相关事情，看短视频，看综艺，看新闻，看博主 一个方法。多少粉丝多少流量
给你你无关。



# OceanBase 源码解读
OceanBase 正式开源后，300 万行核心代码在社区官网同步上线，但这些代码有一些多年变迁的遗迹，新人上手殊为不易。通过本系列的源码解读文章，首先，你可以了解 OceanBase 数据库的基本原理，学到一个数据库是如何实现的。推而广之，你也可以把 OceanBase 的实现原理应用到其他数据库，这对你学习其他数据库也有帮助。其次，在熟悉了 OceanBase 的代码之后，如果有需要，你可以直接在未来的工作中使用我们的代码，或者为 OceanBase 贡献代码。


https://open.oceanbase.com/blog/topics/3983502080

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