---
title: "成为ob贡献者(07):leader"
date: 2024-10-19
description: "自我驱动学习"
draft: false
categories: ["oceanbase"]
---





为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。
本文分为两个部分

1. 捕捉想法:  一个节点故障后，假如上面有1万个分区，会经过1万次选举吗？

   > 根据第一感受，无论是什么，都是真实发生的，都是捕捉来源。

2. 动手验证:

  > 搜集已知 书籍，文章，代码  推理分析深理解





### 1.  分区选举 与RootServer选举有什么区别？



小王提问：




三个表表示他们之间关系



数据划分 相关概念
- tablet：是Bigtable中数据分片和负载均衡的基本单位
	Bigtable将大表划分为大小在100～200MB的子表（tablet）​
	BIgtable维护的是tablet到tablet server的映射关系，
	当需要迁移数据的时候，也是与tablet为单位。

- 分区：

  ​       OB 把普通的表的数据按照一定的规则划分到不同的区块内，同一区块的数据物理上存储在一起。

  ​       这种划分区块的表叫做分区表，其中的每一个区块称作分区。

  ​    也就是说明 大表拆分为更多更小的结构相同的独立对象，即分区。

  ​       分区策略包括范围（Range）分区、列表（List）分区和哈希（Hash）分区。

  ​       tablet_size 用于设置分区内部并行处理时每个分片的大小，通过文字推断出 一个分区包括多个分片

- 副本

  OceanBase 数据库会把同一个分区的数据拷贝到多个机器。不同机器同一个分区的数据拷贝称为副本（ Replica ）

  - 主分区

  ​    每个分区和它的副本构成一个独立的 Paxos 组，其中一个分区为主分区（ Leader ），其它分区为备分区（ Follower ）

-  RootService

​         根据租户 Primary Zone 等因素，均衡各机器分区 Leader 数目。

















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



