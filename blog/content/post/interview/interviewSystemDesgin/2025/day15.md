---
title: 最佳实践：XSKY借助英特尔SPDK提升Ceph性能
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---

>知识地图: 分布式存储--Bluestore引擎

问：What SPDK can do to improve Ceph?

回答：
1. 请看 Bluestore案例分享-扬子夜-王豪迈.pdf分享 
2. ceph-day-beijing-spdk-for-ceph


- ​**​2016年Ceph中国社区年终盛典​**​  
    王豪迈作为演讲嘉宾，主题为《What’s new in Kraken》，重点探讨了Bluestore的设计（如BlueStore = Block + NewStore）、AsyncMessenger插件优化网络性能、Ceph-MGR模块扩展性提升等进展
-  XSKY CTO王豪迈获评Intel® Software Innovator
- 王豪迈目前是Ceph技术委员会成员，全球Ceph基金会]董事会成员。他还是全球Ceph社区主要贡献者之一，Ceph性能优化和网络模块项目的主要推进人。为了表彰他在Ceph社区K版本中所做出的努力，他被Ceph社区官方提名为杰出贡献者,
- XSKY也是英特尔官方认可的**Intel® Data Center Builders Program**计划成员，共同推进SDI（Software Defined Infrastructure）方案在全球客户的落地。
    ![](https://picx.zhimg.com/v2-a28d49616f72e0d4516c46686d9a03a3_1440w.jpg)


- 周雁波，Intel存储软件工程师，主要从事SPDK软件开发工作。


## 小思考：ceph 你了解，XSKY不了解 他么之间发生什么事情？

![ceph遇到什么问题，如何解决](https://s2.loli.net/2025/07/21/EW4V298jofi1wKF.png)

![](https://s2.loli.net/2025/07/21/3WpbtSmyiP5ZKxq.png)
![image.png](https://s2.loli.net/2025/07/21/XSKCGM1f8Zdeb9V.png)

![image.png](https://s2.loli.net/2025/07/21/UFoVzdprtf7w1hA.png)

![image.png](https://s2.loli.net/2025/07/21/VD6XJgHQ2nPuIUw.png)

## 查缺补漏1： DPDK 与SPDK，RDMA区别？

### 1.1 使用场景不同
*   **DPDK**是网络高性能领域的基石，适合需要处理海量网络流量的场景。
*   **SPDK**是存储性能革命的推动者，适合对延迟敏感的存储系统。
*   两者结合可构建全栈高性能基础设施（如基于NVMe-oF的远程存储网络）

| **特性**   | **DPDK**                         | **SPDK**               |
| -------- | -------------------------------- | ---------------------- |
| **核心领域** | 网络数据包处理                          | 存储I/O加速                |
| **主要技术** | 用户态网卡驱动、零拷贝                      | 用户态NVMe驱动、异步I/O        |
| **优化目标** | 高吞吐、低延迟的网络转发                     | 低延迟、高并发的存储访问           |
| **典型硬件** | 高速网卡（如Intel NICs、SmartNIC）       | NVMe SSD、持久内存（PMem）    |
| **协同使用** | 可用于SPDK的网络传输层（如NVMe-oF over TCP） | 依赖DPDK实现网络加速时（如远程存储访问） |
|          |                                  |                        |


RDMA（Remote Direct Memory Access）是一种网络传输技术，用于在远程主机之间直接访问内存。
### 1.2 SPDK

SPDK是一个框架而不是分布式系统，它的基石是用户态（user space）、轮询（polled-mode）、异步（asynchronous）和无锁的NVMe驱动

SPDK (Storage performance development kit) 是由Intel发起、
用于加速使用**NVMe SSD作为后端存储**的应用软件加速库。
- 官方网址：https://spdk.io/。SPDK重用了很多DPDK中的一些公共组件，比如内存管理，轮询方式的实现等，SPDK的核心是通过无锁，轮询的方式实现高效的用户态驱动集合
- SPDK一个重要的设计理念是**无锁化**，这样避免了多线程由于对锁的争抢而引入的性能开销。


### 1.3 DPDK
#### 产生背景

网络设备（路由器、交换机、媒体网关、SBC、PS网关等）需要在瞬间进行大量的报文收发，

因此在传统的网络设备上，往往能够看到专门的NP（Network Process）处理器，有的用[FPGA]，这些专用器件通过内置的硬件电路高效转发报文，

只有需要对报文进行深度处理的时候才需要CPU干涉。

>但在公有云、NFV等应用场景下，基础设施以CPU为运算核心，往往不具备专用的NP处理器，操作系统也以通用Linux为主，


由于包处理任务存在内核态与用户态的切换，以及多次的内存拷贝，系统消耗变大，以CPU为核心的系统存在很大的处理瓶颈。为了提升在通用服务器（COTS）的数据包处理效能，Intel推出了服务于IA（Intel Architecture）系统的DPDK技术。


- DPDK是Data Plane Development Kit的缩写。简单说，DPDK应用程序运行在操作系统的User Space，利用自身提供的数据面库进行收发包处理，绕过了Linux内核态协议栈，以提升报文处理效率


Linux 内核为支持用户空间驱动程序的开发提供了一个框架，名为 UIO（userspace I/O）。 这是一个通用的内核驱动程序，可以帮助开发人员编写能够访问设备寄存器和处理中断的用 户空间驱动程序。DPDK 也是 UIO 的使用者。

- DPDK的UIO驱动屏蔽了硬件发出中断，然后在用户态采用主动轮询的方式，这种模式被称为PMD（Poll Mode Driver）
### 1.4 RDMA 软件架构

RDMA 的软件架构按层次可分成两部分，即 rdma-core 和内核 RDMA 子系统，分别运行
在 Linux 系统中的用户态和内核态。整个软件架构适用于所有类型的 RDMA 网卡，不管网卡
执行了哪种 RDMA 协议（InfiniBand/RoCE/ iWARP）。
 - RDMA 数据传输
 - 发起数据传输——RDMA Write
 - rdma-core 中的示例程序 rping，用来发起一次 RDMA Write 数据
传输 https://github.com/linux-rdma/rdma-core/blob/master/librdmacm/examples/rping.c
- 应用程序在调用 ibv_post_send 发起 RDMA Write 操作后，接下来系统中会如何处理呢![](https://s2.loli.net/2025/07/21/b39w1ikDtn8jVl5.png)




## 二、实践案例 XSKY 借助英特尔SPDK 提升 Ceph 性能

XSKY（星辰天合）。作为英特尔在中国的首批SPDK合作伙伴之一，
XSKY率先将SPDK与Ceph用户态文件系统BlueFS整合，

Ceph作为开源软件定义存储的首选系统
在闪存环境下仍难觅突破性进展。
顺应业界趋势，将SPDK的支持在Ceph中实现势在必行。【不是从0开始的呀，解决比别人没有问题】

**BlueStore优化​**​：将SPDK集成至Ceph的存储引擎BlueStore，直接管理NVMe设备
![](http://mmbiz.qpic.cn/mmbiz/tJ5NYyM9MHANfLGBNygLCYgYFcSUwRE1pS6cagN5jZD2uEZcbanSClPlfpfpibSicjabhj00bPRS0UiaJCDiaWR0Zw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

SPDK对Ceph的BlueStore做了哪些具体优化？

![](https://s2.loli.net/2025/07/21/6tEpQhDbrSiZwma.png)

SPDK RBD bdev结合Ceph RBD，可以将image定义为SPDK的块设备类型。通过SPDK框架对块设备的管理，我们实现了用户态驱动，并利用轮询机制，替换了中断模型，降低了延迟，提升了对块设备操作的性能

####  ​**​RBD（块存储）加速​**​

- SPDK 提供 `bdev_rbd` 模块，将 Ceph RBD 镜像映射为 SPDK 块设备，供 iSCSI/NVMe-oF 等协议调用
    



###  参考
- 2022 SPDK线上论坛（二）| Ceph RBD浅析及性能调优
- SPDK对接Ceph性能优化
-  干货 | Ceph中国社区年终盛典之Ceph最新进展
-  Ceph IO路径和性能分析.pdf
### 最动人的作品，为自己而写，刚刚好打动别人



我在寻找一位积极上进的小伙伴，
一起参与**神奇早起 30 天改变人生计划**，发展个人事情，不妨 试试
1️⃣ 加入我的技术交流群Offer 来碗里 (回复“面经”获取），一起抱团取暖 
![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)
2️⃣ 关注公众号:后端开发成长指南(回复“面经”获取）获取过去我**全部面试录音**和大厂面试复盘攻略
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)
3️⃣ **感兴趣的读者可以通过公众号获取老王的联系方式。**

----------------我是黄金分割线-----------------------------

#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，打通任督二脉。
- 但是不要给自己这样假设：别人完成就等着自己完成了，这个逃避问题表现,裁员时候别人不会这么想。
##### 低头走路：
- 一次专注做好一个小事。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 ,把基本事情做好。













