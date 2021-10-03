#Go# #源码学习#



Go Etcd 号称云原生的基石，奇伢专门开了一个专题从 Etcd 源码角度来学习这个项目。持续更新中，此篇为汇总： 从 raftexample 开始 1、[Go Etcd 源码学习【1】开篇](https://t.zsxq.com/jq7AyzB)  ✅

2、[Go Etcd 源码学习【2】编译看看](https://t.zsxq.com/qbQz3zv) 

3 [Go Etcd 源码学习【3】raftexample 学习](https://t.zsxq.com/BE6YBQJ) 









## 内容：



#### Go Etcd 源码学习【1】

开篇 代码仓库 奇伢拉了个仓库出来，用于阅读 etcd 的代码，版本是 v3.4.10 。 [https://github.com/liqingqiya/readcode-etcd-v3.4.1...](https://github.com/liqingqiya/readcode-etcd-v3.4.10) 

我会不断的在这个仓库里加注释。 

etcd etcd 说白了就是是 kv 存储，只不过这个 kv 存储是分布式的，数据是多副本冗余的，kv 是有多版本功能的。



https://github.com/wangcy6/readcode-etcd-v3.4.10

Cd



# Go Etcd 源码学习【2】编译看看



整个编译看看（可以把一些依赖包下载下来）：

git clone git@github.com:wangcy6/readcode-etcd-v3.4.10.git

go mod vendor



然后，我们就去编译 raftexample 这个程序。

![img](https://article-images.zsxq.com/Fmwhe4MnXCc3L6NijFnsr3kTb46Z)

目录下出现 `raftexample` 的二进制就代表你编译成功了，也就具有了学习的素材。

安装 README 的提示，我们用 goreman 来启动程序：

![img](https://article-images.zsxq.com/FnSzwcq_PCwR5dYo8sWoLTkKgMpy)

`goreman` 读的是当前目录的 Procfile 文件，这个文件是这样写的：

![img](https://article-images.zsxq.com/FvuOWEcyzazTP5nsQY6R0HZ19rRs)

goreman 根据 Procfile 的配置启动 3 个 raftexample 进程，分别使用了不同的端口，这样就省去了我们人为的键入，方便一些。

`goreman start` 执行，你就会发现拉起了 3 个进程，这组成了一个最简单的 raft 集群。

![img](https://article-images.zsxq.com/FjzvYh_ZVatiiP9rmFAOHVGgM1PD)



好啦，原材料准备好了，接下来，我们就调试+阅读这个 raftexample 程序的代码，来理解 raft 是怎么回事，敬请期待。







```
https://github.com/wangcy6/readcode-etcd-v3.4.10/tree/master/src/go.etcd.io/etcd/contrib

https://github.com/wangcy6/readcode-etcd-v3.4.10/tree/master/src/go.etcd.io/etcd/contrib/raftexample
goreman start
curl -L http://127.0.0.1:12380/my-key -XPUT -d foo

```





# Go Etcd 源码学习【3】raftexample 学习

https://articles.zsxq.com/id_od6o03o5ttpk.html

![img](https://article-images.zsxq.com/FtpK5XIoNbzzE2RUpd4Om6-Xe-9W)

##  

## api 层



从入口 api 层说起，入口文件在 contrib/raftexample/httpapi.go

核心结构：

![img](https://article-images.zsxq.com/FhNHzaMLUxY4dvAhdHhvDzsu-JL4)



它既然是 api 接口，那么肯定对接外面的请求，协议是 http 协议，那么必定实现 ServeHTTP 的接口：

![img](https://article-images.zsxq.com/Fq-y-6SlcaHjsVBPgFzg3cTX9wUs)



- 写入是需要走 raft 状态机，这里是异步的，递交了没等返回；
- 读这里是直接通过 service 提供的接口进行读，读内存即可（这里没有满足线性一致性）；
- 扩容和下线自然也是要走 raft 状态机的，这里是异步的；



这就是 api 层的全部内容了，api 层把请求交给了 service 的模块，service 业务层的模块也即是 kvstore 的封装，这个是 kv 业务的实现。



## service 层



下面看业务层的实现，业务层实现的是一个 kv 存储。在 contrib/raftexample/kvstore.go 文件。核心结构是 kvstore ：

![img](https://article-images.zsxq.com/FjDJ_jbaFNtf-WmG0QYe7tKSFdFu)



不出意料，核心就是一个 map ，还有一个定制 snap 的函数。对于 kv 主要是读写两个接口：



- Lookup ：从 map 中查询 key/value；
- Propose ：把写请求递交给底层，走 raft 状态机，达成分布式一致；



另外业务层对于 snap 定制了两个函数:

- getSnapshot ：创建一个节点的 snap；
- recoverFromSnapshot ：从 snap 中读取，并恢复节点数据；



另外还有个关键函数：

- readCommits ：处理哪些已经 commit 过，但是还未 apply 的信息。而在这个简单的例子中，apply 就是对 kv map 进行值得写入而已；



所以，我们看到业务层究竟做了哪些事情：



\1. 实现自己业务的语义，比如这里就是一个 kv；

\2. 使用 Propose 递交消息到底层；

\3. 读取从底层抛上来的已经 commit 的消息，进行 apply （业务的设置）；

\4. 实现一个创建快照，恢复快照的方法；



换句话说，业务只理解业务，跟底层打交道的另有其人。业务层只往下丢（propose channel），也只处理上面抛上来的（commit channel）。



**那么跟业务层打交道的是谁？**



关键要看谁去取 propose channel，谁去写 commit channel ？上下两个模块用这两个 channel 解耦了。

在 contrib/raftexample/main.go 里：

![img](https://article-images.zsxq.com/FofRWijo8aJfTr_qkRj__yQYm0xf)



所以答案就是：raftNode 这个由 raftexample 实现的模块，作为承上启下的作用。承接 service 模块，启开最底层的 raft 状态机模块。



## raftNode 层



raftNode 由 raftexample 实现，和 raft 状态机解耦开（ etcd 也实现了自己的 raftNode ）。



- 承上：读 proposeC，写 commitC ；
- 启下：怎么把请求合理的经过 raft 状态机，这个是重点工作；



来看一下核心结构：

![img](https://article-images.zsxq.com/FhPRpuhEJPRj3OXVwQTiLaf9xHWu)



这个结构体字段很多，raftNode 本来就是一个中间的组合协调的角色。大概可以划分下功能：



- 其中 proposeC，confChangeC，commitC，errorC 这几个 channel 是和业务层通信打交道的；
- raft.Node 这个字段是最核心的 raft 状态机的实现；
- 另外还组装了 wal.WAL，raft.MemoryStorage 这两个核心实现；



raftNode 实例由函数 newRaftNode 创建，结构体初始化之后，调用 rc.startRaft( ) 进行初始化。

![img](https://article-images.zsxq.com/Fv7iN2GQm_qor2_sLOFFrnBAGPYz)

##  

## raft machine 层





rraft.Node 模块，实现都在 raft/node.go 文件中。另外不得不提的是 raft.MemoryStorage 模块还有 wal.WAL 模块，这两个是和状态机配合使用的。



# Go Etcd 源码学习【4】raftexample 中 Storage 是什么

https://articles.zsxq.com/id_vzi5tr44re0f.html

raft.Node 是纯状态机的实现。

raft.MemoryStorage 是存储的实现。

wal.WAL 是持久化 log 的实现。

这三个是业务 raftNode 组装功能的核心，今天专门学习下 Storage 这个重要组件。



## Storage



这是一个重要的 interface ，是抽象出来，用于检索 log entries 的存储器。

Storage 任何方法的报错，都会导致 raft 实例停服，并且拒绝参加选举。业务应用来负责清理和恢复这个场景。

![img](https://article-images.zsxq.com/FpgAx-xEOcRcVaYSpJouecnxQGG7)



而 raft.MemoryStorage 是 Storage interface 的一种实现而已。在 raftexmaple 中，直接使用了 raft.MemoryStorage 作为 Storage 存储器 ，当然 Storage 也可以由业务自己实现，只要实现对应的接口，从底层存储介质中取回日志对象（log entries）。



### raft.MemoryStorage



这个是 Storage 的具体实现，我们看下这个实现的方法，借以感受下这个的作用。

来看下 MemoryStorage 的结构：

![img](https://article-images.zsxq.com/FvdB27U8fC1QF8x-VijhzFM5R4sv)



能看到，最关键的其实就是一个 Entry 数组。



#### InitialState

![img](https://article-images.zsxq.com/FsSOGLoBHgVIFv3Kl9elarC65xL6)



而这个 ms.hardState 则是在 raftexample/raft.go 文件，raftNode.replayWAL 函数中，调用 SetHardState 方法进行赋值。

![img](https://article-images.zsxq.com/Fg5D80hkbE2A3HPWmwlb2nQg7zMC)



#### Entries



这个方法用于获取 [ lo, hi ] 这一个区间的日志记录。

![img](https://article-images.zsxq.com/FjGUOPjgJw5RNYZ55KIzwrOKEcuA)



那么对于 MemoryStorage 来说，ms.ents 又是哪里来的呢？

对于 raftexample 来说，是在 raftNode.replayWAL 中，调用 Append 置入的。

![img](https://article-images.zsxq.com/FrHQiEKcP-yoRQkQH0ReDwM7Ny0o)



#### Term

![img](https://article-images.zsxq.com/FjCewanHcB4dCiqAWi287XwEJLB2)



可以看到，这个就是获取到指定位置的 log entry ，需要处理截断和越界的两种情况。



#### LastIndex



返回最后一个 日志的 index 。

![img](https://article-images.zsxq.com/FldqhxZfF507RDlPM4y_Dachuanj)



思考：有这种计算方式，是不是代表 index 一定是连续的？



#### FirstIndex



返回第一个日志的 index 。

![img](https://article-images.zsxq.com/FjHAJwJybkS8OouoCmVNtqAX-k3H)



#### Snapshot



![img](https://article-images.zsxq.com/Fjt4rYgq_ZKJW6ZGdqMEk3i5mWlt)



这个主要是返回 MemoryStorage 的 snapshot ，而这个 snapshot 也是在 replayWAL 里面设置上的。

![img](https://article-images.zsxq.com/Fgl25MCSUB-pje9IB23zR4fKKa9z)



#### 小结下



小结下 MemoryStorage 的功能，这是一个日志全在内存里，实现了日志索引的功能的一个存储。

Storage 是给 raft 状态机用的，是 raft 状态机内部的 raftlog 。



我们知道 raft 算法中，log 是最重要的核心之一。raft 算法三大核心：

\1. leader 选举；

\2. 日志复制；

\3. 选举的正确性保证；



这个日志在 etcd 的 raft 核心模块里则非常巧妙的抽象了出来。



etcd 把 raft 算法和 log 完全拆开，日志抽象成了一个 Storage 的接口，而算法则只对这几个特定语义的接口进行逻辑判断，合力工作则能保证数据的一致性。



这样保证了 raft 算法的最小化，有可以让业务自定义日志的具体实现，很灵活。

Ready 里面的 commit entries 将从这个里面出来。

对于 raft 状态机来讲，我们就可以认为 Storage 的数据是已经持久化了的（这个也是业务要保证的语义），那么在 raft 算法内部就可以完全按照一致性算法来判断。



同学们可能会疑惑？



Storage 如果是 MemoryStorage 的话，明明是个内存数据，怎么说是持久化了的呢？业务怎么来保证？



其实在 etcd 中实现很简单，所有 log entries 添加到 Storage 之前，一定要走 wal 先持久化。





# 网络文章





[etcd源码阅读与分析（一）：raftexample](https://jiajunhuang.com/articles/2018_11_20-etcd_source_code_analysis_raftexample.md.html)

tree -d -L 1 .



- [etcd源码阅读与分析（一）：raftexample](https://jiajunhuang.com/articles/2018_11_20-etcd_source_code_analysis_raftexample.md.html)
- [etcd源码阅读与分析（二）：raft](https://jiajunhuang.com/articles/2018_11_22-etcd_source_code_analysis_raft.md.html)
- [etcd源码阅读与分析（三）：wal](https://jiajunhuang.com/articles/2018_11_24-etcd_source_code_analysis_wal.md.html)
- [etcd源码阅读与分析（四）：lease](https://jiajunhuang.com/articles/2018_11_27-etcd_source_code_analysis_lease.md.html)
- [etcd源码阅读与分析（五）：mvcc](https://jiajunhuang.com/articles/2018_11_28-etcd_source_code_analysis_mvvc.md.html)





# [etcd学习(6)-etcd实现raft源码解读](https://www.cnblogs.com/ricklz/p/15155095.html)



# ETCD 源码分析 #38

https://github.com/loadlj/blog/issues/38

