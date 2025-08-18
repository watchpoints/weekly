---
title: 什么是顺序一致性，什么是分布式协议
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
# 一、大纲

✅  阅读论文：Linearizability: A Correctness Condition for Concurrent Objects 回答是线程一致性

✅  跟着etcd学习如何使用etcd-raft实现线性化读

✅  读一致性 raft tikv 怎么优化的的


你了解分布式协议吗？谈谈你理解

什么是线性一致性？

有没使用分布式协议工程实践经验，举例说一说？

ReadIndex 默认从 Leader 本地读，如果leader 故障了怎么处理

线性一致性和 Raft
- https://tikv.org/blog/lease-read/?utm_source=chatgpt.com 
- 如何确定 leader 是 leader 就是处理就是优化措施？
- # TiKV 源码解析系列文章（十九）read index 和 local read 情景分析
- https://cn.pingcap.com/blog/tikv-source-code-reading-19/

## 为什么要用 ReadIndex？


# 二、整理素材

## 问题


ReadIndex 默认从 Leader 本地读，如果leader 故障了怎么处理
## 为什么要用 ReadIndex？

如果直接用 `commitIndex` 或 `appliedIndex`，没有验证 Leader 身份，可能因 Leader 被替换但未意识到，从而返回旧值，违背线性一致性约束 [pingcap.medium.com+15pierrezemb.fr+15alibabacloud.com+15](https://pierrezemb.fr/posts/diving-into-etcd-linearizable/?utm_source=chatgpt.com)。

ReadIndex 通过一次多数心跳确认，在成本上比写日志快得多，却保证了数据的时序正确性，是 Raft 论文推荐的“轻读优化”方式


当 Leader 故障时，会影响对 ReadIndex 机制的调用，因为 ReadIndex 依赖于 Leader 发送并确认心跳才能保证线性一致性。下面是详细处理流程和机制：

---

## ⚠️ 1. Leader 故障意味着暂时无法进行 ReadIndex 读取

- 一旦 Leader 发生宕机或网络隔离（掉到 minority 分区），Raft 集群会停止接受线性一致性读（包括 ReadIndex）。
    
- 此时所有客户端对于读取或写入都会失败或超时，必须等待新的 Leader 选举完成。([etcd.io](https://etcd.io/docs/v3.2/op-guide/failures/?utm_source=chatgpt.com "Understand failures - etcd"), [microraft.io](https://microraft.io/docs/resiliency-and-fault-tolerance/?utm_source=chatgpt.com "Resiliency and Fault Tolerance - MicroRaft"))
    

---

## 🔄 2. 新的 Leader 选举与 ReadIndex 恢复

- Followers 在检测到超过选举超时时间（election timeout）没有收到心跳后，会开始发起选举([arorashu.github.io](https://arorashu.github.io/posts/raft.html?utm_source=chatgpt.com "Understanding Raft"))。
    
- 当多数节点投票通过后，新 Leader 产生，并会写入一个 **空日志条目**（noop entry）以初始化它的 commitIndex，从而满足延续之前的提交保证。
    
- 在新 Leader 完成 noop 条目 commit 并 apply 到状态机后，ReadIndex 路径才重新可用。
    

---

## 🧭 3. 客户端角度的处理建议

- 客户端发起 ReadIndex 请求，若遇到 Leader 不响应或超时：
    
    1. 应该重试，自动切换到其他节点；
        
    2. 等待选举完成，新 Leader 稳定后继续；  
        etcd 和 TiKV 等客户端库通常会处理这种重试逻辑([etcd.io](https://etcd.io/docs/v3.2/op-guide/failures/?utm_source=chatgpt.com "Understand failures - etcd"), [github.com](https://github.com/etcd-io/etcd/issues/14027?utm_source=chatgpt.com "etcd leader change leads to "timed out waiting for read index ..."), [reddit.com](https://www.reddit.com/r/rancher/comments/zhcq3q/etcd_no_leader_cant_execute_etcdctl/?utm_source=chatgpt.com "etcd No Leader, can't execute etcdctl : r/rancher - Reddit"))。
        

---

## ⏱️ 4. 可用性 & 延迟

|阶段|状态|可用性|影响|
|---|---|---|---|
|Leader 故障期间|无心跳 → 等待选举|❌ 不可用|读写暂时失败|
|新 Leader 选举中|日志写入 noop → commit & apply|✳️ 局部可用|写路径恢复|
|ReadIndex 恢复后|可进行线性一致读|✅ 可用|读取恢复正常|

- 这种选举延迟是由 **election timeout** 决定的（典型 100–500 ms）；恢复速度取决于超时设置和选举时间([groups.google.com](https://groups.google.com/g/etcd-dev/c/82bPTmzGVM0?utm_source=chatgpt.com "Restarting one of etcd members triggers leader election"), [etcd.io](https://etcd.io/docs/v3.3/learning/learner/?utm_source=chatgpt.com "Learner | etcd"), [sofastack.tech](https://www.sofastack.tech/en/projects/sofa-jraft/raft-introduction/?utm_source=chatgpt.com "Introduction to the Raft algorithm - SOFAStack"))。
    

---

## ✅ 小结

1. 如果 Leader 故障，ReadIndex 无法立即服务，你的读取会失败或超时。
    
2. Raft 会通过选举机制迅速选出新 Leader。
    
3. 新 Leader 提交 noop 日志后，ReadIndex 路径恢复正常。
    
4. 客户端应具备重试机制，耐心等待新 Leader 稳定。
    

这就是 ReadIndex 在 Leader 故障情况下的完整处理流程。如果你想了解故障期间如何监控延迟、选举超时配置及客户端策略优化，也可以进一步深入。


## 参考资料
- https://gist.github.com/daidai21/a255b076c42e90c3785f63586ab85d78
- https://javaguide.cn/books/distributed-system.htm 
- 分布式必读经典书籍
- # DDIA 读书逐章分享——第三章（上）：LSM-Tree 和 B-Tree
## 参考书籍
![](https://pic4.zhimg.com/v2-a2c4e98b9f926caaf93abf40dc0a1487_1440w.jpg)



## 读书笔记：


可扩展性
 垂直扩展
 水平扩展



面试不通过心灰意冷怎么办？一招解决这个问题？

复盘--准备--面试，继续复盘--准备--面试


今日阅读陈东明老师的<<分布式系统与一致性>>
第1章 分布式系统的核心特性：一致性”







## 吊打面试官系列:用1 个公式彻底理解线性一致性





本文核心内容已经写完，后面文章有点长
**你可以拉倒文章末尾，先点赞，等需要慢慢再看**
你的点赞是我最大的写作动力！

## 一、 为什么要回答，通过模拟面试驱动，提前练习。

![image.png](https://s2.loli.net/2025/07/01/qBsvenyjGwDcxPQ.png)





为了让激发你探索欲望，先拿出思考 60 秒下这个问题，期间。
- 暂停手机滑动
- 停敲击键盘
- 更不需要上网查询
- 这里上来不会直接给出答案


你可以趁着下面时间想一样
- 中午倒茶水时间
- 在中午吃饭路上
- 下午溜达时候 

>想不出来没有任何关系，就当一60秒放松，
>这个不需要背诵，甚至花时间探索，**我帮助你研究一些没用知识**，


面试官：谈谈你对分布式线性一致性理解


策略1：青铜，程小义：
- 思考：工作 10 年竟然无法回答，根本想不起了 后悔，生气 .回答：说想不起来了（无用）
- 思考:有点印象，模糊感觉，说不清楚，回答：然后支支吾吾（无用）

策略 2：黄金,程小义：
- 思考：一切问题都是纸老虎，你相信你可以抓重点，解决主要矛盾，把分布式想象成单节点下多线程，保证并发请下读写数据一致性。
-  回答：线程一致性就是并发情况下，读操作读取读取 写操作写入“最新”数据   ,为了保证并发下线性线性一致性，单节点和多节点 有不同处理方式。。。。。，主要围绕读，写操作展开。

策略 3： 王者，程小义：
- 看论文，参加Tikv比赛，做实验，提交 pr，然后自信回答。
- 看面什么公司，如果滴滴，字节，百度 一面**根本不会这样提问**，why，互联网主要应用，他们c++ stl 怎么写的，日常for 循环算法写，从**熟练到精通**，他们工程中遇到问题抽象出来。
- 看面什么公司，传统公司根本提问一致性问题，c++语法 也根本不会，面试 xxx 设计，xxx 如何实现的高大上问题。**根本不会循序渐进，不会走，就开始跑这样情况经常出现**。

60 秒思考结束

先说什么公式， 
- 如果2 个操A,B并发执行，
- 他们执行顺序执行不是唯一的，可能AB 或者 BA

就是这么简单，开始。

**v1.0草稿版本**

## 二、先定义清楚问题是什么，学习路径
### 1.1 目标与学习方法 ：

✅  <font color="#245bdb">目标：了解基本概念</font>
 1. 什么是顺序一致性
 2. 什么线程一致性
 3. 顺序一致性与线程一致性他们之间区别？
 4. 在工程实践中，做哪些优化
✅  <font color="#245bdb">学习方法</font>
1. 阅读 陈东明老师的<<分布式系统与一致性>>![](https://s2.loli.net/2025/07/01/AWGFEjxXuk58J2e.png)

2. 阅读唐伟志老师的 <<深入理解分布式存储系统>>![](https://s2.loli.net/2025/07/01/T1jWSK8xIpeoFiY.png)

3. 了解TiKV/Etcd源码实现过程，先模仿在创造。
![image.png](https://s2.loli.net/2025/07/01/FDpvi4VEamrBtKT.png)

![https://www.qiyacloud.cn](https://s2.loli.net/2025/07/01/Yj9HPNXTgwcdsLo.png)

![](https://files.mdnice.com/user/5197/b1b720b6-65dc-4e50-b418-452b97cbfac2.png)
4. 动手练习 写无锁LRU 队列
    阅读：C++并发编程实战，java并发编程实战 
![](https://s2.loli.net/2025/07/01/YkAo54bWFZqBmhC.png)
![](https://s2.loli.net/2025/07/01/fEQxyumiSWvI7sw.png)


5. 提交 pr,提交 pr（最关键关键一步）
### 1.2 分布式一致性常概念上常见误区

一致性虽然是我们非常常用的一个词，
但是非常不幸的是，这个词有太多的含义，在使用的非常容易混淆

什么是—致性这个问题,会发现每个人给出的答案都不太-样
- 有人会说是CAP定理里面的一致性
- 有人会提到数据库事务ACID中的一致性
- 更有甚者会说Paxos或Raft算法中的一致性

通过阅读 清楚地知道**三者是完全不一样的概念**。

>个人感受：共识（Consensus）不等于一致性（Consistency）

要指出一个错误观点即把Paxos或Raft称作分布式一直致性算法 
完全是中文**翻译导致的错误**它们的英文单词并不一样，本文Paxos/Raft统称<font color="#245bdb">共识算法</font>
- 一致性则侧重 于研究副本最终的稳定状态
- 共识问题可以用数学语言来准确描述 —个分布式系统包含′!个进程,记为｛0,1’2’…’′′ˉl｝每个进程都有一个初始值，程之间互相通信,设计—种共识算法使得尽管出现故障但进程 ,间仍能协商出某个不可撤销的最终决定值
- 大白话：

举个生活中的例子’小明和小王出去聚会’

小明问:“小王’我们喝点什么吧? 

”小王:“喝咖啡怎么样? ’

’小明; “好啊’那就来杯咖啡,

在上面的场景中,

**小王提议喝一杯咖啡,小明表示接受小王的提议,两人就“喝杯咖啡”**

**这个问题达成共识’**

并根据这个结果采取行动

这就是生活中常见的共识

**本文重点不是解释 共识算法**

>个人感受：不是ACID中概念上的一致性

ACID的致性属于数据库领域的概念 主要是指数据的一致性没有被破坏,
这种一致性要求不仅指常见的数据库完整性约柬

例如用户可以指定数据库字段A和B必须满足A＋B＝l00·
**这类一致性不属于本文章一致性讨论范畴**
- 需要指出的是’经常会有人将隔离性也纳入一致性有点类似，但是不完全一样

### 1.3 一致性模型到到底是什么

一致性模型就是指,在**并发**编程中,系统和开发者之间的一种约定

如果开发者遵循某些规则
那么开发者执行读操作或写操作的结果是**可预测**的



分布式—致性<font color="#245bdb">验证框架Jepse</font>n对一致性模型进行过分类（TIDB 使用 Jepsen 来进行一致性验证）
来自 官网 https://jepsen.io/consistency/models

![图1-本文重点 右面的 线性，顺序一致性](http://r12f-cdn.azureedge.net/r12f-assets/post_assets/2021-01-14-summarizing-consistency-model/consistency-model-family-tree.png)

- Unavailable表示：满足这类—致性模型的系统发生网络分区时为了保证数据一致性和正确性系统会不可用
- StickyAvailable表示：满足这类一致性模型的系统可以容忍部分节点发生故障还未
    出现故障的节点仍然可用,但前提是**客户端不能将请求发送到不可用的副本节点**
- TOtalAVailab 表示: 满足这类—致性模型的系统可用性是最高的即使网络发生严重,分区在没有发生故障的节点上’仍然保证可用。这类—致性模型包括读后写—致性、单调读一致性和单调写一致

 翻译：

##### 一致性：

![image.png](https://s2.loli.net/2025/07/01/cTExVLrN2yQR7n1.png)


##### 可串行化 / 事务一致性（Serializable）

参考
- http://www.bailis.org/blog/when-is-acid-acid-rarely/
- https://github.com/ept/hermitage

可串行化也分为强一致性和弱一致性,事务隔离级别如下。

| 模型                                                            | 解释                                                                 |
| ------------------------------------------------------------- | ------------------------------------------------------------------ |
| 未提交读  <br>Read Uncommitted (RU)                               | 最低等级的一致性保证，在事务之中允许读取未提交的数据（Dirty Read）                             |
| 已提交读  <br>Read Committed (RC)                                 | 在事务中，读取到的数据一定是提交了的，但是不保证这些数据会不会在重复读时发生变化                           |
| 可重复读  <br>Repeatable Read (RR)                                | 在已提交读的基础上，还保证在一个事务中，读取过的数据在重复读中保持不变，但是同样的查询可能会查到新的数据（Phantom Read） |
| 可串行性  <br>Serializability (S) /  <br>Serializable consistency | 最高等级的一致性保证，提供和SI同样的保证，但是一般是使用锁而不是快照来实现的，不会在合并修改时发生的错误              |
脏读：
例如，事务T1修改一个数据，事务T2在T1提交或者回滚之前读取到了这个数据。如果T1执行了回滚，那么T2就读取到了一个不存在的值。 
 
  ![脏读](https://s2.loli.net/2025/07/01/onHAjRiZeJzPT4D.png)

 在这个例子中，每条虚线表示一个事务，从左到右为时间流逝的方向。
 - w（a=1）表示将值1写入a中；
 - r（a=1）表示读取a，读取到的结果是1；
 - abort表示取消这个事务

不可重复读（non-repeatable read，NRR）
例如，
- 事务T1读取一个数据，
- 然后事务T2修改或者删除这个数据并提交。
- 接下来，如果T1试图再次读取这个数据，
- 那么它会读取到一个修改过的值，或者发现这个数据已经被删除了。 
 
 ![](https://s2.loli.net/2025/07/01/eUsgMpfITuQCEo1.png)

 在这个例子中，commit表示提交事务.



## 三、什么是线程一致性

#### 定义 
**<<Linearizability: A Correctness Condition for Concurrent Objects>>**
地址：https://cs.brown.edu/~mph/HerlihyW90/p463-herlihy.pdf


给出线性致性的严格定义是

给定一个**执行历史**， 
执行历史根据并发操作可以扩展为**多个顺序历史**，
只要从中**找到一个合法**的顺序历史，,那么该执行历史就是线性一致性的。

即：
只要我们能够把执行历史转为然后判断顺序历史是否合法,
就能知道这个执行历史是否满足线性一直致性。

维基百科定义：

**线性一致性**（Linearizability），或称**原子一致性**或**严格一致性**指的是程序在执行的历史中在存在可线性化点P的执行模型，
这意味着一个操作将在程序的调用和返回之间的某个点P起作用。
这里“起作用”的意思是被系统中并发运行的所有其他线程所感知。

**啥玩意，看不懂 没关系 ，太拗口了慢慢往下看，**

这里需要 补充基本概念

#### 基本概念



1. Operations：一次操作 例如读写操作，发起调用（invocation）事件，返回响应（associated respons）事件 ![图 2-写操作](https://s2.loli.net/2025/07/01/oFMemTngtuiEsAh.png)
2. history： 历史 A history is a finite sequence of operation invocation and response events.（一个历史是一个由有限个操作的调用事件和返回事件组成的序列）


![图 4-就是多个操作组成](https://s2.loli.net/2025/07/01/2kOf7tzuG1HEKeZ.png)
 3. sequential history：顺序历史(Sequential history)：历史H是顺序的，如果满足以下两个条件：
    - H中的第一个成员为调用事件；
    - 除了H中的最后一个调用事件之外，H 由相邻的、两两相匹配的调用事件和响应事件组成。
    
	原文：
	A history H is sequential if: 
	 (1) The first event of H is an invocation. 
	 (2) Each invocation,except possibly the last,is immediately followed by a matching response.Each response is immediate followed by ainvocation.”

 看不懂没关系 直接看例子
![并发执行与顺序执行](https://s2.loli.net/2025/07/01/SNvJfna1MDkuQ2W.png)

### 如何达到线性一致
总的来说 线性—致性主要有两个约束条件,
- 第—顺序记录中的任何一次读必须读到**最近/最新**1次写入的数据;
- 第二顺序记录要跟全局时钟下的顺序一致。

线性一致性（Linearizability）要求：
1. 所有操作都必须**看起来像是**在某个瞬间（原子地）完成。
2. 这个瞬间必须**位于操作的调用与响应之间**。
3. 且整体操作序列与某个串行顺序一致，该顺序要与**实际发生时间的先后顺序相**

### 案例1

![图 5--历史执行顺序](https://s2.loli.net/2025/07/01/PwTVRXeUCGIZyNg.png)



图表示的行为满足线性一致，为什么

- 对于同一个对象 x，其初始值为 1
- 客户端 ABCD 并发地进行了请求，按照真实时间（real-time）顺序
- 各个事件的发生顺序如上图所示

**根据线性一致性定义：上面是4 个客户端，并发执行一个其中要给执行顺序，**

**然后扩展到多个执行顺序**


| 客户端 | 操作类型 | 单个顺序            | 整体并发     | 历史顺序  | 至少一个符合    |
| --- | ---- | --------------- | -------- | ----- | --------- |
| A   | 读    | `R()` → `Ok(1)` | AB 是并发   | AB｜BA | 一个AB 符合要求 |
| B   | 写    | `W(2)` → `Ok()` | AB 是并发   | AB｜BA | 一个AB 符合要求 |
| C   | 读    | `R()` → `Ok(2)` | BC 是先后执行 | BC    | 只有一个顺序    |
| D   | 读    | `R()` → `Ok(1)` | 很早       | 最晚    | ❗返回1      |

客户 段 D：为什么返回 1 也合法？

非技术解释：
- D 像是**打了个很早的电话，但对方很晚才接通（响应）**；
- 在它打电话时，世界还是旧状态（x=1）；
- 虽然它“听见”消息晚了，但这个电话是**早打出去的**，所以它看到旧状态也合理。


线性一致性允许的关键在于：
![在看这个图，并发关系 ](https://s2.loli.net/2025/07/01/SNvJfna1MDkuQ2W.png)


- 操作必须**线性化**（即可以插入到一个串行的顺序中），
- 并且**线性化点必须落在调用和响应之间** 

- Client D 的读请求虽然很早发出，但它的响应直到很晚才收到，
- 所以它的线性化点可以放在 B 写操作之前，也可以之后。【并发关系 变成2 个历史顺序，只要一个符合要即可】

> 只要这个“读”在线性时间线上被放在写操作 `W(2)` 之前，它返回 `1` 就是合法
> 


|你担心的问题|实际判断标准|
|---|---|
|D 的响应晚于写操作 B，应该看到2|❌ 不成立，因为线性一致性只要求线性化点在调用和响应之间，可以在 B 写入之前|
|D 返回 1 是否违反线性一致性|✅ 不违反，只要我们在线性时间轴中将 D 的读放在写 B 之前|


### 案例2：

从线性一致性初始论文中找出两个执行历史
请问图中的两个执行历史Hl和H2是否满足线性一致性?

![image.png](https://s2.loli.net/2025/07/01/Gk2jgh8IyBpH1Pd.png)

答案:
- Hl满足线性一致性
- H2不满足线性一致性  客户端A最后的write(0) 和  客户端 B read(1) 在时间上顺序关系。不满足读取最新写入条件。


>个人感受：
	一句话概括：在分布式系统上实现寄存器语义，这个单机定义一样，并发执行会有无数历史 顺序，从一个历史顺序，推到多个其他执行顺序，只要一个符合要求就满足线性一致性。


#### 如何实现

1. 单机多线程如何实现线程一致性：

```c++
//i是关键变量
int inc_counter()
{
	 lock();
	 i++;
	 int j = i；
	 unlock();
	 return j；
}

```

这就是我们最常见的并发编程的线性一致性实现。

还有操作系统都提供了原子比较-交换（CAS）操作。


2. 分布式系统

| 角度    | 具体表现                        | 为什么贵                     |
| ----- | --------------------------- | ------------------------ |
| 单机并发  | atomic、mutex、memory barrier | ❌ CPU级别乱序执行，cache一致性管理复杂 |
| 分布式系统 | Raft、Paxos、ReadIndex、心跳确认   | 无全局时钟，通信延迟高，协议复杂         |
| 时间同步  | 全局线性视图需要一致排序                | 物理时钟不可控，需要逻辑时间模拟         |
## 四、什么是 顺序 一致性

### 4.2 官方理解


顺序一致性是Lamport在1979年首次提出，

论文《How to Make a **Multiprocessor** Computer That Correctly Executes **Multiprocess** Progranm》

翻译：
如果一个多处理器计算机具有顺序一致性的特性，
那么这个多处理器计算机就可以保证多进程程序正确运行


请一定按照 Lamport的思路来理解，暂时放下之前理解方式。
![](https://doc.yonyoucloud.com/doc/wiki/project/java-memory-model/images/11.png)
![](https://doc.yonyoucloud.com/doc/wiki/project/java-memory-model/images/12.png)
![https://lotabout.me/2019/QQA-What-is-Sequential-Consistency/](https://lotabout.me/2019/QQA-What-is-Sequential-Consistency/Sequential-Consistency.svg)
- 定义
顺序一致性的定义Lamport的论文中的定义是：

当一个系统满足下面的条件时，
这个系统就具有顺序一致性（sequential consistency）

原文中要求顺序一致性模型满足两个要求：

> the **result** of any **execution** is the same as 
> if the operations of all the **processors** were executed in some **sequential orde**r,
>  and the operations of each individual processor appear in this sequence in the **order** specified by its **program**.
> 
> （任何执行的结果都与所有处理器的操作按某种顺序执行的情况相同，每个单独的处理器的操作按其程序指定的顺序出现在这个序列中 ，**根本看不懂呀。说的不是人话**）

拆分关键词

**program** :代码，多个代码 ，就并发执行意思
**execution**：执行，并发执行 顺序不确定，不同结果
sequence：代码顺序执行
排序（order）是指经过一定的调整，让某种东西按照一定的规则排列，变得有序

sequential order：排序后的顺序。

第一部分：结果相同，全局视角一致

第二部分：程序顺序，可以重拍


顺序一致性是程序正确执行的并发条件（concurrency condition）

它有两个条件：

> Requirement Rl: Each processor issues memory requests in the order specified by its program.
> 
> Requirement R2: Memory requests from all processors issued to an individual memory module are serviced from a single FIFO queue. Issuing a memory request consists of entering the request on this queue.

顺序一致性（Sequential Consistency）提出了两个约束：

- 单个节点的事件历史在全局历史上符合程序的先后顺序
- 全局事件历史在各个节点上一致

啥意思

顺序一致性要求多处理器系统的执行结果看起来就像所有处理器的操作按照某种全局的顺序一个接一个地执行，而且每个处理器的操作在这个全局顺序中保持其程序规定的顺序。

换句话说，虽然实际上多个处理器是并发执行的，但从结果上看，好像所有操作被排列成一个线性的序列，这个序列满足：

1. 每个处理器的操作顺序与程序中的顺序一致。
2. 整个系统的执行结果与这个线性序列的执行结果一致。

有啥用

Under sequential consistency，
all operations appear to have executed atomically in some order that is consistent with theorder seen at individual nodes and that is equal at allnodes.”​

在顺序一致性的条件下，所有操作好像都是原子操作，并且单个节点看到的顺序与所有节点看到的顺序是相同的。​


### 4.3 顺序一致性难吗？

难，现代的多核 CPU 依然达不到顺序一致性。

我们知道 CPU 执行的主要瓶颈其实是在与内存交互，工程师为了让 CPU 能高速执行，在 CPU 内部使用了多级缓存。它的存在，使得即使 CPU 内部顺序执行指令，指令的结果也可能不满足顺序一致性：

参考：https://www.codedump.info/post/20220710-weekly-22/
![](https://lotabout.me/2019/QQA-What-is-Sequential-Consistency/CPU-Cache.svg)

即如果程序没有数据竞争，则 CPU 可以保证顺序一致性，
而如果遇到数据竞争，就需要程序里手工使用一些数据同步的机制（如锁）。

##  五 、什么是分布式共识算法，如何保证数据的一致性

https://snarfed.org/s/transactions_across_datacenters_io.html
https://www.cs.princeton.edu/courses/archive/fall16/cos418/docs/L13-strong-cap.pdf
## 六、结合开源项目工程优化实践（）



