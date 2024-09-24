---
title: "成为oceanbase贡献者第四天：内置函数"
date: 2023-05-26
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---



为了整理思路，文章采用模拟2人对话方式，如有误，欢迎留言。



### Basic-Paxos协议回顾



一天下午，小王同学正在津津有味回顾：<<成为OB贡献者第四天：从单点到多节点 i++并发方案 >> 

![img](https://cdn.nlark.com/yuque/0/2024/png/215144/1725369592375-51181190-3136-49b7-ac9f-983d375c6189.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_800%2Climit_0) 

通过假设一个存储服务，2个客户端，3个存储节点 存储一个值方式 解决并发问题 引出 Basic Paxos

任何成员都可以提出议案，然后通过2轮 RPC 来确定只一个值。（这里没考虑持久化）



关于：任何成员都可以提出议案 然后写下自己理解

- Basic Paxos只负责批准一个提案，一旦批准之后，其他 proposer 再怎么重复的提交 更高的提案编号（proposal number ），都只会**学习到已经达成**一致的 提案(acceptedValue)，**然后重复提交**,， 

- **学习这个表示很准确，如何学习呢？**

Basic Paxos 是通过2次RPC确定一个值（假设的系统没考虑持久化）

在 phase 2a: Accept

如果 promise 里含有 (acceptedProposal, acceptedValue)，那么就**放弃自己原本的提案**，从返回的这些 promise 里挑出 **acceptedProposal 最大的 acceptedValue 作为本次的提案 value**。



> 注意：这里说2轮RPC确定一个值，并不是说没有其他网络交互 ，
>
> 参考7：[Paxos三部曲之二] 使用Multi-Paxos协议的日志同步与恢复
>
> 参考8：[Paxos三部曲之一] 使用Basic-Paxos协议的日志同步与恢复
>
> Basic-Paxos进行redolog同步的延迟包括了3次网络交互加2次写本地磁盘.



Multi-Paxos 协议将  Basic-Paxos3次网络交互（1.产生logID；2. Prepare阶段；3. Accept阶段），

简化为1次网络交互（accept阶段），省去的（1.产生logID；2. Prepare阶段）让leader代替，

> leader提案时候省略准备（Prepare ）阶段提案编号获取过半同意，
>
> 直接进入接受阶段 写入 <提案编号，提案值> 
>
> 根据Base Paxos 接受阶段 也可能失败情况。 



![image.png](https://cdn.nlark.com/yuque/0/2024/png/215144/1725762817262-2be26a50-6bac-4329-bc5d-4d5217068c74.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_800%2Climit_0) 

这样leader可以一次提交多次提案，大大提升了效率。这样引发其他问题，

- 一次提交多个 可能失败情况 ，在索引位置出现 空日志情况 提示线索：Paxos 幽灵复现问题 

- 需要明确的是Multi-Paxos协议并不假设全局必须只能有唯一的leader来生成日志，

  它允许有多个“自认为是leader的server”来并发生成日志，这样的场景即退化为Basic-Paxo



这是raft本质区别,因为没看相应代码 这里不做注释， todo 当作遗留作业下次研究。



回顾小结：

![img](https://cdn.nlark.com/yuque/0/2024/png/215144/1725372429223-d0422887-ef60-42e2-bf7d-871b10d36e5f.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_800%2Climit_0) 





### 1



叮叮叮，群里 发一个消息，

中断 小王思考 一个，多个Leader一次提交多次提案引发新问题。（只能中断当前事情，处理紧急事情）





















### 参考

[1]  partial resynchronization

  https://redis-doc-test.readthedocs.io/en/latest/topics/replication/

[2] Consistency under partitions

https://cndoc.github.io/redis-doc-cn/cn/topics/sentinel.html

https://redis.io/docs/latest/operate/oss_and_stack/management/sentinel/

[3] [BUG]sentinel failed to failover while master was down

 https://github.com/redis/redis/issues/13239

https://github.com/redis/redis/issues/12570

https://github.com/redis/redis/issues/11719

https://github.com/redis/redis/issues/12184

[4] Database · 原理介绍 · 关于Paxos 幽灵复现问题

  http://mysql.taobao.org/monthly/2018/11/03/

[5] 使用Basic-Paxos协议的日志同步与恢复

http://oceanbase.org.cn/archives/90

[6]

https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000001050885

https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000001053050



【7】[Paxos三部曲之二] 使用Multi-Paxos协议的日志同步与恢复

  http://oceanbase.org.cn/archives/111

http://mysql.taobao.org/monthly/2018/11/03/



【8】 参考8：[Paxos三部曲之一] 使用Basic-Paxos协议的日志同步与恢复 

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



