---
title: 成为ob贡献者(07):leader
date: 2024-10-19
description: 自我驱动学习
draft: false
categories:
  - oceanbase
---





为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。
本文分为两个部分

1. 捕捉想法:  一个节点故障后，假如上面有1万个分区，会经过1万次选举吗？
   > 根据第一感受，无论是什么，都是真实发生的，都是捕捉来源,都可以大胆猜想
2. 动手验证
  > 搜集已知 书籍，文章，代码  推理分析小心求证。

### 1. 捕捉想法: 一个节点故障后，假如上面有1万个分区，会经过1万次选举吗？

**小王提问1：**

看了 # <<成为ob贡献者(6):如何阅读Paxos代码>> 上面提到 数据副本是以分区为单位，每个分区的主副本所在服务器被称为 Leader，我就不理解了

根据强一致性协议，假如3个节点，一个节点故障了，会选举另外一个节点当作领导，三个节点同一 时刻，只有一个leader ，这个很容易理解，
但是引入分区后，数据会分布在多个节点， 这样 每个节点都有主副本，
假如一个节点故障了，上面可能有很多主副本，每个分区都要做选举吗？

假如上面有1万个分区，最坏情况会经过1万次选举吗？

>1.  简化问题：primary zone='z1,z2,z3"  leader副本的分布
> 帮忙查看一下集群部署，主副本是怎么在observer节点分布的？
> https://ask.oceanbase.com/t/topic/35614045
> 
> 2. RS（Root Service）高可用 和副本高可用 是同一个事情吗？
    https://ask.oceanbase.com/t/topic/35614056
    3.  [关于OceanBase RootService切主的一些疑问](https://ask.oceanbase.com/t/topic/35607427) 也提出类似问题，
     是指RS模块不会主动切主，切主是后台选举模块根据多种优先级综合决定的，宕机会触发切主，但不是RS发起的切主，RS只是把server的status改掉了



老王回答：

成为ob贡献者(6) 对一体化定义为多个节点像一个节点一样。
- 单节点： 一个网卡故障怎么处理，采用多个网卡绑定一个虚拟网卡，一个网卡故障，不影响虚拟网卡正常使用
- 同样道理一个节点故障怎么处理，多个节点组成一个虚拟节点个 。
  如 图片1：系统架构中  显示9个节点，把三个zone看成三个节点虚拟，一个虚拟节点 可能1个 物理节点组成，也可能三个，可能更多。
  - 一个节点故障了，整个虚拟节点 是可用的，
  **注意不是100%可用**，故障的数据无法访问。因此采用副本方式 三个虚拟节点组成一个集群。


**数据与元数据关系**


-  一个zone 包含完整分区，自然分摊得到每个节点只存储一部分。
- 需要维护一份完整分区与节点之间关系，俗称元数据。
   图片2： RootServer 管理整个元数据信息，RootServer自身也保证高可用.
   
思考这样一个问题：在单机数据库中，数据库中的表都存储在本地，但在 OceanBase 这样的分布式数据库中，一张实体表可能存在多个分区，每个分区又有多个副本，分散在集群中的多个 server 上。那么当用户想去查询一张实体表时，数据库内部如何在众多 server 上定位这张表呢？

OceanBase 依靠内部的一套系统表来实现实体表的位置发现过程，这套系统表我们一般统称为 meta table

![图片:# 系统架构]( https://obcommunityprod.oss-cn-shanghai.aliyuncs.com/prod/blog/2024-09/1727255056742.png)
> 图片1:系统架构


![[Pasted image 20241119213144.png]]

>图片2 元数据与数据关系 来源 大规模分布式存储系统：原理解析与架构实战


**表，分区 ，tablet关系**

数据划分 相关概念
- tablet：是Bigtable中数据分片和负载均衡的基本单位
	Bigtable将大表划分为大小在100～200MB的子表（tablet）​
	BIgtable维护的是tablet到tablet server的映射关系，
	当需要迁移数据的时候，也是与tablet为单位。

- 分区：
  ​       OB 把普通的表的数据按照一定的规则划分到不同的区块内，同一区块的数据物理上存储在一起。
  ​       这种划分区块的表叫做分区表，其中的每一个区块称作分区。
  ​    也就是说明 大表拆分为更多更小的结构相同的独立对象，即分区。
  ​       tablet_size 用于设置分区内部并行处理时每个分片的大小，通过文字推断出 一个分区包括多个分片

- 副本

  OceanBase 数据库会把同一个分区的数据拷贝到多个机器。不同机器同一个分区的数据拷贝称为副本（ Replica ）

  - 主分区

  ​    每个分区和它的副本构成一个独立的 Paxos 组，其中一个分区为主分区（ Leader ），其它分区为备分区（ Follower ）

-  RootService

​         根据租户 Primary Zone 等因素，均衡各机器分区 Leader 数目。


>小王提问2: 不要说这么多名词听着糊涂，Ob对外表现为一个进程，内部却有不同功能划分，
RootService以单节点为单位，存储整个表元数据信息，单节点保持高可用传统HA机制，也可用分布式协议，
OBserver分区为单位，每个节点存储部分分区数据，整个虚拟节点 存储完整分区数据，为保障高可用采取分布式协议
一般根据选举选举到谁谁就是主分区，但是ob控制选举指定虚拟节点zone，还有每个节点上分区 可能全部是leader，我了解，不过有个问题 RootService虽然提供高可用，但是单点呀？承担全部流量。单点不存在瓶颈吗？


老王回答：

大规模分布式存储系统：原理解析与架构实战8.4.5章节解释

从容量，网络，磁盘分析单机下读写性能

![[Pasted image 20241122215223.png]]

![[Pasted image 20241122215202.png]]




 
## 选举算法

> 老王：请停止在 上面细节不停的思考，至少明白了，要把一个obsever进程 看成三个进程，
  一个是：查询进程，这个无状态，挂了拉起来继续查询
  一个是：存储进程，通过分区副本保证一致性，通过自发选举 ，选举结果 这些信息通过心跳方式上报元数据管理模块
  一个是：元数据管理进程，维护存储进程选举结果，一个集群只有一个`rootservice`服务

因此 选举算法单独抽象出来 可用在分区，也可用在rootservice.

如下：

更多阅读：
[ OceanBase数据库实践入门——了解总控服务](https://mp.weixin.qq.com/s?__biz=MzU3OTc2MDQxNg==&mid=2247483885&idx=1&sn=52bf5a7c8cbea04671f2327455fd16da&chksm=fd6079a2ca17f0b456055a20d3b17ba2125704a2fb4a81d08d375c1ea69f2d4ce93c5f8fe04f&scene=21#wechat_redirect)
集群启动过程：

目前`rootservice`功能包括：

- 集群自举(初始化)
   `bootstrap`操作只需要做一次，一个集群只有一个`rootservice`服务。其他节点在启动时指定`rootservice_list`后，在集群`sys`租户内用一个命令将新节点注册到集群中
   
   其中 `-r`参数就是指定了`rootservice_list`地址。第一次搭建的时候，需要用命令初始化该服务

- 指导分区`leader`选举。
  分区的选举由选举模块负责，`rootservice`只是根据一些规则(如`primary_zone`或`locality`)影响分区的`leader`选举








## 参考
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



