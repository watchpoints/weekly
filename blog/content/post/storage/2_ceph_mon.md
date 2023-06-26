---
title: "日拱一卒：ceph-mon模块"
date: 2023-06-02
description: "you can"
draft: false
tags: ["Cephfs"]
categories: ["Cephfse"]
---



http://localhost:1313/post/storage/2_ceph_mon/





本文主要描述了 结合ceph mon模块，介绍Paxos基本实现。

大纲如下：

> 画外音：导入下面文章  https://www.chatpdf.com/ 提问
>

- [The Part-Time Parliament](https://www.microsoft.com/en-us/research/uploads/prod/2016/12/The-Part-Time-Parliament.pdf)
- [Paxos Made Simple](https://www.microsoft.com/en-us/research/publication/paxos-made-simple/)
- Revisiting the Paxos algorithm  
- https://www.read.seas.harvard.edu/~kohler/class/08w-dsi/chandra07paxos.pdf



先解作者背后故事

最初的相关论文 [The Part-Time Parliament](https://link.zhihu.com/?target=https%3A//lamport.azurewebsites.net/pubs/lamport-paxos.pdf) 发表于 1998 年后，很多人都表示理解不能。

于是 Lamport 在 2001 年，又使用相对简练的语言和逻辑，将其主干思想重新阐述了一遍，便有了 [Paxos made simple](https://link.zhihu.com/?target=https%3A//lamport.azurewebsites.net/pubs/paxos-simple.pdf)。

此后的1998年，Lynch和Lampson还合写了一篇文章“[Revisiting the Paxos algorithm](https://link.zhihu.com/?target=http%3A//groups.csail.mit.edu/tds/paxos.html)”

从那个时候开始Paxos才逐渐引起理论科学家们的关注，而真正为大众熟知应该是在Google发表[Chubby](https://link.zhihu.com/?target=http%3A//duanple.blog.163.com/blog/static/70971767201142412058672/%22%20%5Ct%20%22_blank)之后了。



Google在它的分布式系统中，大量使用了Paxos，比如Chubby、MegaStore、Spanner、Mesa等系统中。这篇文章详细讲述了Google在最初实现Paxos碰到的一系列问题及解决方案，是一篇全面讲解分布式系统工程实践的文章。其中提到的很多真实发生的场景，相信做过分布式系统的人都会感同身受，每个人或多或少都会遇到过一些类似的问题



然后看相关资料

- https://people.cs.rutgers.edu/~pxk/417/notes/paxos.html

- [Paxos三部曲之一] 使用Basic-Paxos协议的日志同步与恢复

- http://oceanbase.org.cn/?p=90

- [[Paxos三部曲之三\] Paxos成员组变更](http://oceanbase.org.cn/?p=160)

  http://oceanbase.org.cn/?cat=16

```bash


```

# Paxos



论文：https://sedna.cs.umd.edu/818/papers/paxosMadeSimple.pdf

阅读文档：https://zhuanlan.zhihu.com/p/408735316

https://ceph.io/geen-categorie/monitors-and-paxos-a-chat-with-joao/

 Prepare->promise->propose->accept->learn

1. 提议者（Proposer）

2. 接受者（Acceptor）

3. 学习者（Learner）

   

- 请为入门小白描述一下 什么是paxos，他解决了什么问题，然后如何解决的？

- 

Paxos是一种用于分布式系统中达成共识的算法，它的目的是让多个节点在没有中心化控制的情况下，就某个值达成一致。

在分布式系统中，由于网络延迟、节点故障等原因，节点之间的通信可能会出现问题，从而导致共识无法达成。

Paxos算法就是为了解决这个问题而设计的。

 Paxos算法的核心思想是通过多轮投票来达成共识。在每一轮投票中，节点会向其他节点发送请求，并根据其他节点的响应来决定下一步该做什么。

如果大多数节点都同意了某个值，那么这个值就被认为是达成共识的值。 Paxos算法解决了分布式系统中的共识问题，即如何让多个节点就某个值达成一致。它通过多轮投票来实现共识，并且可以容忍节点故障和网络延迟等问题。



Paxos算法的实现比较复杂，但是它已经被广泛应用于分布式系统中，例如Google的Chubby锁服务和Apache的ZooKeeper分布式协调服务等。 总之，Paxos算法是一种用于分布式系统中达成共识的算法，它解决了共识问题，并且可以容忍节点故障和网络延迟等问题





- Paxos如何工程实现？

- Paxos算法的工程实现可以分为以下几个步骤： 1. 设计系统架构：首先，需要设计分布式系统的整体架构，确定系统中的角色和节点数量。通常，Paxos算法中会有提议者（Proposer）、接受者（Acceptor）和学习者（Learner）等角色。 2. 实现网络通信：为了让节点之间能够进行通信，需要实现网络通信模块。这个模块负责节点之间的消息传递，包括发送和接收消息。 3. 实现提议者角色：提议者负责发起提议并推动共识的达成。在实现提议者角色时，需要实现以下功能：   - 生成唯一的提议编号；   - 发送prepare请求给接受者，并处理接受者的promise响应；   - 根据接受者的promise响应，决定是否发送accept请求；   - 处理接受者的accept响应，判断是否达成共识。 4. 实现接受者角色：接受者负责接收提议者的请求，并根据算法规则进行响应。在实现接受者角色时，需要实现以下功能：   - 接收并处理来自提议者的prepare请求；   - 根据当前状态和提议者的请求，返回promise响应；   - 接收并处理来自提议者的accept请求；   - 根据当前状态和提议者的请求，返回accept响应。 5. 实现学习者角色：学习者负责接收已达成共识的值，并将其应用到系统中。在实现学习者角色时，需要实现以下功能：   - 接收并处理来自接受者的已达成共识的值；   - 将共识的值应用到系统中，以完成相应的操作

- 







# 流程分析



### 涉及文件

- D:\db\note_ceph\src\mon\PaxosService.h

- 

  ~~~c#
  
  /**
   * A Paxos Service is an abstraction that easily allows one to obtain an
   * association between a Monitor and a Paxos class, in order to implement any
   * service.
   */
  class PaxosService 
  ~~~



主要参考

- [Ceph Monitor PaxosService](https://blog.wjin.org/posts/ceph-monitor-paxosservice.html)

- [Ceph Monitor Paxos](https://blog.wjin.org/posts/ceph-monitor-paxos.html)

- [Ceph Monitor Leader Elect](https://blog.wjin.org/posts/ceph-monitor-leader-elect.html)

- [Ceph Monitor Overview](https://blog.wjin.org/posts/ceph-monitor-overview.html)

# [Ceph Monitor Overview](https://blog.wjin.org/posts/ceph-monitor-overview.html)

- Introduction

monitor在ceph集群中起着非常关键的作用，它维护着几张map(monmap, osdmap, pgmap等)， 通过paxos算法保证数据的一致性。

 monitor节点信息会存放在monmap这张表中,其中存放的就是rank值，在选举leader的时候，rank值最小的获胜，所以monitor地位并不是平等的，这样做的目的可能是为了快速的选举出leader。



monitor维护的map，都是以PaxosService的服务提供，不同服务继承基类PaxosService实现自己的特性，这些服务通过paxos算法对数据进行更新， 

> 只有leader才可以调用propose相关函数进行更新，如果peon节点收到更新的消息，则需要将消息转发给leader节点， 所以同一时刻paxos算法只会存在一个proposer，几乎没有竞争，决议会很快完成，更新也是非常迅速的。





![](https://blog.wjin.org/assets/img/post/ceph_mon_startup.png)

- https://blog.wjin.org/posts/ceph-monitor-startup.html
- https://blog.wjin.org/posts/ceph-monitor-overview.html



# Main Thread

- 主线程的初始化工作，参见代码ceph_mon.cc:



Data Store

monitor维护了很多map以及自身Elector和Paxos算法的数据，这些数据肯定是需要地方存储的，最开始的时候monitor采用文件存储，后来采用k/v存储， 主要是利用k/v的原子操作以及对key做有序排列，目前支持levelDB和rocksDB。



主要实现是在文件MonitorDBStore.h中，将对key的操作封装成一个op， 然后考虑到同时对多个key操作的时候，需要确保事务性，所以使用的时候，都是以transaction的形式提交，一个transaction可能包含多个op。





# [Ceph Monitor Paxos](https://blog.wjin.org/posts/ceph-monitor-paxos.html)







# Introduction

paxos算法主要用来解决分布式系统中的数据一致性，ceph monitor中实现了paxos算法，然后抽象出了PaxosService基类，基于此实现了不同的服务， 比如MonmapMonitor, OSDMonitor, PGMonitor等，分别对应monmap, osdmap, pgmap。

paxos需要根据monitor状态来做转换，大致如下:

- monitor启动的时候，preinit会调用函数init_paxos初始化paxos
- monitor进入bootstrap，准备重新选举的时候，会restart paxos
- monitor选举成功，成为leader的时候，会将paxos初始化leader
- monitor选举失败，成为peon的时候，会将paxos初始化为peon
- monitor运行过程中，leader上的PaxosService会提议一些值，进行paxos决议，即propose



![img](https://blog.wjin.org/assets/img/post/ceph_mon_paxos_3.png)



# [Ceph Monitor PaxosService](https://blog.wjin.org/posts/ceph-monitor-paxosservice.html)



1. PaxosService是一个虚基类，内部利用Paxos类的功能，包装了一些接口，即提供一些模板方法，用来构建基于paxos的服务。 目前所有服务如下图所示:

![](https://blog.wjin.org/assets/img/post/ceph_mon_paxosservice.png)

如果考虑需要实现自己的一个能够利用paxos的服务，应该从何入手？大致应该考虑如下几个方面:

- Init
- Restart
- Process
- Update
- Active

![](https://blog.wjin.org/assets/img/post/ceph_mon_sequence.png)



## [ceph monitor----初始化和选举](https://www.cnblogs.com/yi-mu-xi/p/10364797.html)

# Ceph Monitor源码机制分析（三）—— 选举

https://blog.csdn.net/scaleqiao/article/details/52315468



![](https://img-blog.csdn.net/20160825155948415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

选举的入口函数是Monitor::start_election



这么说还是有点笼统，让我们用代码说话吧，当然你得学会看代码。选举的入口函数是Monitor::start_election()，查一下调用这个函数的代码，不难看出会在以下三种情况发生时，调用它：

- Monitor::handle_probe_reply()， monitor进行bootstrap时，首先会向monmap中所有的成员发送MMonProbe消息，然后在收到peer返回的probereply时，会根据返回的quorum信息以及paxos版本来判定是否需要发起选举。

- Elector::handle_propose()，这个是在收到别的monitor发过来的选举请求消息时，会根据情况触发重新选举。

- Monitor::do_admin_command()和Monitor::handle_command()，

​     这两个属于通过ceph命令或者mon的admin socket执行quorumenter和quorum exit触发的选举操作。



> 旁白：【ceph】Admin Socket机制|ceph dump 命令原理

- https://blog.csdn.net/bandaoyu/article/details/123070446

- ceph daemon /var/run/ceph/ceph-mon.*.asok help

- CephContext中会创建一个AdminSocket对象，该对象本质是一个线程。

​      ceph-mon/ceph-osd/ceph-mds这些进程都会有创建一个AdminSocket的线程，负责响应用户的探查命令。





————————————————
版权声明：本文为CSDN博主「瞧见风」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/scaleqiao/article/details/52315468

![](https://img-blog.csdn.net/20160825160932814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





## 代码分析

##  文件：MDSMonitor.cc







##  他山之石

- https://monica.im/s/959082ae
