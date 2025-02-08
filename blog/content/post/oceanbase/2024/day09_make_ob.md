---
title: 成为ob贡献者(09):如何证明采用了PALF设计就是安全的
date: 2024-12-21
description: 自我驱动学习
draft: false
categories:
  - oceanbase
---
---
title: 成为ob贡献者(6):如何阅读Paxos代码 
date: 2024-10-04
description: 自我驱动学习，专业上只能靠自己
categories:
  - oceanbase
---
**温故而知新：**

成为ob贡献者(08):接口层 PALF回顾

**小王提问：** 在PALF设计中提到副本的日志系统（replicated logging system），如何理解？

**老王回答：**

- 函数调用理解吗？
- RPC，就像调用本地函数一样调用其他节点上的函数，隐藏了网络通信的细节，这个也理解吧？
- Linux系统标准IO函数read、write理解吧？
- 通过PALF接口层 ，事务引擎可以把redo日志像本地一样写到其他节点，因此强一致性协议实现，网络传输的细节
   同样，**根据序列号，我们可以像读本地一样读其他节点的数据**。理解了吧。
![1734621005559-c6125a15-dc1b-49d7-8ff8-33f23077f8af.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_1256%2Climit_0](https://cdn.nlark.com/yuque/0/2024/png/215144/1734621005559-c6125a15-dc1b-49d7-8ff8-33f23077f8af.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_1256%2Climit_0)

**通过本文你获得如下收益：**

有了强一致性协议Paxos。
- 领导选举算法可以用它。
- 最长日志（ the longest logs）重新确认（reconfirmation）算法可以用它
- 甚至日志复制 本身也可以用它。


小王提问：太抽象了，不知道说是什么？该怎么入手呢


老王回答：
     好吧，PALF中 pending follower 和state machine safety 提到4.1章节，
      重点看这个吧，然后完成下面2个作业

>1. 如何证明证明 PALF 是安全的
>2. 回答PALF 与raft差异

 raft转成下面3个子问题
 - Leader选举
 - Log复制
 - 安全性保证
 PALF 转换下面5个子问题
 
- 选举安全性（日志重新确认，根据优先级选举，然后获取最新日志来保证）
- 领导者只追加模式、（ PALF 采用 Raft 的日志复制方案，raft保证了它是对的）
- 日志匹配、（PALF 采用 Raft 的日志复制方案，raft保证了它是对的）
- 领导者完整性（日志重新确认来保证）
- 状态机安全性（已经写入不能被修改。**如下图怎么证明呢**）


英文单词
- election safety
- leader append-only
- log matching,
- leader completeness,
-  state machine safety
![图片1](https://cdn.nlark.com/yuque/0/2024/png/215144/1734682545213-e4e46bcb-df76-4f40-a566-74c8b3a4db07.png?x-oss-process=image%2Fformat%2Cwebp)
              


1. 捕捉想法:  
   无数个问题同时出现，让思路更加混乱，但是描述不出来具体是什么问题。
   凭着感觉  开始集中精力阅读重点文章
  - PALF: Replicated Write-Ahead Logging for Distributed Databases 
  
2. 动手验证
  > 任务1：翻译：PALF: Replicated Write-Ahead Logging for Distributed Databases
  > 
  > 任务2：做练习题：#Paxos 试题 看看你能得多少分
  

开始：


### Leader Election

![[Pasted image 20241217091033.png]]

小王提问：什么是 pending follower,和上一个leader未提交的日志有关系吗？ （4.1章节有解释


 **pending follower：**
The reason for switching to  pending follower rather than follower is that the transaction engine  
may have appended some logs to the leader before the **leadership**  
**transfer**.

To determine the replication results of these pending logs,  
 **pending followerand wait  
for logs from the new leader (§4.1). After the status of all pending  
logs is clear, the replica will switch to follower.

当一个服务器（前领导者）失去领导地位后，它不会立即变成普通的追随者（follower），而是先变成一个待定的追随者（pending follower）。

这样做的原因是因为在领导权转移之前，事务引擎可能已经在领导者上追加了一些日志条目。

为了确定这些待定日志的复制结果，前领导者必须进入待定追随者角色，并等待来自新领导者的日志。以下是这个过程的几个关键点：

1. **待定日志的处理**：前领导者可能在其日志中有一些未被复制或未被提交的条目，这些被称为待定日志（pending logs）。这些日志的复制结果未知，需要通过与新领导者的通信来确定。
    
2. **等待新领导者的日志**：作为待定追随者，前领导者会等待新领导者发送的日志条目。这个过程中

具体等看到4.1章节时候才有明确答案。


---
**选举算法**

选举算法，作为 Basic Paxos算法的一个变体，其核心任务是选举出优先级最高的副本作为Leader。本文的焦点集中在日志复制上，而非选举算法本身。

**辅助理解**
1. 选举算法的实现依托于Paxos算法，并非通过HA（高可用性）等其他方式来实现。
2. Basic Paxos算法通过两次RPC 确定一个值。
3. 最高优先级 也是一个值，可用**Basic Paxos**实现



原文：
The election algorithm is  essentially a variant of **Basic Paxos**, it reaches a **consensus** about  which replica owns the highest priority in a majority

This paper focuses the design of the replicated  
logging system, therefore, we leave implementation details of the  
election algorithm for another paper


### Basic Paxos实现 最长日志（ the longest logs）重新确认（reconfirmation）算法


根据上文，领导选举是根据优先级选举的， 不是根据最长日志选举的，
自然新的候选人日志远远落后其他节点，

候选人日志不是最新日志，其他节点日志是最新的。
这个场景不能按照候选人日志覆盖其他节点的日志

因此需要多个节点对**最大日志这一个数**值达成一致。

回顾：
根据<<成为OB贡献者第四天：从单点到多节点 i++并发方案 >> 
https://open.oceanbase.com/blog/14180550530
![img](https://obcommunityprod.oss-cn-shanghai.aliyuncs.com/prod/blog/2024-09/1726543331323.png) 


>自然Basic Paxos满足这个特点，（假如系统只写一个值）
>Paxos分为2个阶段 Prepare， Accept 。 ---图2
  在并发情况下， 在Accept 阶段 已经写入提案（没有被大多数节点接受），但是提议者可见（运用多数派查询到）不能被修改。
  s1 发起3.1 提案，s5. 发起4.5提案， 2个不同的提案编号，但是写入都是同一个值x --图1


![](https://obcommunityprod.oss-cn-shanghai.aliyuncs.com/prod/blog/2024-09/1726543481005.png)



       图片1 情况2 提案被接受，提议者可见

![](https://obcommunityprod.oss-cn-shanghai.aliyuncs.com/prod/blog/2024-09/1726543417539.png)
    
                  图2： 
---

如图：最长日志（ the longest logs）确认算法

（s5. 发起4.5提案，但是s1 发起3.1 提案部分写入， 2个不同的提案编号，但是写入都是同一个值x 是不是有点类似？**对一个值达成一致**）
![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734618996253-fce72585-110b-4cd3-a7a0-afbbc36177dd.png?x-oss-process=image%2Fformat%2Cwebp)


**日志重新确认算法（Alg.1）**
1. **准备阶段（Paxos Prepare消息广播）**
   - 候选人向所有副本广播带有高级ProposalID（领导任期的标识符）`𝑝𝑖𝑑 + 1`的Paxos Prepare消息（第2行）---pid 理解成提案编号
   - 每个副本将存储Prepare消息中的`𝑝𝑖𝑑`，并且只有当Prepare消息中的`𝑝𝑖𝑑`大于副本所见过的最大ProposalID `𝑝𝑖𝑑𝑚𝑎𝑥`时，才会向候选人响应其日志（第9行）。
   - 为了避免传输无用的日志，Prepare消息的确认只包含日志的尾部LSN。
2. **接受阶段（Paxos Accept）**
   - 一旦候选人从大多数副本收到投票（第3行），它就开始Paxos接受阶段：
     - 选择日志最长的副本（第4行）
       //selecting the replica with the longest logs
        //对比 图片1 情况2 提案被接受，提议者可见.
     - 从该副本获取日志（第5行）。
        // int read(LSN lsn, Record &r);  //根据LSN随机读位置 PALF。
     - 将这些日志复制到所有副本（第6行）。
3. **启动工作日志复制**
   - 最后，候选人将StartWorking日志复制到所有副本（第8行）。
   - 注意，StartWorking日志是一种特殊的重新配置日志（§5.3），用于回滚前领导可能未提交的成员资格。
   - 只要StartWorking日志达到大多数，候选人就会担任领导职责。

>课后作业 对比Raft领导者变更的安全性

### 举例说明

假设一个分布式系统中有以下场景：

- **前任领导者**：在任期T1中，领导者尝试添加一个新的成员到集群中，但这个变更没有被提交（即没有复制到多数节点）。
- **领导者失效**：随后，前任领导者因为某些原因失效。
- **新领导者选举**：在任期T2中，新的领导者被选举出来。

在Raft中，以下是发生的情况：

- **日志一致性检查**：新领导者会发现前任领导者未提交的成员变更。由于这个变更没有被多数节点接受，新领导者不会将其视为有效变更。
- **覆盖不一致的日志**：新领导者会继续复制其自己的日志条目，覆盖前任领导者未提交的变更。这确保了系统状态的一致性。

在StartWorking日志的机制中：

- **StartWorking日志**：新领导者会复制StartWorking日志到所有副本。这个日志会包含指令来回滚前任领导者未提交的成员变更。
- **回滚未提交变更**：通过复制StartWorking日志，新领导者确保所有副本都认同回滚未提交的成员变更。这保证了即使在新领导者接管后，系统的配置状态也是一致的。


---



原文：
~~~ 
Due to flexible leader election, the candidate may have fewer logs than other replicas. Before taking over as  a leader, the candidate should re-confirm the logs appended by the  
previous leader to guarantee that its logs are not fewer than any  
replica in a majority. The log reconfirmation (Alg.1) is essentially a  complete instance of Basic Paxos [27]. Specifically, the candidate  broadcasts the advanced ProposalID(identifier for the leader’s term)  𝑝𝑖𝑑 + 1 to all replicas with Paxos Prepare messages (line 2). 

Each  replica will store the 𝑝𝑖𝑑 of the Prepare message, and respond to  
the candidate with its logs, only if the 𝑝𝑖𝑑 in the Prepare message is.
larger than the maximum ProposalID 𝑝𝑖𝑑𝑚𝑎𝑥 the replica has seen  
(line 9). 

To avoid transporting useless logs, the acknowledgement  
of Prepare message only contains the tail LSN of logs.  

Once the candidate receives votes from any majority of replicas  
(line 3), it starts the Paxos accept phase: selecting the replica with  
the longest logs (line 4), getting logs from it (line 5), and replicating  
these logs to all replicas (line 6). 

Finally, the candidate replicates a  
StartWorking log to all replicas (line 8). Note that the StartWorking  
log is a special reconfiguration log (§5.3), it is used to roll back  
the possible uncommitted membership of the previous leader. 

The  candidate will serve as the leader as long as the StartWorking log  
reaches majority

~~~


### 日志复制 Log Replication

>带着问题看文章 PALF 日志复制 和Raft类似，看到这句话小心，不一样地方在哪里
>
1. 首先看Raft是怎么做日志匹配的，
2. 与Raft不同点是什么?
3. 能举例说明吗？


### 1.什么是raft 

- 主要参考资料：Raft算法精读
- 演示：https://raft.github.io/
- https://pdos.csail.mit.edu/6.824/papers/raft-extended.pdf
- https://pdos.csail.mit.edu/6.824/papers/raft-faq.txt

raft算法之所以简单的原因之一是它将问题分解成三个子问题，分别是：

1. Leader选举
2. Log复制
3. 安全性保证

>图3：持久化和非持久化状态：

思考：Raft 协议中如何确保易失性状态（volatile state）在服务器重启后重建？

![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734767369718-9ae087dd-f51c-4228-9923-f7062858104d.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_605%2Climit_0)
 图3：选举
 
![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734767355086-d9dbed31-5a32-481e-b892-7e3360408aa9.png)
![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734767319402-4aaec552-9456-4101-a802-61e0510c7bf3.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_608%2Climit_0)

### Entry格式

Logs由Entries组成，每个Entry包含一个term和命令，格式如下

![Entries](https://cdn.nlark.com/yuque/0/2024/png/215144/1734769716982-c4bec496-4892-4c96-84f6-f966ae8894aa.png?x-oss-process=image%2Fformat%2Cwebp)
## raft算法保证的性质

1. **Election Safety**: at most one leader can be elected in a given term.
2. **Leader Append-Only**: a leader never overwrites or deletes entries in its log; it only appends new entries.
3. **Log Matching**: if two logs contain an entry with the same index and term, then the logs are identical in all entries up through the given index.
4. **Leader Completeness**: if a log entry is committed in a given term, then that entry will be present in the logs of the leaders for all higher-numbered terms.
5. **State Machine Safety**: if a server has applied a log entry at a given index to its state 
6. machine, no other server will ever apply a different log entry for the same index.
暂时可以先不看，需要知道的是raft所有的规则都是为了保证上面的这些性质，而这些性质又是保证raft正确的前提。

**要想理解raft，其实就是搞明白：**

1. leader和follower需要维护哪些变量，每个变量的含义
2. leader什么时候发送AppendEntries RPC，携带哪些参数，follower收到请求后做什么？leader收到响应后做什么？
3. candidate什么时候发送RequestVote RPC，携带哪些参数，follower收到请求后做什么？candidate收到响应后做什么？


安全性：
什么情况下Entry可以被commit？满足以下**两个**条件：

1. A log entry is committed once the leader that created the entry has replicated it on a majority of the servers.（leader将该entry拷贝到大部分server中）
2. **不能commit term比当前leader的term小的Entry。和前面leader选举限制一样，这也是论文在5.4 Safety这一小节给的一个restriction**。不是很好理解，论文在5.4.2节给出了解释
![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734767032169-43a91eb6-bc60-4d7a-9ab2-919966ef7464.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_694%2Climit_0)

T1： 图c： A被选举成leader， 此时有上一个任期的term=2 ，index=2，和term=4 index=3的日志

假如 先处理index=2的日志，这个做法是错误的，

T2：index=2 term=2的日志复制的S3节点，但是没有commited确认，此时S1 crash。

T3：图d  S5当选leader
    为什么，只有S1 term=4 反对 其他同意。S5当选领导。

> 该怎么做: index=2，term=2，index=3，term=4 一块复制

如果S1现在奔溃了，S5不可能成为leader（S5的最后一个Entry的term=3，S1, S2, S3都会拒绝投票，因为它们的logs更up-to-date），也就不可能出现commit的Entry被覆盖的情况。


>关键：S5不可能成为leader 是日志中包含term，先同步当前任期请求，在处理历史任期请求。
     如果当前term没有请求，构造空的请求，然后一起和历史任期请求一块处理


小王：这个违背尝试呀？这个没有严格index，从小到大处理

作业：对比PALF怎么实现的处理上一个任期的日志呢


### 2.  PALF 日志复制

a log sequence number (LSN)


简单的描述
1. 领导者根据LSN产生日志（log entry），复制完成后，最后 按LSN的顺序由领导者提交（committed）

3.  当follower 收到新的新的日志（log entry），
    - 必须保证历史日志（preceding logs）被接受（accepted）然后接受当前日志
    - 如果同一个位置有冲突，按照最提案编号来截取。
    
  注意 accepted 和committed区别，并且 根据LSN顺序处理。
 
> 顺序处理 中间有空日志吗？ 具体更需细节文章没有描述，需要查资料确认

a log sequence number (LSN) 大小等于 last the LSN + 当前日志大小 



![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734682322323-9b615841-3646-4621-8778-70dad23dab8c.png?x-oss-process=image%2Fformat%2Cwebp)

### 3 .安全性不同地方在哪里

得益于 Raft 中的安全性论证，我们只需验证 PALF 是否确保以下特性：
- 选举安全性（日志重新确认，根据优先级选举，然后获取最新日志）
- 领导者只追加模式、（raft模式 PALF 采用 Raft 的日志复制方案）
- 日志匹配、（raft模式 PALF 自然采用 Raft 的日志复制方案）
- 领导者完整性（日志重新确认）
- 状态机安全性（已经写入不能被修改）

Raft和PALF之间的关键区别在于日志重新确认。
候选者执行一次基本Paxos实例，以从接受大多数日志的副本中学习缺失的日志。

如果前任领导者提交的日志条目已被多数接受，那么候选人必须看到并学习到这一点，
从而确保了选举的安全性和领导者的完整性



**状态机安全性举例说明**

 这个是例子，完整证明看4.1章节
 We leave the proof of the state  machine safety property in §4.1



![](https://cdn.nlark.com/yuque/0/2024/png/215144/1734682545213-e4e46bcb-df76-4f40-a566-74c8b3a4db07.png?x-oss-process=image%2Fformat%2Cwebp)


如图6，一个系统有三个节点A,B,C组成，（a）（b）（c） 代表三个时刻。

1. a时间点：节点A是leader，已经接受日志log1、log2和log3。
其中日志log1已经提交到全部节点，log2 写入大多数节点，log3 没有开始复制


2. b时间点：
   A暂时与B和C失去连接 C被选为新的领导者，利用reconfirmed从B那里重新确认了当前最长日志log2，然后复制到B 和C节点

3. c时间点:
当网络恢复后（参见图6c），副本A转变为待定的追随者状态，因为它的选举租期已经过期。它开始从新领导者C那里接收日志，A中的log2由于已被新领导者提交，将被接受。

与此相反，A中的log3将被截断，因为C中有一个包含更大ProposalID的另一个log3。


The state machine safety property of the consensus protocol is  
ensured by the explicit replication results. If a log has been **committed,** it must have been applied to the leader and will be replayed  
to the follower. If PALF fails to reach consensus on the log, state  
machine of the **previous leader will be rolled back** by calling the  
failure function.


![](https://cdn.nlark.com/yuque/__mermaid_v3/bbcd7921529875f7b7ba215178cf7754.svg)


原文：
### Log Replication
- **Responsibility of the Leader**: Once a leader is elected in PALF, it becomes responsible for replicating and committing log entries across the cluster.
- **Log Sequence Number (LSN)**: When log entries are appended to the leader, each entry is assigned an LSN, which represents the physical offset in the log blocks where the entry is stored. The LSN for the next log entry is the sum of the current log's LSN and the size of the log.
- **Log Storage**: Log entries are stored in log blocks in LogStorage, and the LSN system allows clients to interact with PALF as if it were a normal file system.
- **Order of Log Acceptance**: Followers will only accept a log entry if all preceding entries have been accepted. If there's a conflict with a higher proposal number, PALF will truncate the conflicting logs, similar to Raft.
### Correctness
- **Properties to Validate**: To ensure correctness, PALF must validate properties such as election safety, leader append-only, log matching, leader completeness, and state machine safety.
- **Natural Properties**: Leader append-only and log matching properties are naturally provided by PALF since it uses the same log replication scheme as Raft.
- **Log Reconfirmation**: The key difference from Raft is the log reconfirmation process. A candidate for leadership performs an instance of Basic Paxos to learn missing logs from the replica that has accepted the most logs in a majority. This ensures election safety and leader completeness.
- **State Machine Safety**: The proof of state machine safety is deferred to a later section (§4.1) because it relates to how logs are applied to the transaction engine.


## 4.1 章节 明确的执行结果


第4章节 介绍了为OceanBase数据库的事务引擎设计的PALF（基于共识协议实现）的特性。


>通过阅读4.1章节 期望回答 网络分区情况下，还能保证数据一致性吗？

1. **什么是pending logs，不处理有什么影响**

例如：事务引擎 通过PALF接口添加一个记录（record）

如果因为网络故障 产生新的选举，
新的领导 不知道 前任领导者新增的日志条目（appended log  
entries）是否提交，这些尚未确认的日志被称为“待定日志”(pending logs)。

PALF接口必须返回明确的执行结果，是成功 还是失败，事务引擎 根据返回判断该事务是commit or rollback。


2. **谁负责解决上个领导未处理的请求**

  自然是PALFl来解决（什么情况下无法解决呢 ）
  
>PALF guarantees that the transaction engine will be explicitly  
notified of replication results unless the leader crashes or the network is interrupted permanently

如果领导权转移到了另一个副本，
- 前任领导者会变成“待定跟随者”（pending follower）

- 当这个“待定跟随者”从新的领导者那里接收到日志时，这些待定日志的复制结果就会变得明确——要么被确认提交，要么被截断。

When the pending follower receives logs from the  
new leader, the replication results of pending logs become explicit  
(committed or truncated)

- 对于已提交的日志，系统会通过调用成功函数来通知事务引擎；

- 而对于被截断的日志，则会通过调用失败函数来通知复制失败。

因此，前任领导者必须转换为“待定跟随者”，等待新领导者的日志，
然后才能正式成为跟随者

3. **怎么解决，这个证明过程没看懂跳过**




原文：

~~~
Data synchronization tools (such as Change Data Capture) usually consume transactions in the order of logs; however, the LSN  
is incompetent for tracking the order of transactions because it  
is locally ordered within single PALF group. For scalability, data  
partitions are usually distributed among multiple Streams. If different transactions modify data partitions in different Streams, the  
LSNs of their logs are incomparable. To track the order of transactions with logs across PALF groups, a natural approach is to record  
commit versions of transactions in the payloads of log entries, as  
some systems have done [18, 42]. This approach does work, but it  
has disadvantages. For instance, commit versions may not strictly  
increase with LSN due to parallel executions of transactions (i.e.,  
assigning smaller LSN to logs of transactions with greater commit
~~~


原文

~~~
The OceanBase database provides globally meaningful commit  
versions to transactions using CSN. 

When a transaction is going to  
be committed, the transaction engine fetches a timestamp from a  
global timestamp oracle and appends the commit record with the  
timestamp as 𝑅𝑒𝑓 𝐶𝑆𝑁 . The 𝐶𝑆𝑁 returned by the append method  
tracks the order indicated by 𝑅𝑒𝑓 𝐶𝑆𝑁 and acts as the commit version of the transaction. 

Note that the 𝐶𝑆𝑁 is not generated by  
the global timestamp oracle, which may have a value greater than  
the current global timestamp.

As a result, the transaction may  
be invisible to a future read request that fetches a smaller readable version from the global timestamp oracle. 

To avoid this, the  
transaction engine will not respond to the client until the global  
timestamp is greater than the 𝐶𝑆𝑁 . Through cooperation between  
the transaction engine and PALF, CSN successfully tracks the order  
of transactions across PALF groups
~~~

## 课后作业 
1. Raft 与PALF在处理上一个任期的日志有什么差异
    4.1 Explicit Replication Results  给出pending logs 证明过程但是没看懂



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

重新意译和润色 和排版美化
{ 润色的结果 }

语法分析：
{ }

结合专业知识 给出自己理解 使用用Mermaid语法绘制流程图，并且是手绘风格
整体理解:
为了掌握可以给出三个题目进行练习？

```



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