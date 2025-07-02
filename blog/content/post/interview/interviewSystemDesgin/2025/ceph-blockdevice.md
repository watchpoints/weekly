---
title: 块设备
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---

## Q1:Ceph RBD（RADOS Block Device）与 osd 关系是什么？

ceph是一个统一存储，
ceph同时支持对象、块和文件三种形态
其中块设备形态，就是Rados Block Device，简称RBD

|项目|描述|
|---|---|
|**RBD（RADOS Block Device）**|一个将块设备抽象映射到 Ceph 集群上的接口。它对用户（如 QEMU、KVM、librbd 应用）暴露的是一个“虚拟磁盘”。|
|**OSD（Object Storage Daemon）**|Ceph 集群中负责**存储对象数据、处理读写请求、维护数据副本**的守护进程。是实际的数据承载者。|

         用户进程（QEMU, Docker, 或自定义应用）
                  │
              使用 librbd
                  │
         ========== RBD 层 ==========
                  │
         把块地址映射为对象（object）
                  │
            RADOS 协议读写对象
                  │
        多个 OSD Daemon 组成的对象存储池
         ┌────────────┬────────────┐
         │   OSD.1    │   OSD.2    │  ... N 个
         └────────────┴────────────┘


## Q2 为什么 **librbd** 使用的 RBD 块设备无法被 Linux 的 **page cache** 管理？



### ✅ 核心结论：

> 因为 `librbd` 是运行在 **用户态** 的库，它直接绕过了 **内核文件系统层** 和 **块设备驱动层**，
> 所以 Linux 内核的 page cache 根本“看不到”这些数据，自然就不能缓存和管理它们。

---

 🧱 情况一：常规 `mount` 文件系统

 举例

```bash
mount /dev/sda1 /mnt
cat /mnt/file
```

 I/O 路径：

```
用户进程
  ↓ sys_read()
内核 VFS 层
  ↓
ext4/xfs 文件系统
  ↓
页缓存（Page Cache）【！】
  ↓
块设备驱动
  ↓
硬盘（或虚拟磁盘）
```

🔍 **关键点：**

- 内核在文件系统层使用页缓存（page cache）来缓存文件数据；
- 所有 I/O 操作都在内核中完成，内核可以精准控制哪些页是脏的、什么时候刷回。

### 🧊 情况二：使用 `librbd` 映射块设备到用户态（如 QEMU 使用 RBD 后端）

#### 举例

```bash
rbd map my-image
qemu-img create -f raw rbd:pool/image
```

或者直接从 `librbd` 调用：

```c++
librbd::Image image;
image.read(offset, length, buffer);
```

#### I/O 路径：

```
用户进程（QEMU 或应用）
  ↓
librbd（用户态库）
  ↓
Ceph Messenger
  ↓
OSD
```

🔍 **关键点：**

- 数据从用户进程直接由 librbd 通过网络发往远程 OSD；
- **完全绕过内核文件系统层和块设备驱动**；
- 内核根本不知道你访问了哪个“文件”或“页”，所以没法缓存。
    
---

 🚫 为什么 page cache 管理不了？

因为：

1. **页缓存是内核的组件**，它在文件系统或块设备驱动中运作；
    
2. `librbd` 是用户空间库，它直接操作网络对象存储（Ceph OSD）；
    
3. 它访问的数据并不经过 Linux 内核 I/O 栈的 **page cache 探测点**；
    
4. 因此，Linux 根本没机会把它缓存进 page cache。
    

## Q3：`librbd` 写数据时还有文件系统吗？


**在 RBD 层面：没有文件系统**
- `librbd` 将块设备地址转换为一组对象（object name）；
- 这些对象通过 RADOS 协议发送到 Ceph 的 OSD；
- 它不涉及 POSIX 文件系统，不调用 `open()`、`write()`、`fsync()` 等文件 API；
- 所以，**RBD 是构建在对象存储之上的块设备抽象，而非构建在文件系统之上**

## Q4：`librbd` 写数据 数据写哪里去了？

回答：librbd → RADOS 对象 → OSD → BlueStore → 直接写入块设备

解释：
> **BlueStore 是 Ceph 自研的、绕过文件系统的对象存储引擎。**

- 它直接管理裸设备（块设备）；
    
- 使用自己实现的空间分配、校验、事务日志；
    
- 不再通过文件系统，而是直接用裸磁盘+KeyValue结构（rocksdb/btree）组织对象。

✅ 总结一句话
- librbd 自身不依赖文件系统，它通过 RADOS 协议将数据对象发往 OSD。
- 是否使用文件系统取决于 OSD 后端，现代 Ceph 默认的 BlueStore 完全绕过了文件系统，直接管理裸设备以提升性能。

## Q5:  问：为什么分布式块存储绕过文件系统？

答：
- POSIX 文件系统设计用于单机，不擅长高并发跨节点访问
- Ceph RADOS 就是这样：用户看到的是一个块设备，底层却是分布式对象


解释：

- **常规文件系统路径**：  
    应用 → VFS → ext4/xfs/btrfs → Page Cache → 块层 → 设备驱动 → 磁盘
    
- **RBD 块设备路径**：  
    应用（如 QEMU/librbd）→ librbd（用户态跳过内核）→ RBD 驱动 → Ceph OSD → 存储后端（无文件系统 or XFS）
## Q6 DMA（直接内存访问）能直接访问用户态缓存吗？在用户态缓冲区和磁盘设备


在 `libaio` 启用 ​**​`O_DIRECT` 模式​**​时，​**​
0 次 CPU 拷贝"​**​ 指的是数据在传输过程中 ​**​无需经过 CPU 参与的内存复制​**​，而是通过 ​**​DMA（直接内存访问）​**​ 直接在用户态缓冲区和磁盘设备之间传输数据。以下是具体原理和实现细节：

---

### ​**​1. 传统 I/O 的数据拷贝（对比理解）​**​

#### ​**​默认模式（带页面缓存）​**​

text

复制

`用户态缓冲区 →（CPU 拷贝）→ 内核页面缓存 →（CPU 拷贝）→ 磁盘控制器`

- ​**​拷贝次数​**​：2 次（用户态→内核态→磁盘）
- ​**​问题​**​：CPU 需要全程参与数据搬运，占用计算资源。

#### ​**​`O_DIRECT` 模式（0 CPU 拷贝）​**​

text

复制

`用户态缓冲区 ← DMA → 磁盘控制器`

- ​**​拷贝次数​**​：0 次（CPU 仅发起指令，不触碰数据）
- ​**​关键机制​**​：通过 DMA 控制器直接完成数据传输。

---

### ​**​2. 实现条件（DMA 零拷贝的核心要求）​**​

#### ​**​(1) 内存对齐​**​

- ​**​缓冲区地址​**​：必须按块设备扇区大小对齐（通常 4KB）。
    
    c
    
    复制
    
    `// 示例：posix_memalign 分配对齐内存 void *buf; posix_memalign(&buf, 4096, 4096); // 4KB 对齐`
    
- ​**​文件偏移​**​：必须是磁盘扇区大小的整数倍。

#### ​**​(2) 绕过页面缓存​**​

- 使用 `O_DIRECT` 标志打开文件：
    
    `int fd = open("file.data", O_RDWR | O_DIRECT);`
    

#### ​**​(3) DMA 控制器介入​**​

- 磁盘控制器通过 ​**​DMA 引擎​**​ 直接读写用户态内存，无需 CPU 逐字节搬运。

---

### ​**​3. 性能优势​**​

|​**​场景​**​|CPU 利用率|延迟|适用场景|
|---|---|---|---|
|传统 I/O|高|较高|通用文件操作|
|`O_DIRECT`|低|极低|数据库/高性能存储|

- ​**​实测数据​**​：在 NVMe SSD 上，`O_DIRECT` 可降低 ​**​30%~50%​**​ 的 I/O 延迟（因省去拷贝和上下文切换）。

---

### ​**​4. 底层原理（Linux 内核实现）​**​

当应用调用 `libaio` 的 `io_submit()` 时：

1. 内核检查用户缓冲区的物理内存是否连续且对齐（通过 `get_user_pages()`）。
2. 若对齐，则构造 ​**​DMA 描述符​**​，交由磁盘控制器直接访问用户内存。
3. 数据传输完成后，通过中断或轮询通知 CPU（无数据拷贝）



### Direct I/O 和 `io_submit()` 与 DMA 的关系

#### 1. `io_submit()` 提交直接 I/O 请求

当应用使用 `io_submit()` 提交一个带有 `O_DIRECT` 的 I/O 请求时，该请求绕过 page cache，由内核直接处理传输到用户缓冲区([scylladb.com](https://www.scylladb.com/2017/10/05/io-access-methods-scylla/?utm_source=chatgpt.com "Different I/O Access Methods for Linux, What We Chose for ScyllaDB ..."))。

#### 2. 内核为 DMA 做准备

内核通过 `get_user_pages()` 锁定用户缓冲区，并使用 `dma_map_*()` 将这些页面映射到设备可访问的总线地址空间，为 DMA 传输做好地址准备([docs.kernel.org](https://docs.kernel.org/core-api/dma-api-howto.html?utm_source=chatgpt.com "Dynamic DMA mapping Guide - The Linux Kernel documentation"))。

#### 3. DMA 传输后清理映射

设备完成数据传输产生中断后，内核调用 `dma_unmap_*()` 解除映射，并解锁页面，`io_getevents()` 通知应用 I/O 操作已完成([github.com](https://github.com/littledan/linux-aio?utm_source=chatgpt.com "How to use the Linux AIO feature - GitHub"))。

这三步概述了从提交请求到数据通过 DMA 写入用户空间并通知完成的完整流程。
# Q7  Sendfile with io_uring (almost)



### 参考
- https://rootw.github.io/2018/01/RBD-all/  # 【Rados Block Device】一、概述
- https://www.youtube.com/watch?v=XrBIOuGIcvA&ab_channel=DanielPersson # How to setup a Ceph RADOS Block Device

- 【Rados Block Device】四、Client内核RBD驱动分析－设备IO流程
- https://rootw.github.io/2018/01/RBD-client-3/
- dio https://www.scylladb.com/2017/10/05/io-access-methods-scylla/?utm_source=chatgpt.com

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
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，别人投入时间完成任务，别人采取措施解决问题了，不要不贪别人功劳，
- 但是不要给自己这样假设：别人完成就等着自己完成了，大家一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现， 
- 别人不这么假设，至少kpi上不会写成自己的，至少晋升不是你，裁员淘汰是，你的整个公司ceo，整个部门总裁，整个领导不帮助一下的，他们不这么想 ，你什么没做，战略是别人10年一点带你研究的多难，项目拆分别人10年完成多少问题，项目实现10年安排组织一点点完成多少bug，多少代码，是不要给自己这样假设：你等了看了观察10年什么做 ，0 贡献，
-  但是不要给自己这样假设，别人全部市场，别人全部市场，别人占据全部客户，一切重要无比，你太差，太才，思考不行，沟通不行，认知不行，去tmd，给别人丢脸。这个方面我无法控制，在这方面经历任何问题应该的。
- 我控制 的事情是 我必须亲自了解行业遇到难题，了解有什么需求，行业解决方案，我可以从三万英尺看问题，像周围人学习，像免费公开英文资料学习，从模仿开始。然后免费公开。我要通过技术拿到百万年薪P7职务，我必须糊涂混沌中走出来
-  目标：拿百万年 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 现状：缺乏实战，渴望提升动手能力公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 成为优秀完成任务，成为团队、公司都认可的核心骨干。优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量

##### 低头走路：
- 一次专注做好一个小事。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。
- 我控制 的事情是 我通过写自己代码拿到百万收益。代码就是杠杆，我必须创造可以运行在2c2g云主机小而美产品出来（服务普通人），而不是运行构建至少10台*64cpu* 300g内存物理机大而全项目（领航者，超越其他产品，出货全球N1，这个还是有停留有限斗争游戏，为top 10人企业服务）我必须糊涂混沌中走出来










