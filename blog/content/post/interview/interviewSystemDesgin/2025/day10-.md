---
title: 面试官:io_uring 相比Linux AIO性能提高5%,为什么成为革命性技术（v1.0）？
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---

各位老师好，这是大厂面试拆解——项目实战系列的第10篇文章。

最近投递简历石沉大海，这个时候更要冷静

**我问自己 面试成功第一性原理是什么？**
1. 自然是通过面试官 项目系统设计回答
2. 死磕系统设计题目，今天不搞明白，一家公司面试不通过，换另外一家公司同样不通过，下一次面试还是不通过。
3. 面试官都不更换题目这说明**行业类就一个问题**。
4.  第一性原理 就是自己真正搞懂，而不是考虑市场环境如何。

**我问自己 面试策略是什么？**
1. 如果不知道准备什么，先开枪，再瞄准，
2. 找1-2家公司面一次就知道，这个时候必须复盘总结。
3. 无数次面试经验告诉我，大部分90%人都 都会被同一个问题问到。为什么总结，忙工作，忙其他 干扰因素很多，甚至自己1个月后自己忘记了。
4. 把面试官当陪练 你越战越勇，平时根本没有和大神交流机会，这次就是。准备-交流，准备-交流 也是高效的学校方式。


- 游戏地图：深入理解计算机系统--IO模型

希望在io_uring使用场景上对你有帮助。

`io_uring` 已成为现代高性能开源项目的基石技术
尤其在存储、
数据库和
网络领域实现革命性突破。
**其设计兼顾效率与灵活性**，未来有望在更多场景替代传统 I/O 模型

# 一、面试回顾青铜被虐
## 岗位信息
- 时间：2025年 5月28 
- 岗位：后端开发工程师

![](https://s2.loli.net/2025/06/10/gIRXBhyr3pM5Co4.jpg)



##  对话内容


**面试官**：
在你简历提到使用 io_uring请问，
- 说说libaio和   io_uring有什么区别？
- 然后说一说 io_uring怎么使用的？

**我的感觉**：
- <font color="#245bdb">自己简历写内容，自己忘记了</font>，这妥妥犯了一个致命错误，别人揪出来，尴尬，面试官印象打折扣80%
- 我看W.Richard Steven << UNIX网络编程>> 和 陈硕老师的 << Linux多线程服务端编程：使用muduo C++网络库>>
- 提到 5种IO模型、阻塞IO和非阻塞IO、同步IO和异步IO。<font color="#245bdb">只研究到同步IO </font>（select、poll、epoll 、kqueue），大多数面试这个，**自己源码和数据结构还不和清楚呢**
- ![](https://s2.loli.net/2025/06/10/HgZyKweB45XqFdr.png)
- 纯异步IO **看过相关文章，看过几眼，现在忘记了**，<font color="#245bdb">没有实际操作</font>，当场卡克了，不知道怎么回答了。回答的没底气

**我回答**：把自己知道全部回答一次。

**面试官：提问，还有吗?等了20分钟 ，这个才是重点**



我:反复说之前内容。

>后记：
1. 最重要一点 减少用户态到内核态拷贝，一开始说了，这个没有深入讨论
2. 自己没有实际代码结合，开始想想更多 问题，例如epoll 可以监控多个，纯异步IO不能解决，判断IO出来完成 一点 纠结15分钟。
3. 还是那句话，在纯知识方面，你不会，没时间学习，面试官更没时间，更不会，**在不清楚情况下，试图说全部要点，导致 思路混乱**。

>小陷阱

1. 在日常工作中 和面试一样，领导反问，真的吗？你确定吗？还有其他吗？

2. 为什么不敢大声说，就这几点，而是习惯说 <font color="#245bdb">我回去在想想，别人认为根本考虑不周全，是糊弄，应付领导，这就是干活了，等白干</font>
    想什么，一个项目1周完成，就是它，简单还效率。
    在工作中 复杂设计绝对不通过评审的.
3. 一个简单事情 你知道，大家知道，但是经过**反复验证** 这个才是做事情，其他超出能力范围，责任范围 不是做事情。
4. 清楚 自己能做什么，把能做事情做好，不能做什么，不能做事情 找人解决，而不是自己解决，这个职场大忌，**有方案不找领导，专家讨论，就是最大问题，这个蛮干，瞎干，目中无人**
5. 需求是什么问清楚，方案是什么考虑清楚，然后找评审，**评审了 后面出问题领导不责怪，如果自己做，结果出问题了，就是自己问题。** 因为分工协作


# 二、查缺补漏王者归来



## 2.1 拆分任务 

✅ 步骤1： 动手写 io_uring一个 demo例子，最简单一个例子
1. 从 最简单完全体会背后设计原理，越简单越好，别把事情搞复杂了

✅ 步骤2 ：阅读材料
1. Understanding Modern Storage APIs: A systematic study of libaio, SPDK, and io_uring
2.  How io_uring and eBPF Will Revolutionize Programming in Linux, ScyllaDB, 2020
3. An Introduction to the io_uring Asynchronous I/O Framework, Oracle, 2020
✅ 步骤3 ：回答区别（不要试图完美回答，自己怎么理解就怎么回答）

✅ 步骤4：结合实际项目 验证 **你猜想**。不用怕打脸，及时调整，及时提问 


| ​**​项目​**​          | ​**​应用场景​**​ | ​**​具体实现方式​**​                                                                                             | ​**​性能收益​**​                          |
| ------------------- | ------------ | ---------------------------------------------------------------------------------------------------------- | ------------------------------------- |
| ​**​Ceph​**​        | 分布式存储系统      | ​**​BlueStore OSD​**​：直接集成 `io_uring` 优化块设备读写<br><br>​**​CrimsonOSD​**​：基于 `Seastar` 框架原生支持 `io_uring`<br> | 显著提升 OSD 吞吐量，降低延迟，适应高并发存储场景<br><br>   |
| ​**​TiDB (TiKV)​**​ | 分布式数据库存储层    | 通过 ​**​RocksDB​**​ 启用 `io_uring` 异步 I/O，用于 WAL 日志写入、SSTable 读取和 Compaction<br><br>                         | Compaction 写入时间减少 50%，整体 I/O 效率提升<br> |
| ​**​RocksDB​**​     | 单机 KV 存储引擎   | 官方实现 `PosixRandomAccessFile::MultiRead()` 支持 `io_uring`，优化批量读取<br>                                         | 随机读取和 Compaction 性能显著提升，尤其在高并发场景<br>  |
| ​**​SPDK​**​        | 用户态存储框架      | 深度整合 `io_uring`，实现零拷贝和异步 I/O 操作<br>                                                                        | 极致降低 I/O 延迟，适用于 NVMe 存储和虚拟化环境         |
| ​**​Netty​**​       | 高性能网络通信      | 通过 `netty-incubator-transport-io_uring` 模块支持异步 Socket I/O                                                  | 单 Worker 场景 QPS 提升 20%，长连接吞吐量优化       |
| ​**​Redis​**​       | 内存数据库        | 实验性支持 `io_uring` 作为事件循环后端（需内核配置）<br><br>                                                                   | 高负载下 QPS 提升 8%~32%（开启 SQ Polling 时）   |
| ​**​Nginx​**​       | Web 服务器      | 通过第三方模块实现静态文件异步读写<br><br>                                                                                  | 长连接场景 QPS 提升 20%，短连接性能波动              |
| ​**​xynet​**​       | C++ 高性能网络库   | 基于 `io_uring` 和 C++20 协程，实现异步网络操作<br>                                                                      | 减少动态内存分配和系统调用，适用于低延迟通信<br>            |
| ​**​PostgreSQL​**​  | 关系型数据库       | 社区探索异步 WAL 日志写入和索引刷盘（实验性）<br>                                                                              | 目标降低事务提交延迟，提升高并发 OLTP 性能              |
| ​**​QEMU​**​        | 虚拟化磁盘 I/O    | `virtio-blk` 后端驱动集成 `io_uring`，加速虚拟机磁盘访问                                                                   | 虚拟机 I/O 吞吐量提升，延迟降低<br>                |


### 2.1  看一段代码

```
https://github.com/axboe/liburing/tree/master/examples

io_uring-test 这个程序使用 4 个 SQE，从输入文件中读取最多 16KB 数据。

https://blog.csdn.net/baidu_15952103/article/details/109888362
struct iovec定义了一个向量元素
https://blog.csdn.net/winux/article/details/117590294

```



#### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题



如何提升存储系统的性能是一个对存储工程师们来说是永恒的大命题，解决这个问题并没有一击即中的银弹，IO性能的优化都在细节里。

- 今天我们来讲一讲性能和IO模型之间的关系。

我们先从本地磁盘的IO模型说起。
一方面，对本地磁盘来说，
传统机械磁盘HDD介质的IO性能比CPU指令和应用程序差了好几个数量级；

另一方面，新型的SATA SSD和NVMe SSD硬盘的性能大幅提升，
在硬盘内部，磁盘控制器芯片具有多个队列来处理并发的IO请求，
磁盘本身具备了更高程度并发的能力。



- 那么如何解决磁盘交互慢，以及利用新型磁盘内部特性，
提升数据访问性能，降低系统开销呢？
由此系统工程师们引入了多种IO模型来应对这些问题

在网络编程中【6】
我们通常使用epoll] IO多路复用来处理网络IO，
然而epoll也并不是异步网络IO，
仅仅是内核提供了IO复用机制，
epoll回调通知的是数据可以读取或者写入了，
**具体的读写操作仍然需要用户去做，而不是内核代替完成**

- 在存储IO栈中，做存储的同学大都使用过libaio，
然而那是一个巨难用啊Linux AIO这个奇葩。
首先只能在DIO下使用，用不了pagecache；
其次用户的数据地址空间起始地址和大小必须页大小对齐 


--

io_uring是Linux内核提供的一种新型异步I/O框架，旨在大幅提升系统处理I/O请求的效率和性能。liburing简化了这个复杂过程，让开发者可以更方便地利用这一先进技术。

####  二、 这个技术适用的场景。任何技术都有其适用的场景



### 翻译 How io_uring and eBPF Will Revolutionize Programming in Linux

https://thenewstack.io/how-io_uring-and-ebpf-will-revolutionize-programming-in-linux/

很多人可能还没意识到，Linux 内核在过去几年已经发生了一场革命。这场革命源于 **==两个激动人心的新接口的引入：eBPF 和 io_uring
我们认为，二者将会完全改变应用与内核交互的方式==**，
以及 应用开发者思考和看待内核的方式

But it’s a sure revolution, one that most people haven’t noticed yet. That’s because of two new, exciting interfaces: [eBPF](https://thenewstack.io/linux-technology-for-the-new-year-ebpf/) (or BPF for short) and [io_uring](https://github.com/torvalds/linux/tree/master/tools/io_uring), the latter added to Linux in 2019 and still in very active development. Those interfaces may look evolutionary, but they are revolutionary in the sense that they will — we bet — completely change the way applications work with and think about the Linux Kernel.

本文介绍 io_uring
 
 Linux I/O 系统调用演进
- 基于 fd 的阻塞式 I/O：`read()/write()`
-  非阻塞式 I/O：`select()/poll()/epoll()`

![单线程处理 ，一个io阻塞影响全部io](https://cdn.thenewstack.io/media/2020/04/91ed6951-costachart1.png)
- 1.3 线程池方式

 异步 IO（AIO）

前面提到，随着存储设备越来越快，主线程和 worker 线性之间的上下文切换开销占比越来越高。 现在市场上的一些设备，例如 [Intel Optane](https://pcper.com/2018/12/intels-optane-dc-persistent-memory-dimms-push-latency-closer-to-dram) ，**==延迟已经低到和上下文切换一个量级==**（微秒 `us`）。换个方式描述， 更能让我们感受到这种开销： **==上下文每切换一次，我们就少一次 dispatch I/O 的机会==**。

因此，Linux **==2.6==** 内核引入了异步 I/O（asynchronous I/O）接口， 方便起见，本文简写为 `linux-aio`。AIO **==原理==**是很简单的：

- 用户通过 `io_submit()` 提交 I/O 请求，
- 过一会再调用 `io_getevents()` 来检查哪些 events 已经 ready 了。
- 使程序员**==能编写完全异步的代码==**。

近期，[Linux AIO 甚至支持了](https://lwn.net/Articles/742978/) `epoll()`：也就是说 不仅能提交 storage I/O 请求，还能提交网络 I/O 请求。照这样发展下去，linux-aio **==似乎能成为一个王者==**。但由于它糟糕的演进之路，这个愿望几乎不可能实现了。 我们从 **==Linus 标志性的激烈言辞中就能略窥一斑==**：

_So I think this is ridiculously ugly.  
所以我认为这太丑陋了。_

Linux AIO 确实存在问题和限制：

- Linux-aio only works for O_DIRECT files, rendering it virtually useless for normal, non-database applications.  
    Linux-aio 仅适用于 O_DIRECT 文件，因此它对普通的非数据库应用程序几乎毫无用处。
- The interface is not designed to be extensible. Although it is possible — we did extend it — every new addition is complex.  
    该接口不是为可扩展的而设计的。尽管有可能 — 我们确实扩展了它 — 但每个新添加的内容都很复杂。
- Although the interface is technically non-blocking, [there are many reasons that can lead it to blocking](https://lwn.net/Articles/724198/), often in ways that are impossible to predict.  
    尽管该接口在技术上是非阻塞的，但有许多[原因可能导致它阻塞](https://lwn.net/Articles/724198/) ，而且通常是以无法预测的方式

 
####  **What Is io_uring?**


io_uring 来自资深内核开发者 Jens Axboe 的想法，
他在 Linux I/O stack 领域颇有研究。

随着设备越来越快， 
**中断驱动（interrupt-driven）模式效率已经低于轮询模式 （polling for completions**）
这也是高性能领域最常见的主题之一。

io_uring 的基本逻辑与 linux-aio 是类似的：
提供两个接口，
一个将 I/O 请求提交到内核，
一个从内核接收完成事件。 

但随着开发深入，它逐渐变成了一个完全不同的接口：
**设计者开始从源头思考 如何支持完全异步的操作**

What Is io_uring?
io_uring is the brainchild of Jens Axboe, 

a seasoned kernel developer who has been involved in the Linux I/O stack for a while. 

Mailing list archaeology tells us that this work started with a simple motivation: as devices get extremely fast, 

interrupt-driven work is no longer as efficient as polling for completions 

— a common theme that underlies the architecture of performance-oriented I/O systems.

But as the work evolved, it grew into a radically different interface, conceived from the ground up to allow fully asynchronous operation.

It’s a basic theory of operation is close to linux-aio: there is an interface to push work into the kernel, and another interface to retrieve completed work.


But there are some crucial differences:  
但是也有一些关键的区别

- By design, the interfaces are designed to be truly asynchronous. With the right set of flags, it will never initiate any work in the system call context itself and will just queue work. This guarantees that the application will never block.  
    根据设计，接口设计为真正的异步。使用正确的标志集，它永远不会在系统调用上下文本身中启动任何工作，而只会对工作进行排队。这保证了应用程序永远不会阻塞。
- It works with any kind of I/O: it doesn’t matter if they are cached files, direct-access files, or even blocking sockets. That is right: because of its async-by-design nature, there is no need for poll+read/write to deal with sockets. One submits a blocking read, and once it is ready it will show up in the completion ring.  
    它适用于任何类型的 I/O：无论它们是缓存文件、直接访问文件，甚至是阻塞套接字，都无关紧要。没错：由于其 async-by-design 性质，因此不需要 poll+read/write 来处理套接字。一个提交一个阻塞读取，一旦准备好，它就会显示在完成环中。
- It is flexible and extensible: new opcodes are being added at a rate that leads us to believe that indeed soon it will grow to re-implement every single Linux system call.  
    它是灵活且可扩展的：新作码的添加速度使我们相信，它确实很快就会发展到重新实现每一个 Linux 系统调用。

1. 由于设计上就是异步的（async-by-design nature），因无需 poll+read/write 来处理 sockets==**。 只需提交一个阻塞式读（blocking read），请求完成之后，就会出现在 completion ring。
    
2. **==灵活、可扩展==**：基于 `io_uring` 甚至能重写（re-implement）Linux 的每个系统调用。

d. io_uring 代码由 Jens Axboe 和 nvme 驱动维护者 Christoph Helwig 合作完成， 可以说是从立案阶段就有意识的与nvme驱动层紧密联动；与nvme高度匹配。


The application, whenever it wants to check whether work is ready or not, just looks at the cqe ring buffer and consumes entries if they are ready. There is no need to go to the kernel to consume those entries.


面向未来的Linux异步IO引擎：io-uring
Jens Axboe发布了一篇名为《Efficient IO with io_uring》文档，对io_uring进行了介绍。首先，他简述了Linux的IO发展历程，总结了当前Linux的原生异步IO接口（AIO）的局限，描述了io_uring易用、效率高等优势，并对io_uring的实现进行了概述。

io_uring 实例可工作在三种模式：

1. **==中断驱动模式==**（interrupt driven）
2. **==轮询模式==**（polled）
3. **==内核轮询模式==**（kernel polled）



#### 三、技术的组成部分和关键点

它从根本上改变了 Linux 应用程序的设计方式：它们不是在需要时发出系统调用的代码流，必须考虑文件是否准备就绪，而是自然而然地成为一个事件循环，不断向共享缓冲区添加内容，处理完成、冲洗、重复的先前条目。

It fundamentally changes the way Linux applications are to be designed: Instead of a flow of code that issues syscalls when needed, that have to think about whether or not a file is ready, they naturally become an event-loop that constantly add things to a shared buffer, deals with the previous entries that completed, rinse, repeat.  



### 五、对比其他怎么实现的？





    
### 参考
-  现代异步存储访问API探索：libaio、io_uring和SPDK
- https://cloud.tencent.com/developer/article/1748032
-  [译] Linux 异步 I/O 框架 io_uring：基本原理、程序示例与性能压测（2020）
- https://arthurchiao.art/blog/intro-to-io-uring-zh/
- https://stackoverflow.com/questions/13407542/is-there-really-no-asynchronous-block-i-o-on-linux
- 【6】 # 浅析开源项目之io_uring https://zhuanlan.zhihu.com/p/361955546
- 【7】 https://openinx.github.io/ppt/io-uring.pdf

--------------------——END--------------------------



### 我是谁



最动人的作品，为自己而写，刚刚好打动别人
刚刚好，是最难得的美好




![如果需要帮忙 call we wang_cyi](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，别人投入时间完成任务，别人采取措施解决问题了，不要不贪别人功劳，
- 但是不要给自己这样假设：别人完成就等着自己完成了，大家一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现， 
- 别人不这么假设，至少kpi上不会写成自己的，至少晋升不是你，裁员淘汰是，你的整个公司ceo，整个部门总裁，整个领导不帮助一下的，他们不这么想 ，你什么没做，战略是别人10年一点带你研究的多难，项目拆分别人10年完成多少问题，项目实现10年安排组织一点点完成多少bug，多少代码，是不要给自己这样假设：你等了看了观察10年什么做 ，0 贡献，
-  但是不要给自己这样假设，别人全部市场，别人全部市场，别人占据全部客户，一切重要无比，你太差，太才，思考不行，沟通不行，认知不行，去tmd，给别人丢脸。这个方面我无法控制，在这方面经历任何问题应该的。
- 我控制 的事情是 我必须亲自了解行业遇到难题，了解有什么需求，行业解决方案，我可以从三万英尺看问题，像周围人学习，像免费公开英文资料学习，从模仿开始。然后免费公开。我要通过技术拿到百万年薪P7职务，我必须糊涂混沌中走出来
-  目标：拿百万年 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 现状：缺乏实战，渴望提升动手能力公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 成为优秀完成任务，成为团队、公司都认可的核心骨干。优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量

##### 低头走路：
- 一次只讲清楚一个问题。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。
- 我控制 的事情是 我通过写自己代码拿到百万收益。代码就是杠杆，我必须创造可以运行在2c2g云主机小而美产品出来（服务普通人），而不是运行构建至少10台*64cpu* 300g内存物理机大而全项目（领航者，超越其他产品，出货全球N1，这个还是有停留有限斗争游戏，为top 10人企业服务）我必须糊涂混沌中走出来



如果您觉得阅读本文对您有帮助，
请点一下“**点赞，转发**” 按钮，
您的“**点赞，转发**” 将是我最大的写作动力！




## 


龙蜥白皮书精选：利用 io_uring 提升数据库系统性能
https://developer.aliyun.com/article/1225797

相比用户态框架SPDK，io_uring 可复用 Linux 内核的标准驱动，无需额外的用户态驱动开发，应用场景更通用，编程接口更友好

传统的 IO 软件栈已经无法完全释放出高性能存储设备的性能，高性能 IO 栈是当前存储领域重点研究的课题之一，代表性的如用户态方案 SPDK，以及标准的内核态方案 io_uring。

Linux 社区从零开始设计一种全新的异步 IO 框架 io_uring。io_uring 为了避免在提交和完成事件中的内存拷贝，设计了一对共享的 ring buffer 用于应用程序和内核之间的通信。该设计带来的好处有：

- 提交、完成请求时无需应用和内核之间的内存拷贝。
- 使用 SQPOLL 高级特性时，应用程序无需系统调用。
- 无锁操作，用 Memory Ordering 实现同步等。