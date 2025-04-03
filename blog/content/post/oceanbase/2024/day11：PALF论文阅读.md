---
title: 成为ob贡献者(10)
date: 2024-12-21
description: 自我驱动学习
draft: false
categories:
  - oceanbase
---




为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。
本文分为两个部分

1. 捕捉想法: 
     通过别人分享 我了解到，在写数据库 之前，别人是看论文搞清楚基本原理才这个事实。
     这样事情我能不模仿，
     首先遇到的第一问题，你又不是研究生学历不够，没有相关氛围熏陶，自我否定！
     第二个事情是学10几年英语 还是哑巴英语 根本没信心 ！
     第三个事情 哪有时间呀 ，看完了有什么价值呢，无法开始行动！
2. 动手验证：
    第一步：资料是公开的，你可以不可以下载？ 1分钟完成。
    第二步：不懂核心思想，能不能一句一句翻译？可以。
    第三步：制定计划一共多少章节，需要多少天完成，精确到每天至少看多句子。
           从单词到句子到文章拆分。
3.最小行动：每天拿出 2分钟看一个句子 ，完成任务就下班收工。多一句别看。

    



# 第5章节：数据变更同步


![输入图片说明](https://foruda.gitee.com/images/1739357371842613036/cc118291_116887.png "屏幕截图")
原文：

Besides serving transactions, distributed databases also act as the  
source of data flow. Downstream applications can be deployed to  
provide various services by synchronizing data changes recorded  
in physical logs. 

This section introduces two typical physical log  
synchronization scenarios in OceanBase, describes what challenges  
they bring to PALF, and depicts how to address these challenges by  
utilizing features of PALF.


**翻译：**

除了处理事务，分布式数据库还充当数据流的源头。下游应用可以通过同步物理日志中记录的数据更改来部署，从而提供各种服务。

本节介绍了OceanBase中两种典型的物理日志同步场景，描述了它们给PALF带来的挑战，并阐述了如何利用PALF的特性来解决这些挑战。

**通俗理解：**

分布式数据库不仅仅是处理事务（如存取数据）的地方，它们还扮演着数据流的起点。下游系统（指其他依赖这些数据的应用或服务）可以通过获取数据库中记录的数据变化（比如用户修改了什么信息），来实现各种功能，比如数据分析、实时报告等。

在OceanBase数据库中，有两种常见的方式来同步这些数据变化。本节主要讲解这两种方式，以及它们给数据库的高可用性（PALF）带来了哪些问题，最后介绍如何利用PALF的特性来解决这些问题。

简单来说，数据库不仅存储和处理数据，还能把数据的变动告诉其他系统，这样其他系统就能根据最新的数据做相应的处理。而OceanBase在这方面有两种常用的方法，但这些方法也带来了一些挑战，文中会讲解应该如何应对这些挑战。


# 5.1 第一段

原文：
When clients write data to databases, records of modifications are  
appended to the leader of the PALF group and replicated to followers. 

Besides replicating logs within the database, data changes can  
be synchronized out of the database for richer functions. 

There are  two typical scenarios in the OceanBase database: 
physical standby  databases and database restore

翻译：

当客户端向数据库写入数据时，修改记录会被追加到PALF组的主节点，并复制到从节点。在数据库内部复制日志的同时，数据变化也可以被同步到数据库外部，以实现更丰富的功能。在OceanBase数据库中，主要有两种典型场景：物理备用数据库和数据库恢复

**通俗理解**：

想象一下，你在银行存钱，每次存钱都会记录一条转账记录。银行的系统会把这条记录同时保存到主要的记录本（主节点）和几个副本记录本（从节点），这样即使主要记录本坏了，副本记录本还能保证数据不丢失。

除了在银行内部保存这些记录，银行还可以把这些记录送到其他地方，比如给审计部门或者数据分析部门，用于更广泛的用途。在OceanBase数据库中，这种技术有两种主要用途：

1. **物理备用数据库**：就像银行有多个分行各自保存一份记录副本，数据库也可以有多个物理备用数据库，确保在主数据库出问题时，备用数据库能立即接管，保证服务不中断。
    
2. **数据库恢复**：如果数据库因为意外情况（比如系统故障）导致数据丢失，可以通过之前保存的记录快速恢复数据，确保数据安全。



# 第5.1章节 第二段 

## **1. 原文：**

As shown in Figure 7, the physical standby database is an【independent】 database in which the data are identical to the primary database. 

It could serve part of read requests to relieve pressure on the primary database. 

Compared to traditional primary-backup architecture, 
it offers higher availability because each database clus-ter can tolerate failures. 

One of the most important features of the physical standby database is Database-Level Data Protection and Disaster Recovery, a physical standby database can be switched to be the primary database by a failover operation if the original primary database becomes unavailable, 

which distinguish it from replica-level protection such as Paxos learners [7].‘

In production databases, database restore is a core component of the high-reliability feature.

If data have been lost due to storage media damage or human errors,archived logs stored in offline storage (such as NFS or Cloud ObjectStores) could be used to restore an identical database


## 2 核心论点：

- 物理备库相当于主数据库的一个“影子副本”，它可以用来处理部分查询请求，减少主数据库的压力。
- 传统的主-备架构中，主库故障可能导致整个系统不可用，而物理备库可以随时“顶上”，确保业务不中断。
- 物理备库比 Paxos 复制更强，它不仅存储数据副本，还能直接切换为主库运行。
- 在真实业务场景中，如果数据库出现故障，备份日志（存储在云端或 NAS）可以帮助恢复数据，避免灾难性数据丢失。

**简单来说，物理备库不仅是数据的备份，还能“接管”主数据库的工作，确保系统稳定运行！** 🚀

## 3. *支持论据（翻译句子）*

- **物理备库（physical standby database）** 是一个独立的数据库，其数据与主数据库（primary database）相同。它可以处理部分读取请求，减轻主数据库的负担。
- **与传统的主-备架构相比**，物理备库提供了更高的可用性，因为数据库集群可以容忍故障。
- **物理备库的核心特性** 是数据库级别的数据保护和灾难恢复（disaster recovery）。如果主数据库不可用，物理备库可以通过故障转移（failover）操作变为主数据库。
- **这一点区别于** 复制级别的保护（如 Paxos 学习者），因为 Paxos 学习者只是复制数据，而物理备库可以真正接管主数据库的角色。
- **在生产环境中**，数据库恢复（restore）是高可靠性的关键部分。如果由于存储介质损坏或人为错误导致数据丢失，存储在离线存储（如 NFS 或云对象存储）的归档日志（archived logs）可以用于恢复数据库。

---

## 4. 重点单词与语法
- Figure 示意图，架构图
-  physical standby database —— 物理备用数据库
- **independent** —— 独立的
  这里指物理备用数据库是一个独立的数据库，而不仅仅是数据副本。
  
- **distinguish A from B**：使 A 有别于 B
- 这里指物理备用数据库与 Paxos 复制的不同之处在于，它可以成为主数据库，而 Paxos 仅仅是复制数据

**archived logs** —— 归档日志
- **archived**（已归档的）+ **logs**（日志）
- 这些日志用于在数据丢失后恢复数据库。



#### 第三段


原文：

The basic idea behind physical standby databases or database restore is similar: synchronizing data changes recorded in physical logs from the primary database or external storage to the standby database or the restoring database.  
物理备用数据库或数据库恢复的基本原理相似：通过同步主数据库或外部存储中记录的物理日志数据变更，以保持备用数据库或恢复中数据库的数据一致性。


For the OceanBase database, one of the challenges in implementing these features is synchronizing logs from one PALF group (or external storage) to another PALF group.  
对于 OceanBase 数据库，实现这些功能的一大难点在于：需要将日志从一个 PALF 组（或外部存储）同步到另一个 PALF 组。

In addition, these PALF groups should be independently available.  
此外，这些 PALF 组应具备独立可用性，以确保系统的高可靠性和稳定性。

A naïve solution is to read log entries from the PALF group in the primary database and append the logs to the corresponding PALF group in the standby database, as shown in Fig. 3.  
一种较为直接的方案是从主数据库的 PALF 组中读取日志条目，并将其追加到备用数据库中对应的 PALF 组中，如图 3 所示。

However, the consensus protocol will attach a log header to the appended record for replication, which results in inconsistency between the log format of the primary database and that of the standby database.  
然而，共识协议会在追加的日志记录上附加日志头以用于数据复制，这会导致主数据库和备用数据库的日志格式不一致。


小王提问：

特性：备库同步主库数据，采用不是Paxos learners 方式。
       采用讲一个PALF group 日志复制到另外一个PALF group
       遇到的问题是 日志格式 不一致？这个不理解，从哪里看到解释

老王：目前回答不了？
       
# 5.2 PALF Group Mirror    

## 段落1 

We abstracted the requirements of synchronizing data changes across PALF groups as a primitive: the PALF group mirror, which is an independent PALF group that performs the same consensus protocol as described in §3.4.
我们将跨 PALF 组同步数据变更的需求抽象为一种原语：PALF 组镜像，它是一个独立的 PALF 组，执行与 §3.4 中描述的相同的共识协议。

It 【maintains】 a mirror of the data change prefixes, which are stored in the primary PALF group or external storage.
它维护着存储在主 PALF 组或外部存储中的数据变更前缀的镜像。

【The PALF group mirror】 can be reconfigured independently and be switched to a primary group as needed.
PALF 组镜像可以独立地进行重新配置，并根据需要切换为主组。




小王提问：data change prefixes 这个属于什么操作？

老王回答：
**举个例子：**
- 想象你在一本笔记本上记账：
    - 第一次记账：`[001] 小明 -100 元`
    - 第二次记账：`[002] 小红 +200 元`
    - 第三次记账：`[003] 小明 +50 元`
- 这里的 `[001]`、`[002]`、`[003]` 就是 **data change prefixes**。它们表示每次数据变更的顺序和标识。


### 段落2 

One of the most important differences between the primary PALF group and PALF group mirrors is the pattern of writing log records. 主 PALF 组与 PALF 组镜像之间最重要的区别之一在于日志记录的写入模式。

In primary PALF group, a log record is appended to the PALF group, attached with a log header, and replicated to replicas by the consensus protocol. 在主 PALF 组中，日志记录会被追加到 PALF 组中，并附加上日志头，然后通过共识协议复制到副本上。

As for the PALF group mirror, it only accepts logs committed by a primary PALF group. 而对于 PALF 组镜像，它只接受由主 PALF 组提交的日志。

When a committed log is mirrored to the leader, some fields of the log header (e.g., ProposalID) will be replaced with the leader’s own values. 当已提交的日志被镜像到领导者（leader）时，日志头中的某些字段（例如 ProposalID）会被替换为领导者自身的值。

The leader reuses the LSN and CSN of the original log entries, stores logs to the LSN, and replicates logs to followers. 领导者会重用原始日志条目的 LSN 和 CSN，将日志存储到对应的 LSN，并将日志复制给跟随者（followers）

## 参考资料
- OceanBase4.0跟我学--分布式到底可靠不可靠，到底丢不丢数--终于学完了
- https://wnso49yyot.feishu.cn/file/ElgzbJ196o2FKlxF6aPcQN4mnId


# 提示词


请双语翻译，满足 信达雅原则 并检查3遍， 一句英文，一句中文 方面对照阅读学习英语方式输出



	 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

 二、这个技术的优势和劣势分别是什么 


 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

 四、技术的组成部分和关键点。

 五、技术的底层原理和关键实现

 六、已有的实现和它之间的对比
 


### **PALF 论文阅读计划（每天 2 分钟）**

**目标：**

- **每天** 阅读一句话（理解关键点）。
- **每周** 至少阅读一个段落（加深理解）。
- **每月** 至少阅读一个章节（完成论文）。
- **共 10 章节，预计 3 个月完成**。

---

## **第一个月：掌握论文背景与核心概念**

### **第 1 周：论文概述与研究背景**

📌 **目标：理解论文的研究问题和动机**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 1**|论文标题、作者、摘要第一句|了解论文主题|
|**Day 2**|摘要第二句|PALF 研究的核心问题|
|**Day 3**|摘要第三句|PALF 主要贡献|
|**Day 4**|摘要第四句|论文的实验结果概览|
|**Day 5**|摘要第五句|PALF 如何改进现有方法|
|**Day 6**|论文引言第一句|研究动机|
|**Day 7**|**阅读完整引言段落**|论文的背景与问题定义|

📖 **第一周阅读成果：** 掌握 PALF 研究动机、贡献和论文总体结构。

---

### **第 2 周：相关工作与 WAL 背景**

📌 **目标：理解 WAL 在分布式系统中的作用**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 8**|相关工作第一句|现有 WAL 复制方案|
|**Day 9**|相关工作第二句|现有方法的局限性|
|**Day 10**|相关工作第三句|为什么需要 PALF？|
|**Day 11**|PALF 目标第一句|PALF 设计原则|
|**Day 12**|PALF 目标第二句|PALF 如何优化 WAL 复制|
|**Day 13**|PALF 目标第三句|PALF 设计的核心创新|
|**Day 14**|**阅读完整相关工作段落**|现有 WAL 方法的不足|

📖 **第二周阅读成果：** 了解 PALF 相比现有 WAL 方法的优势。

---

### **第 3 周：PALF 系统架构**

📌 **目标：理解 PALF 如何设计日志复制系统**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 15**|PALF 架构第一句|PALF 组件介绍|
|**Day 16**|PALF 架构第二句|日志管理模块|
|**Day 17**|PALF 架构第三句|日志存储策略|
|**Day 18**|PALF 架构第四句|日志复制机制|
|**Day 19**|PALF 架构第五句|数据一致性保障|
|**Day 20**|**阅读完整 PALF 架构段落**|了解 PALF 关键组件|

📖 **第三周阅读成果：** 掌握 PALF 的架构与核心机制。

---

### **第 4 周：PALF 日志复制与一致性**

📌 **目标：理解 PALF 如何确保日志复制的高效性**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 21**|PALF 复制流程第一句|复制数据的触发点|
|**Day 22**|PALF 复制流程第二句|日志传输优化|
|**Day 23**|PALF 复制流程第三句|事务提交逻辑|
|**Day 24**|PALF 复制流程第四句|复制协议的细节|
|**Day 25**|PALF 复制流程第五句|网络故障处理策略|
|**Day 26**|**阅读完整 PALF 复制流程段落**|理解日志复制细节|
|**Day 27 - 30**|**总结第 1 章**|回顾第 1 章要点|

📖 **第四周阅读成果：** 掌握 PALF 复制流程的关键点。

---

## **第二个月：PALF 的优化与实验分析**

### **第 5 周：故障恢复机制**

📌 **目标：掌握 PALF 如何在崩溃后快速恢复**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 31 - 37**|逐步阅读 PALF 故障恢复章节|事务回滚、数据一致性保障|

📖 **成果：** 理解 PALF 如何确保数据库崩溃后可恢复。

---

### **第 6 周：性能优化**

📌 **目标：理解 PALF 如何提高吞吐量和降低延迟**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 38 - 44**|逐步阅读 PALF 优化策略|WAL 压缩、存储优化|

📖 **成果：** 了解 PALF 如何优化数据库性能。

---

### **第 7 周：实验设计与评估**

📌 **目标：理解 PALF 的实验数据和结论**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 45 - 51**|逐步阅读 PALF 实验部分|实验设置、对比方法、数据分析|

📖 **成果：** 掌握 PALF 真实场景下的性能表现。

---

### **第 8 周：论文总结与未来工作**

📌 **目标：回顾 PALF 的贡献和可能改进方向**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 52 - 58**|逐步阅读论文总结|论文的主要贡献、局限性、未来研究方向|

📖 **成果：** 总结 PALF 的整体价值，并思考未来改进方向。

---

## **第三个月：复习与应用**

### **第 9 周 - 10 周：总结与深入思考**

📌 **目标：完整掌握 PALF 并应用到自己的研究中**

|**天数**|**任务**|**备注**|
|---|---|---|
|**Day 59 - 65**|总结论文核心技术|复盘所有章节的内容|
|**Day 66 - 72**|论文应用场景分析|PALF 在分布式数据库中的适用性|
|**Day 73 - 80**|与其他 WAL 方案对比|结合 Raft、Paxos 进行分析|
|**Day 81 - 90**|论文最终总结|整理阅读笔记，写出个人理解|

📖 **最终成果：** **深入掌握 PALF 论文的所有关键点，并能够应用到自己的研究或技术学习中！** 🎯

---

**🎯 计划总结：**  
✅ **每天 2 分钟，坚持 90 天，完整掌握 PALF 论文！**  
✅ **从核心概念 → 关键技术 → 细节分析 → 应用总结，层层深入！**

这个计划适合你吗？要不要调整某些部分？😊