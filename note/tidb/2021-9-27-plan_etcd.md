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





# 网络文章





[etcd源码阅读与分析（一）：raftexample](https://jiajunhuang.com/articles/2018_11_20-etcd_source_code_analysis_raftexample.md.html)

tree -d -L 1 .



- [etcd源码阅读与分析（一）：raftexample](https://jiajunhuang.com/articles/2018_11_20-etcd_source_code_analysis_raftexample.md.html)
- [etcd源码阅读与分析（二）：raft](https://jiajunhuang.com/articles/2018_11_22-etcd_source_code_analysis_raft.md.html)
- [etcd源码阅读与分析（三）：wal](https://jiajunhuang.com/articles/2018_11_24-etcd_source_code_analysis_wal.md.html)
- [etcd源码阅读与分析（四）：lease](https://jiajunhuang.com/articles/2018_11_27-etcd_source_code_analysis_lease.md.html)
- [etcd源码阅读与分析（五）：mvcc](https://jiajunhuang.com/articles/2018_11_28-etcd_source_code_analysis_mvvc.md.html)