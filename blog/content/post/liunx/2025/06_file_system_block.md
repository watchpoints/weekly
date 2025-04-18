---
title: 什么是块存储
date: 2025-03-31
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---



本文的写作宗旨是：绝不装逼，一定要简单，简单，再简单！
作为个人学习实践笔记，可能存在错误或遗漏，欢迎指正_
# 大纲

- 对象存储、文件存储和块存储的区别？有哪些具体案例

- 为了掌握块存储我需要了解哪些知识,如何掌握
- Linux存储IO栈
- Io 读写流程
- https://www.cnblogs.com/xiaolincoding/p/13719610.html
- # [Linux DirectIO机制分析](https://oenhan.com/ext3-fs-directio)



## 内容


### 对象存储、文件存储和块存储的区别？
- https://www.zhihu.com/question/21536660




**文件存储：**
- 特点是什么?
- 支持 标准的 POSIX（Portable Operating System Interface）接口。open、read、write、seek、close 访问文件
疑问来了：什么是POSIX
1. POSIX 标准包含了进程管理、文件管理、网络通信、线程和同步、信号处理等方面的功能。这些接口定义了函数、数据类型和常量等，为开发者提供了一个可移植的方法来与操作系统进行交互。
2. POSIX 接口的设计基于 Unix 操作系统的经验和理念。它通过定义一组通用的 API，将底层操作系统的功能封装起来，从而提供了与具体操作系统无关的高级功能。
3. 。在LinuxIO协议栈中，块设备位于VFS层之下，提供了基于块的读写能力
>画外音:c语言开发工程师都是核心稳定.

优点：便于扩展&共享；
缺点：**读写速度慢**


对象存储
- 支持的访问接口基本都是restful接口,对象存储不支持随机读取和写入
- 对象存储采用的则是扁平的组织方式,不是目录树的方式
- 块存储最明显的特征就是不能被操作系统直接读写，需要格式化为指定的文件系统（Ext3、Ext4、NTFS）后才可以访问
- S3 的全称 是 Simple Storage Service ，简单存储服务。它是文件系统/存储的一种简化替代：牺牲了强一致性、目录管理，访问时延等功能属性，以换取廉价的成本与海量伸缩的能力



#### **块存储**

**优点： 读写快（带宽&IOPS)
1、 这种方式的好处当然是因为通过了Raid与LVM等手段，对数据提供了保护。

2、 另外也可以将多块廉价的硬盘组合起来，成为一个大容量的逻辑盘对外提供服务，提高了容量。

3、 写入数据的时候，由于是多块磁盘组合出来的逻辑盘，所以几块磁盘可以并行写入的，提升了读写效率。



- 缺点：因为太底层了，不利于扩展( 高性能场景下优点大于缺点)
- 块存储最明显的特征就是不能被操作系统直接读写，需要格式化为指定的文件系统（Ext3、Ext4、NTFS）后才可以访问.
- 硬盘这样的块设备通常可以划分为多个逻辑块设备，也就是我们熟悉的硬盘分区（Partition）。反过来，单个介质的容量、性能有限，可以通过某些技术手段把多个物理块设备组合成一个逻辑块设备，例如各种级别的[RAID])，JBOD，某些操作系统的卷管理系统（Volume Manager）如Windows的动态磁盘、Linux的[LVM]等。
- LVM 是 Logical Volume Manager 的缩写，中文一般翻译为 "逻辑卷管理"，它是 Linux 下对磁盘分区进行管理的一种机制。LVM 是建立在磁盘分区和文件系统之间的一个逻辑层，系统管理员可以利用 LVM **在不重新对磁盘分区的情况下动态的调整分区的大小**
- https://zh.wikipedia.org/wiki/RAID



###  相关书籍

### 相关产品
1. 图解云硬盘
- https://support.huaweicloud.com/productdesc-evs/zh-cn_topic_0014580741.html
- https://support.huaweicloud.com/productdesc-evs/evs_01_0127.html

 2. ceph 提供服务
- **RBD**： RADOS block device，提供的块设备服务

#### 3.  FusionStorage 8.0.1 块存储产品简介

FusionStorage是一款可大规模横向扩展的存储产品，通过存储系统软件将服务器的本
地存储资源组织起来，构建全分布式存储池，通过**SCSI和iSCSI**接口向上层应用提供块

存储服务，满足云资源池及数据库等场景的存储需求。

OceanStor Pacific系列块服务支持的主存类型：SATA盘、SAS盘、NVMe SSD盘、SAS
SSD盘。
● 从IO性能看：NVMe SSD盘 > SAS SSD盘 > SAS盘 > SATA盘

### 4. 硬盘基础知识（HDD、SSD、IDE、PATA、SATA、SCSI、SAS）
https://www.cnblogs.com/xiaodoujiaohome/p/11732687.html

- **硬盘接口类型：有IDE=ATA<SATA,  SCSI<SAS  NVME(适用于SSd)  光纤通道硬盘接口**
- **硬盘材质类型：分为机械和固态:HDD SSD**
- **硬盘的使用方式：单块或者raid:raid用raid控制器代替磁盘控制器**
- **硬盘机柜网络：有FC网络的和普通网线的网络**
- **硬盘+服务器的存储架构：DAS=server+raid SAN=server--raid存储器   NAS=server+rais存储器+文件系统**



| 周次      | 学习主题                             | 主要内容与目标                                                                                                                                                                                                                                                                                                                                                                                                           | 参考资料                                                                                                                                                                                          | 产出／评估                             |
| ------- | -------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------- |
| **第1周** | **基础回顾：存储与网络**                   | - 操作系统文件系统与块概念复习<br/>- SCSI、多路径（MPIO/DM‑Multipath）原理<br/>- TCP/IP 与 iSCSI 协议概览                                                                                                                                                                                                                                                                                                                                    | - OSTEP《Storage》章节<br/>- ArchWiki Open‑iSCSI 入门 [wiki.archlinux](https://wiki.archlinux.org/title/Open-iSCSI?utm_source=chatgpt.com)                                                          | - 撰写一份 2 页总结报告                    |
| **第2周** | **iSCSI 负载均衡监控实战**               | - 在 Linux（`multipath -ll` + `iptraf`）与 VMware vSphere 中配置并监控 Round‑Robin<br/>- 分析多路径 I/O 分布                                                                                                                                                                                                                                                                                                                       | - Broadcom 社区 vSphere iSCSI 负载均衡指南 [community.broadcom](https://community.broadcom.com/vmware-cloud-foundation/discussion/iscsi-load-balancing?utm_source=chatgpt.com)<br/>- `multipath` 官方文档 | - 完成多路径监控脚本并展示 I/O 均衡报告           |
| **第3周** | **iSCSI Initiator/Target 开发与集成** | - 阅读并理解 Open‑iSCSI 源码架构<br/>- 使用 `iscsiadm` 调试会话<br/>- 探索 StarWind iSCSI Accelerator 源码与原理 [starwindsoftware](https://www.starwindsoftware.com/starwind-iscsi-accelerator?utm_source=chatgpt.com)                                                                                                                                                                                                                 | - GitHub open‑iscsi 项目 [github](https://github.com/open-iscsi/open-iscsi?utm_source=chatgpt.com)<br/>- StarWind Accelerator 设计文档                                                              | - 提交一段定制化 Initiator 模块代码示例        |
| **第4周** | **iSCSI 高级优化与项目对接**              | - 基于 DPDK/SPDK 的加速思路<br/>- 在 vSAN / Hyper‑V 环境中对接 iSCSI Target Service                                                                                                                                                                                                                                                                                                                                            | - StarWind Accelerator 实战案例 [starwindsoftware](https://www.starwindsoftware.com/resource-library/starwind-iscsi-accelerator-load-balancer/?utm_source=chatgpt.com)<br/>- vSAN iSCSI CLI 文档    | - 撰写对接方案文档并进行内部评审                 |
| **第5周** | **Ceph RBD 性能基准与分析**             | - 使用 Ceph 原生工具（`rbd bench`, `fio --ioengine=rbd`）做 4K/8K 随机读写测试 [docs.redhat](https://docs.redhat.com/en/documentation/red_hat_ceph_storage/1.3/html/administration_guide/benchmarking_performance?utm_source=chatgpt.com)[docs.redhat](https://docs.redhat.com/en/documentation/red_hat_ceph_storage/5/html/administration_guide/ceph-performance-benchmarking?utm_source=chatgpt.com)<br/>- 分析条带、缓存、PG 分布对性能的影响 | - Red Hat Ceph 基准测试章<br/>- Intel Ceph RBD 性能调优 [intel](https://www.intel.com/content/www/us/en/developer/articles/technical/performance-tuning-of-ceph-rbd.html?utm_source=chatgpt.com)       | - 输出一份性能对比报告（表格＋图表）               |
| **第6周** | **Ceph 集群健康与故障排查**               | - 使用 `ceph status`、`ceph osd df`、`ceph pg stat` 诊断集群健康 <br/>- 模拟 OSD 挂掉，验证 RBD 恢复流程                                                                                                                                                                                                                                                                                                                               | - Ceph 官方 HEALTH 文档<br/>- Rook Ceph 工具箱                                                                                                                                                       | - 完成故障演练报告与恢复演示视频                 |
| **第7周** | **CSI 驱动对接与监控**                  | - 部署 Ceph‑CSI 驱动到 Kubernetes（RBD/CephFS） [github](https://github.com/ceph/ceph-csi?utm_source=chatgpt.com)<br/>- 集成 Prometheus/Grafana 监控 CSI GRPC 与 liveness 探针 [rook](https://rook.io/docs/rook/v1.12/Storage-Configuration/Ceph-CSI/ceph-csi-drivers/?utm_source=chatgpt.com)                                                                                                                                  | - GitHub ceph-csi<br/>- Rook CSI 监控指南                                                                                                                                                         | - 提交 Kubernetes 示例清单与监控 Dashboard |
| **第8周** | **文档输出与培训演练**                    | - 汇总前 7 周核心内容，编写《iSCSI & RBD 全栈实践指南》<br/>- 设计培训 PPT，进行 30 分钟内部分享                                                                                                                                                                                                                                                                                                                                                  | - 参考 Cisco IPS FC/iSCSI 路由案例 [docs.ceph](https://docs.ceph.com/en/mimic/rbd/iscsi-initiator-win?utm_source=chatgpt.com)<br/>- Ceph CSI 运维手册                                                   | - 发布 PDF 文档＋培                     |
#### 性能
- https://www.cnblogs.com/bamanzi/p/linux-irq-and-cpu-affinity.html
- mpstat -P ALL 1 ，mpstat 是 Multi-Processor Statistics 的缩写，是实时监控工
mpstat 是一个常用的多核 CPU 性能分析工具，用来实时查看每个 CPU 的性能指标，以及所有 CPU 的平均指标。pidstat 是一个常用的进程性能分析工具，用来实时查看进程的 CPU、内存、I/O 以及上下文切换等性能指标

聊一聊被人误解的iowait
果系统处于 iowait 状态，那么必须满足以下两个条件：

1. 系统中存在等待 I/O 请求完成的进程。
irq（通常缩写为 hi），代表处理硬中断的 CPU 时间。4softirq（通常缩写为 si），代表处理软中断的 CPU 时间。

系统的软中断CPU使用率升高，我该怎么办
watch -d cat /proc/softirqs
TIMER（定时中断）、NET_RX（网络接收）、SCHED（内核调度）、RCU（RCU 锁）等这几个软中断都在不停变化。

sar -n DEV 1
**这个忙于处理中断的CPU都在处理哪个（些）中断？**

https://www.cnblogs.com/bamanzi/p/linux-irq-and-cpu-affinity.html

如果你发现网络流量上不去

- Redis 高负载下的中断优化
- - Ceph剖析1- RDB 块存储原理
- [Ceph IO, Rados IO 流程解析（读写）](https://www.cnblogs.com/wx-angel/p/13143403.html "发布于 2020-06-16 18:01")


### Linux存储IO栈--块


Linux 存储系统的 I/O 栈，由上到下分为三个层次，分别是文件系统层,通用块层和
设备层

- Linux存储IO栈梳理（一） -- 存储栈全貌图
-  Linux存储IO栈梳理（二） -- Linux内核存储栈流程和接口
- Linux存储IO栈梳理（三） -- eBPF和ftrace跟踪IO写流程
- Linux存储IO栈梳理（四） -- 通用块层
宋宝华：Linux文件读写（BIO）波澜壮阔的一生


**数据密集型应用系统设计》（DDIA
[《DDIA 逐章精读》](https://ddia.qtmuniao.com/)
--->Buffered IO`、`mmap(2)`、`Direct IO
---> write文件一个字节后何时发起写磁盘IO？

--->Linux 内核支持的几种 I/O 调度算法，分别为 `NONE`、`NOOP`、`CFQ` 以及 

- 分享一篇很棒的Linux IO栈讲解
https://www.qiyacloud.cn/about/
```text
int fd = open(fname, O_RDWR | O_DIRECT);
```
inux上传统的块设备层和IO调度器（如`cfq`）主要是针对`HDD`设计的。HDD设备的随机IO性能很差，吞吐量大约是几百IOPS，延迟在毫秒级（耗时可参考[之前文章](https://xiaodongq.github.io/2024/07/11/linux-storage-io-stack/)的耗时体感图和IOPS对比），所以当时IO性能的瓶颈在硬件，而不是内核。
物理设备层: 物理设备层有 HDD,SSD，Nvme 等磁盘设备。

 cat /sys/block/vda/queue/scheduler
[mq-deadline] kyber bfq none

从你提供的 `dmesg` 输出来看，系统正在加载和注册不同的 I/O 调度器（scheduler）。这些调度器是 Linux 内核中用于管理磁盘 I/O 请求的算法，它们的作用是优化数据的读写顺序，以提高磁盘的性能和吞吐量。

以下是输出中提到的调度器的简要说明：
```
[root@watchpoints ~]# dmesg | grep -i scheduler
[    0.036610] rcu: RCU calculated value of scheduler-enlistment delay is 100 jiffies.
[    0.589075] io scheduler mq-deadline registered
[    0.589077] io scheduler kyber registered
[    0.589104] io scheduler bfq registered

```
### 1. **`mq-deadline`**
   - **特点**：这是一个基于 `deadline` 算法的多队列（multi-queue）调度器。
   - **用途**：适用于需要保证 I/O 请求的截止时间（deadline）的场景，比如实时性要求较高的系统。
   - **工作原理**：它会根据请求的截止时间来调度 I/O 操作，确保请求不会被无限延迟。

### 2. **`kyber`**
   - **特点**：这是一个相对较新的调度器，专为现代存储设备（如 SSD 和 NVMe）设计。
   - **用途**：适用于高性能存储设备，能够充分利用这些设备的高吞吐量和低延迟特性。
   - **工作原理**：它通过动态调整队列深度和请求大小来优化性能。

### 3. **`bfq`（Budget Fair Queueing）**
   - **特点**：这是一个公平队列调度器，旨在提供公平的 I/O 带宽分配。
   - **用途**：适用于多用户或多任务的场景，确保每个进程都能获得合理的 I/O 资源。
   - **工作原理**：它会根据每个进程的预算（budget）来分配 I/O 请求，确保公平性。

### 4. **CPU 调度器**
   - 你提到的 `scheduler` 可能还包括 CPU 调度器，比如 Linux 内核中的 `CFS`（完全公平调度器）。
   - **CFS** 是默认的 CPU 调度器，它通过时间片分配来确保每个进程公平地使用 CPU 资源。

### 总结
调度器的选择取决于你的系统需求：
- 如果是桌面系统，`bfq` 或 `kyber` 可能更适合，因为它们能提供更好的响应性。
- 如果是服务器系统，`mq-deadline` 或 `kyber` 可能更适合，因为它们能优化吞吐量。

如果你需要更改调度器，可以通过以下命令查看和设置当前的调度器：
```bash
# 查看当前的调度器
cat /sys/block/sda/queue/scheduler

# 设置调度器（例如切换到 bfq）
echo bfq > /sys/block/sda/queue/scheduler
```

希望这些信息对你有帮助！如果还有其他问题，欢迎随时提问。

## 链接我 


坚持思考，方向比努力更重要。

关注公共号：后端开发成长指南  回复电子书 

如果更进一步交流 添加 微信：wang_cyi


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


我是小王同学，

希望帮你深入理解分布式存储系统3FS更进一步 ，
为了更容易理解设计背后原理，这里从一个真实面试场故事开始的。

### 阅读对象（也是我正在做事情）

#### **1. 目标：冲击大厂，拿百万年薪**

- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094551.png)


    
#### **2. 现状：缺乏实战，渴望提升动手能力**

- 公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
    
- 想通过阅读优秀的源码，提高代码能力，从“不会写”到“敢写”，提升C++编程自信。
    
- 需要掌握高效学习和实践的方法，弥补缺乏实战经验的短板。

####  3. 价值：成为优秀完成任务，成为团队、公司都认可的核心骨干。

**优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量**

 **错误示范**：
- 不少同学工作很忙，天天加班，做了很多公司的事情。
  但是 不是本团队事情，不是本部门事情，领导不认可，绩效不高
- 做低优先级的任务，无法利他，绩效不高
- 招进来最后变成了随时被裁掉的一些征兆
1.  刻意提高工作难度
2. 工作中不公平对待
3. 制造恶性竞争
4. 捧杀