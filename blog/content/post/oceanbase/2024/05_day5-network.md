---
title: "成为OB贡献者(5):从一道OBCE面试题再探Paxos"
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

### 从obce面试题开始



叮叮叮，群里 发一个消息，

中断 小王思考 一个/多个Leader一次提交多次提案引发新问题





![](D:\db\daily-interview\blog\content\post\oceanbase\2024\assets\image-20240924192126618.png) 



因为一个/多个Leader一次提交多次提案引发新问题 一时间也完成不了，采取接受方式。



先处理紧急事情。



> obce面试题。
>
> 有套三地五中心的ob集群。a,b,c三个城市，a,b城市分别有两个全功能副本的zone，c城市有一份日志型副本的zone，
>
> 这时候a和b之间的网络断了，会发生脑裂吗，会有什么情况产生。





### 理解问题是什么？



小王是这样理解脑裂 这个概念的

~~~
脑裂 理解成------>2个leader ，为什么2个leader，

网络分区情况下，原来存在的leader，和新选举的leader，

---------> 这不对呀。不是多数派，不会产生2个leader，看采用协议，

Redis 就是异步 这个没有超时判断情况，不知道leader是否失效
参考[1]。这样更容易理解这个问题是什么
~~~



老王：请一句话描述问题是什么 

小王：

> Redis的集群方案哨兵模式 默认异步复制，在网络分区情况下，出现2个leader情况，同时接受客户端写入，导致数据不一致，
>
> Redis是无法处理这样情况的？增加2个参数避免这个情况
>
> \#健康的slave的个数小于N，mater就禁止写入
> min-slaves-to-write 1
> \#延迟小于min-slaves-max-lag秒的slave才认为是健康的slave。
> \#数据复制和同步的延迟不能超过10秒
> min-slaves-max-lag 10
>
> 下一步任务：
>
> 反向假设一下：Paxos 允许多个提议者存在 如何处理这个情况？
>
> ob，目前还无法举例说明，：后面给出答案
>
> 成为OB贡献者第四天：从单点到多节点 i++并发方案  稍微提到解决方式



![image-20240925085507941](D:\db\daily-interview\blog\content\post\oceanbase\2024\assets\image-20240925085507941.png)



**回到Redis3.0 是如何处理这个情况的**



![image-20240925084746478](D:\db\daily-interview\blog\content\post\oceanbase\2024\assets\image-20240925084746478.png)



- 网络分区下的一致性



Redis2.8版开始正式提供名为Sentinel的主从切换方案。



![image-20240925092155740](D:\db\daily-interview\blog\content\post\oceanbase\2024\assets\image-20240925092155740.png) 





假设 3节点组成的存储系统，

T1 时刻 原始状态如下

- Redis instances.   Redis3 是 Leader， Redis2， Redis1 是follower
- Sentinel instances. 每个节点上都哨兵
- Clients. 2个客户端A B



T2 时刻 Redis3 节点发送网络分区，此时Redis1被Sentine提升Leader

- 此时客户端B依然写入 Redis3 ，

- 此时客户端A写入 Redis1



T3:时刻： 网络恢复，Redis3 变成Redis1 的follower，主从复制开始。



>  T1 时刻 客户端B写入数据被覆盖,主从复制这个方式 很暴力 不解决冲突 直接覆盖
>
> 针对这个情况Paxos怎么处理没有写过半情况？？



- 由于 Redis 是异步复制的，因此在这种情况下无法完全防止数据丢失，但是您可以使用以下 Redis 配置选项限制 Redis 3 和 Redis 1 之间的差异：



```
min-slaves-to-write 1
min-slaves-max-lag 10
要求至少有1个slave，数据复制和同步延迟不能超过10秒
如果说一旦所有的slave，数据复制和同步的延迟都超过了10秒钟，那么master就会拒绝接收任
```



- 根据配置 

  Redis 3  在10秒 后不可用。

  RPO（Recovery Point Objective）会10秒数据丢失。



- 来源：33 脑裂：一次奇怪的数据丢失 络一个真实案例：

在使用主从集群时，我曾遇到过这样一个问题：我们的主从集群有1个主库、5个从库和3个哨兵实例，在使用的过程中，我们发现客户端发送的一些数据丢失了，这直接影响到了业务层的数据可靠性。

通过排查才知道，这其实是主从集群中脑裂问题导致的（不是异步同步导致的）





> 旁白
>
> 先不考虑内部实现，从用户角度考虑  ，
>
> Paxos 是如何解决 2个客户端 写入不同Leader情况？
>
> 2个提议者 都提案情况？后面根据这个问题 看代码。



###  理解：三地五中心部署模式



- 物理部署

 一个机柜上 一个交换机 ，下面几个物理机组网



- 逻辑部署

![img](https://cdn.nlark.com/yuque/0/2024/jpeg/215144/1726643590754-0c0e3b6e-0bd7-47e4-94d0-e8a8571dccca.jpeg)





- 全功能型副本具备以下特点：

​      可以构成 Paxos 成员组，并且要求 Paxos 成员组多数派必须为全功能型副本。



| 特性项               | 描述                                   |
| -------------------- | -------------------------------------- |
| 副本名称及缩写       | FULL(F)                                |
| 是否有 Log           | 有，参与投票(SYNC_CLOG)                |
| 是否有 MemTable      | 有(WITH_MEMSTORE)                      |
| 是否有 SSTable       | 有(WITH_SSSTORE)                       |
| 数据安全             | 高                                     |
| 恢复为 Leader 的时间 | 快                                     |
| 资源成本             | 高                                     |
| 服务                 | Leader 提供读写，Follower 可非一致性读 |
| 副本类型转换限制     | 可转换为只读型副本                     |



- 只读型副本主要具备以下特点：

​         不可构成 Paxos 成员组，它不作为 Paxos 成员参与日志的投票，而是作为一个观察者实时追赶 Paxos 成员的日志，并在本地回放，         

| 特性项               | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| 副本名称及缩写       | READONLY(R)                                                  |
| 是否有 LOG           | 有，是异步日志，但不属于 Paxos 组，只是 Listener (ASYNC_CLOG) |
| 是否有 MemTable      | 有(WITH_MEMSTORE)                                            |
| 是否有 SSTable       | 有(WITH_SSSTORE)                                             |
| 数据安全             | 中                                                           |
| 恢复为 Leader 的时间 | 不支持                                                       |
| 资源成本             | 高                                                           |
| 服务                 | 可非一致性读                                                 |
| 副本类型转换限制     | 只能转换成全能型副本                                         |



- 三地五中心容灾场景  参考3



![img](https://cdn.nlark.com/yuque/0/2024/png/215144/1726644125651-8e25fb95-e4e8-4ce7-a3da-57f582033b54.png)

### 参考



- [1] Consistency under partitions

​      https://cndoc.github.io/redis-doc-cn/cn/topics/sentinel.html

​      https://redis.io/docs/latest/operate/oss_and_stack/management/sentinel/

- [2] [Paxos三部曲之二] 使用Multi-Paxos协议的日志同步与恢复

​    http://mysql.taobao.org/monthly/2018/11/03/

​    http://oceanbase.org.cn/archives/111

​    http://mysql.taobao.org/monthly/2018/11/03/

- [3] [【OceanBase 4.x DBA 进阶教程】容灾架构设计](https://ask.oceanbase.com/t/topic/35611854)

- [4] OceanBase 数据库中副本类型

https://www.oceanbase.com/docs/common-ocp-1000000001406894

https://www.oceanbase.com/docs/common-ocp-1000000001406894

http://oceanbase.org.cn/archives/90

https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000001050885

https://www.oceanbase.com/docs/common-oceanbase-database-cn-100000000105305

https://www.oceanbase.com/docs/common-ocp-1000000001406108







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



