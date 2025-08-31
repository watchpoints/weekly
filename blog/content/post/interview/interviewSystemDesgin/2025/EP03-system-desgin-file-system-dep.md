---
title: 从零开发分布式文件系统(三) :国产文件系统JuiceFS｜沧海｜3FS 能相互替代吗?百万 OPS如何满足（1）(ceph 默认 5 千)
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
#  一、周刊目标

- 不是让你成为**C++专家**， 而是让你成为**C++面试专家**。
- 不是让你疯狂学习新知识， 而是帮你**重新整理已有知识

书接上回：

[从零开发分布式文件系统(一) :100G读写带宽,百万IO请求文件系统怎么实现的](https://mp.weixin.qq.com/s/QdhOqdcIhuVStjfB3QgP5A)


## 1.1 **​上期回顾​**​：关键词——计算与存储分离

- Linux 默认的 Ext4 文件系统是​**​计算与存储紧密耦合​**​的经典范式，在普通应用场景中完全够用。
- ​**​耦合体现在哪里​**​：  
    采用​**​物理共生、资源共享​**​的方式，计算资源（CPU、内存）和存储资源（磁盘）在物理和逻辑上都被捆绑在同一台服务器中，共享同一套电源、主板和 I/O 通道（如 PCIe）。
- ​**​优势​**​：（在延迟方面 分布式比不过单机）
    计算单元可通过操作系统内核（经 VFS → Ext4 驱动）​**​直接访问​**​本地块设备（如 `/dev/sdaX`），路径最短，延迟极低（纳秒/微秒级）。
- ​**​劣势​**​：  
    ​**​计算密集型任务​**​与 ​**​I/O 密集型任务​**​会相互竞争资源，彼此影响性能。
- GPFS 和 Lustre 从设计之初，就致力于​**​将计算与存储强制解耦​**​，使它们可分布在不同的、通过网络互联的节点池中。

---

> 画外音：
> 1. 并不是说一提到“分布式”就显得更高大上——对绝大多数普通开发工程师而言，真正要深耕的依然是单节点性能。
> 2. 比如理解 Linux Ext4 文件系统，吃透系统调用和 I/O 栈的过程，往往更为重要。



 
## 1.2 **本期任务​**​：差异化设计遇到挑战

- 从零开发分布式文件系统（三）：JuiceFS｜沧海｜3FS 架构差异对比
    
- **小思考**1：诸如 DDN 公司的 Lustre 和 IBM 的 GPFS 这类系统，虽在架构上实现了计算与存储分离，但其计算节点和存储节点仍处于同一集群内部，**模块之间依然存在较强的绑定关系**。
- 那么问题来了：如果我在进行 AI 训练，**只想增加计算资源而不扩展存储资源**，应该怎么做？（现实中并不存在“全能”配置，那样成本太高。我可能只需执行一次计算任务，没必要配置上百个硬盘的存储空间。）
    
- **小思考2**：像 JuiceFS、沧海存储、3FS 这样的国产存储系统，会不会走和 GPFS、Lustre 完全一样的技术路线？  显然不会——一味模仿注定难以超越。它们必须找到适合自己的应用场景
- 如果所有大型企业或研究院都自建机房、自搭集群，这不仅成本极高，也容易形成技术垄断，需要很强运营人员，例如 DBA，那还有其他公司生存空间，没有接触过，感受不到？


>所以，JuiceFS、沧海存储、3FS 等国产存储系统，
不会去走 GPFS/Lustre 一模一样的老路，而是会结合自身技术优势和市场需求，

走出一条差异化的发展道路。它们可能会在以下方面发力：

- **云原生：** 更好地与Kubernetes等容器编排平台集成。
- **RDMA/NVMe-oF：** 利用最新的高速网络和存储协议。针对特定应用场景（如 AI 大模型训练、海量小文件、实时分析）进行深度优化。
-   **软件定义存储（SDS）：** 提供更丰富的数据服务，如数据多活、数据迁移、数据智能分析
   整合文件、对象、表格等多种数据形态。
- 强化国产化适配和自主可控，满足国内企业的安全和合规需求

这才是明智和可持续的发展路径。



## 1.3 **一页 ppt 汇报：**


- JuiceFS vs 3FS 元数据常见误区

| 维度            | 常见疑问           | 关键技术                           | 解决方案             |
| ------------- | -------------- | ------------------------------ | ---------------- |
| **元引擎**       | JuiceFS 内置元服务  | 外部数据库驱动                        | 提前规划 Redis/TiKV  |
| **一致性**       | 两者一样 POSIX 强一致 | JuiceFS 元强一致但数据 close-to-open  | 关闭缓存或 3FS        |
| **rename 风暴** | 3FS 无瓶颈        | FDB 乐观事务会冲突                    | keyspace 设计+热点分片 |
| **小文件性能**     | 两者差不多          | 3FS RDMA，本地 NVMe；JuiceFS 走对象存储 | JuiceFS 用小文件打包   |
| **可运维性**      | JuiceFS 部署简单   | HA/扩容/热点规避需自管                  | Meta Proxy 或企业版  |
| **AI 训练场景**   | 可互换            | 3FS 专为大模型高并发元操作设计              | AI→3FS，湖→JuiceFS |
|               |                |                                |                  |
- 使用场景区别

| 特性维度             | JuiceFS                                                 | 沧海存储（百度智能云）                                                    | 3FS (Fire-Flyer File System)              |
| ---------------- | ------------------------------------------------------- | -------------------------------------------------------------- | ----------------------------------------- |
| ​**​核心架构​**​     | 数据与元数据分离，元数据存于数据库（Redis/MySQL等），数据存于对象存储（S3、OSS、MinIO等） | 统一分布式存储底座，支持对象存储（BOS）、文件存储（CFS/AFS），元数据与数据分离                   | 元数据与数据分离，数据存于本地NVMe SSD，元数据依赖FoundationDB |
| ​**​数据持久化位置​**​  | 对象存储（云上或自建）                                             | 百度智能云底层存储                                                      | 本地NVMe SSD                                |
| ​**​扩展性​**​      | 弹性扩展，元数据引擎和对象存储可独立扩容                                    | 支持EB级别扩展，统一存储底座支持平滑扩展                                          | 存储层需自行管理扩展，元数据服务可水平扩展                     |
| ​**​缓存机制​**​     | 支持本地缓存和分布式缓存（企业版）                                       | 依赖百度智能云底层优化，具体机制未公开                                            | 无额外缓存设计（直接访问高速本地磁盘）                       |
| ​**​网络协议​**​     | 基于 TCP/IP                                               | 未公开具体协议，支持标准S3/NSF等访问接口                                        | 支持 RDMA（远程直接内存访问），高吞吐、低延迟                 |
| ​**​POSIX兼容性​**​ | 完全兼容 POSIX                                              | 文件存储CFS/AFS支持POSIX，对象存储BOS为对象接口                                | 部分兼容POSIX（如文件长度最终一致，但不支持ACL、快照等高级功能）      |
| ​**​适用场景​**​     | 云原生、AI训练、大数据分析、Kubernetes持久化存储、文件共享与协作                  | 对象存储（BOS）：云存储、备份归档、静态资源分发  <br>文件存储（CFS/AFS）：高性能计算、AI训练、企业文件共享 | 大规模AI训练和推理（高带宽、低延迟读取场景）                   |
| ​**​冗余与一致性​**​   | 数据可靠性由对象存储保障，元数据依赖数据库事务                                 | 多副本、EC纠删码，强一致性保障                                               | 多副本，基于CRAQ协议实现强一致性                        |
| ​**​运维成本​**​     | 较低，无需管理物理存储，依赖云服务或自建对象存储                                | 作为云服务，用户无需关心底层运维                                               | 较高，需自行管理本地SSD硬件和数据副本                      |
| ​**​特色功能​**​     | 数据压缩、加密、快照、多云数据同步、S3网关代理                                | 作为百度智能云存储基座，支持多种存储服务统一集成                                       | Native Client（绕过FUSE，零拷贝，极致低延迟）           |
| ​**​开源/商业形态​**​  | 社区版开源（Apache 2.0），企业版提供高级功能和支持                          | 商业云服务（百度智能云）                                                   | 开源（MIT协议）                                 |

### 💡 关键对比总结：

- ​**​JuiceFS​**​ 更适用于​**​多云、混合云环境​**​，强调数据与元数据的分离存储和弹性扩展，适合需要高度兼容POSIX且希望降低运维复杂度的场景。
- ​**​沧海存储（百度智能云）​**​ 是​**​商业化云存储服务​**​，提供对象、文件、块等多种存储形态，强项在于底层统一存储底座技术，适合企业级云上应用和高性能计算场景。
- ​**​3FS​**​ 专注于​**​极致性能的AI训练和推理场景​**​，利用本地NVMe SSD和RDMA网络实现低延迟和高带宽，但需用户自行维护硬件和数据冗余。


# 二、在元数据管理 JuiceFS vs 3FS vs 沧海

## 2.1 架构概览对比



![JuiceFS](https://s2.loli.net/2025/08/31/NwI8DmOHCasAkKW.png)



![3FS](https://s2.loli.net/2025/08/31/gtYHo69kbFrscvn.png)
3FS 整个系统由四个部分组成，分别是 Cluster Manager、Client、Meta Service、Storage Service。

**所有组件均接入 RDMA 网络实现高速互联，DeepSeek 内部实际使用的是 InfiniBand。**
# 三、相关疑问


| 维度            | 常见误区           | 正解                             | 实战解决方案           |
| ------------- | -------------- | ------------------------------ | ---------------- |
| **元引擎**       | JuiceFS 内置元服务  | 外部数据库驱动                        | 提前规划 Redis/TiKV  |
| **一致性**       | 两者一样 POSIX 强一致 | JuiceFS 元强一致但数据 close-to-open  | 关闭缓存或 3FS        |
| **rename 风暴** | 3FS 无瓶颈        | FDB 乐观事务会冲突                    | keyspace 设计+热点分片 |
| **小文件性能**     | 两者差不多          | 3FS RDMA，本地 NVMe；JuiceFS 走对象存储 | JuiceFS 用小文件打包   |
| **可运维性**      | JuiceFS 部署简单   | HA/扩容/热点规避需自管                  | Meta Proxy 或企业版  |
| **AI 训练场景**   | 可互换            | 3FS 专为大模型高并发元操作设计              | AI→3FS，湖→JuiceFS |
## Q1:JuiceFS、3FS 在满足POSIX 协议，在AI 场景可以相互代替？


### <font color="#c00000">误区：都是 POSIX 接口，用哪个都一样。</font>

### **原因分析**
- AI 训练/推理场景**元操作密集**（dataloader 随机读、checkpoint频繁），JuiceFS Redis 模式会卡在内存瓶颈，TiKV 模式延迟高。
- 3FS 针对大模型 workload 设计：RDMA + 无状态 Meta + CRAQ，吞吐与一致性保障都更强。

### **解决办法**
- **数据湖** → JuiceFS。
- **大模型训练/推理** → 3FS。
- 各自使用场景
### 最少知识:

###  疑问 1：什么是 数据加载器（Dataloader）这个和元数据有什么关系 

  在大模型训练与推理中，**数据访问模式**和普通大数据分析、数据湖场景完全不同，

  主要体现在 **元数据操作（metadata operation）极度密集**。
  
 1. 数据加载器（Dataloader）

- **特征**：每个 batch 都要随机取几百～几千张小图片、文本块、embedding 碎片。
- **结果**：每秒可能有 **上万次随机打开 + seek + close** 操作。
- **关键**：这不是大顺序读，而是**元操作极端频繁**。

假设 GPT 训练一轮
每个 GPU 每秒 1,000 次文件 open+stat+read
8 张卡 = 8,000 ops/s
200 节点集群 = 1,600,000 ops/s  → 纯元操作洪峰

##### Checkpoint 高频写入

- 大模型训练时每隔几分钟会做 **checkpoint**，每次几十 GB～上 TB。
- Checkpoint 的写入会产生**大量小对象的创建、修改、删除**，同时对一致性要求高。
- POSIX 接口支持类似，但实现差异巨大。

操作密集指 **元操作（metadata ops）洪峰**，包括：

- `open` / `close`
- `stat` / `list`
- `mkdir` / `rmdir`


在 AI workload 中，IO 吞吐反而不是瓶颈，**每秒上百万的元操作才是杀手**。

在大模型场景下，文件系统面临的元数据访问压力非常不同于传统 OLAP：

|场景|特征|典型操作|
|---|---|---|
|**数据加载 (DataLoader)**|**随机小文件访问**，每个 batch 都要随机读取若干样本|`stat() + open() + read()` 非常频繁|
|**Checkpoint / Snapshot**|周期性大规模写入，且强一致要求|`rename()`、`fsync()`、`stat()`|
|**推理多租户**|同一模型多副本并行加载|相同小文件被高频打开|

特点：
- **元数据请求量级极高** → 可能达到 **百万 ops/s**
- **延迟敏感** → 如果元数据请求慢，GPU 空转，吞吐大幅下降
- **一致性要求高** → 特别是 checkpoint 不能“读到旧数据”


###  疑问 2：JuiceFS 为什么无法解决这些问题

####  **2.1 DataLoader 随机读瓶颈**

JuiceFS 元数据有两种模式：

- **Redis 模式**：
    - 所有元数据放在 Redis 内存
    - 单节点 Redis 无法支撑 **百万 QPS** 的随机元操作（单机一般 10万）
    - Redis 的性能瓶颈主要在于内存和网络 I/O，CPU 不会是性能瓶颈所在（满足百万）
    - CPU 单核瓶颈 + 内存瓶颈 → p99 延迟毫秒级，延迟增大
    -  CPU 的 NUMA 架构对 Redis 性能的影响![iRedis 的 99% 尾延迟](https://s2.loli.net/2025/08/31/AtPOIehVWxQn4Xo.png)

- **TiKV 模式**：
    - 虽然解决容量瓶颈
    - 但 TiKV 走 **Raft 强一致**写路径，写入和元查询都需要 Leader 提交 → p99 延迟经常 > 10ms

**结果**：DataLoader 每次 open/stat 文件都受制于元数据延迟，GPU 会频繁 idle。

---

#### **2.2 Checkpoint / Snapshot 瓶颈**

- JuiceFS 底层依赖对象存储（S3、OSS 等）
- 大文件写入需要：
    1. 切块（chunk）
    2. 并发上传
    3. 元数据逐块提交到 Redis/TiKV
- 当写入上百 GB 的 checkpoint 时：
    - Redis 模式：元数据爆炸
    -  TiKV 模式：容量大但延迟高**
- TiKV 通过 **Raft 强一致**，所有写入必须走完整 Raft 提交路径
- Raft 的特点：**写路径长** → leader 复制 → follower → commit → 回 ack
 
- GPU 训练场景对延迟极度敏感，10ms 足以让 GPU 闲置等待

> **结论**：TiKV 模式解决了“容量”，但在 AI workload 下会 **拖慢吞吐**。
- 对 AI 训练场景来说，Checkpoint 时间太长，GPU 大量等待


### 疑问3：3FS 如何规避这两个问题？

(1) RDMA：绕过内核、零拷贝加速

(2) 无状态 Meta：横向扩展

(3) CRAQ（Chain Replication with Apportioned Queries）


#####  3.1 **无状态 MetaServer：缓解百万 QPS 元操作**

| 对比项           | JuiceFS（Redis 模式）    | 3FS 无状态 Meta            |
| ------------- | -------------------- | ----------------------- |
| 元数据存储         | Redis 单点 / TiKV Raft | **分布式 KV + Shared Log** |
| MetaServer 状态 | 有状态                  | **无状态**                 |
| 扩展方式          | Redis/TiKV 水平扩展受限    | MetaServer 可无限水平扩展      |
| 元操作 QPS       | 单 Redis ~ 20W QPS    | **> 100W QPS**          |

- 3FS 把所有元数据存入 **共享日志 + KV 存储**：
    - MetaServer 本身 **无状态**，只做请求转发和缓存
    - 任何 MetaServer 都可处理 open/stat/create 等请求
    - 元数据一致性通过后端共享日志保证
        
**效果**：支持百万级 QPS 的元数据访问，DataLoader 不再受制于元数据延迟。

**3FS 无状态 Meta**：
- MetaServer **不存状态**，真正的元数据存在共享 KV/日志系统。
- 每个 MetaServer 节点都可以并行处理请求 → 元操作的负载**天然分散**。
- 水平扩展简单：直接增加 MetaServer 节点即可提升吞吐，无需对存储 KV 做迁移或复杂协调。

Tidb 也是这样设计呀？

| 层级                        | 角色与状态                                                         |
| ------------------------- | ------------------------------------------------------------- |
| **TiDB Server**           | SQL 层 / 计算节点，**无状态**，请求路由 + 执行计划生成，内存缓存热点数据即可，节点挂掉无持久状态       |
| **PD (Placement Driver)** | 集群元信息管理（Region 映射、Leader 信息、TSO 时间戳），**有状态**，必须持久化，否则集群无法定位数据 |
| **TiKV**                  | KV 存储节点，**有状态**，存储实际数据 + Raft 日志，维护 Region 状态机                |

- **3FS 的无状态节点**不仅是“计算无状态”，而是**元数据访问也不依赖节点持久状态**。
    
    - 任何节点都可以处理 open/stat/create/delete 请求
        
    - 节点 crash 不影响元数据访问
        
- **TiDB 的 PD**负责元数据和集群状态
    
    - 高频元操作仍依赖 PD / Leader 节点
        
    - 节点扩容和故障恢复要协调 PD → 元操作受制于有状态节点
- **TiDB Server 无状态，但元数据依赖 PD/TiKV 的状态** → 节点不能随意水平扩展来处理高频元操作
- **3FS MetaServer 无状态 + KV/log 持久化** → 节点水平扩展完全透明，不影响元数据访问

>File metadata operations (e.g. open or create files/directories) are sent to metadata services, which implement the file system semantics. 
>Metadata services are stateless, since file metadata are stored in a transactional key-value store (e.g. FoundationDB). Clients can connect to any metadata service


 **对比 3FS 无状态 MetaServer**

|特性|JuiceFS + TiKV|3FS MetaServer|
|---|---|---|
|节点是否持久化状态|**有状态**：Leader 保存 Raft 日志和状态机|**无状态**：节点不保存全局元数据状态，只做路由/缓存|
|元数据访问路径|写必须访问 Leader|写可发到任意 MetaServer 节点，由底层 KV/log 保证一致性|
|扩展方式|Leader 受限，高频操作瓶颈|任意节点处理请求 → 水平扩展天然分散|
|节点崩溃恢复|Raft 日志回放恢复|节点无状态 → 秒级恢复|

---


- **“有状态”不单指存储位置**，还包括**节点在操作元数据时是否依赖自身状态（日志、commit index、缓存）**
- JuiceFS TiKV 模式：Leader 保存 Raft 状态 → 元操作依赖节点 → 有状态
    
- 3FS：MetaServer 节点不保存全局状态 → 真正的无状态

#### 3.2 CRAQ（Chain Replication with Asynchronous Quorums）

原文：
- Each storage service manages a few local SSDs and provides a chunk store interface.
- The storage service implements Chain Replication with Apportioned Queries (CRAQ) to ensure strong consistency. CRAQ’s write-all-read-any approach helps to unleash the throughput of SSDs and RDMA network. 
- A 3FS file is split into equally sized chunks, which are replicated over multiple SSDs.

| 特性             | JuiceFS + TiKV           | 3FS（DeepSeek）    |
| -------------- | ------------------------ | ---------------- |
| 元数据一致性         | Raft 强一致，多数派提交           | CRAQ 顺序复制 + 日志一致 |
| 写路径长度          | Leader + Follower 多轮 RTT | 单链尾写入，一轮 RTT     |
| 单次写元延迟（p99）    | 10~15ms                  | <1ms             |
| 读取策略           | 只能读 Leader               | 任意节点可读           |
| AI workload 适配 | 差，易卡在元数据瓶颈               | 好，针对训练优化         |



#### ✅ ​**​为什么能强一致？​**​

1. 1.​**​所有节点必须写入​**​：数据必须从 Head 传到 Tail，​**​所有节点都持久化 v2​**​。
2. 2.​**​尾节点（Tail）是最终裁决者​**​：只有 Tail 确认写入成功后，才广播通知其他节点“此版本已提交”。
3. 3.​**​写入期间阻塞读​**​：如果读请求命中 dirty 节点，需等待 Tail 确认后才响应（后文详解）。

> 💡 ​**​类比理解​**​：就像银行转账——必须经过记账员A→B→行长C签字，​**​三方全部确认​**​才算成功，缺一不可


##### **读取路径（Read-Any）​**​：如何保证读到最新数据？

#### 🔧 ​**​核心规则：任意节点都可读，但通过版本号+状态保证正确性​**
![](https://s2.loli.net/2025/08/31/erESaQ5FG9jfu3i.png)

#### ✅ ​**​如何确保一定是最新数据？​**​

|​**​节点状态​**​|​**​读取行为​**​|​**​是否最新数据​**​|​**​原理​**​|
|---|---|---|---|
|​**​Clean​**​|直接返回本地数据|✅ 是|该节点数据已通过 Tail 确认，与链尾完全一致|
|​**​Dirty​**​|先向 Tail 查询最新版本|✅ 是|Tail 拥有全局最新版本（所有写入必经它），查询后返回​**​已提交的最新数据​**​|
|​**​未同步​**​|从 Tail 拉取最新数据|✅ 是|节点可能落后于 Tail，但通过查询 Tail 强制同步|

> 💡 ​**​关键​**​：无论读哪个节点，最终依赖 ​**​Tail 的权威版本​**​兜底！  
> ​**​即使中间节点数据未提交（Dirty）​**​，也会通过查询 Tail 获得最新值，​**​绝不会返回旧数据​**​。


当读取命中 Dirty 节点时，需额外向 Tail 查询。RDMA 的价值在此凸显：

![](https://s2.loli.net/2025/08/31/WQOawvyBDnPAz3o.png)

- ​**​传统网络​**​：查询 Tail 需经操作系统内核（延迟 50μs+）
- ∙​**​RDMA​**​：网卡直接访问 Tail 内存（延迟 1~5μs ⚡️），​**​几乎等同于读本地 SSD！**





| 对比项    | JuiceFS          | 3FS            |
| ------ | ---------------- | -------------- |
| 传输协议   | TCP / HTTP Range | **RDMA**       |
| 内核路径   | 走 Linux TCP 栈    | **绕过内核，用户态直通** |
| 内存复制   | 至少 2 次 memcpy    | **零拷贝**        |
| p99 延迟 | ms 级             | **亚毫秒**        |


3FS 引入 **CRAQ（Chain Replication with Asynchronous Quorums）**，写入与读取路径彻底优化：

#### **写入路径**
- 只需将数据写到 **链尾节点**
- 中间节点异步复制，无需写入方等待
- 写入延迟接近单机磁盘速度

#### **读取路径**
- 可从链上任意节点读取，而非必须找 Leader
- **并行多副本读**，读取延迟显著降低
#### **一致性保障**
- CRAQ 保证 **单调一致（Monotonic Consistency）**
- 对 Checkpoint / Snapshot 场景足够安全，且大幅降低写延迟

**效果**：
- Checkpoint 写入速度比 JuiceFS 提升 **2~4 倍**
- Snapshot 操作几乎无阻塞，p99 延迟可控制在 **<5ms**


# 参考资料

####  【1】 DeepSeek 3FS 与 JuiceFS：架构与特性比较 
- https://juicefs.com/zh-cn/blog/engineering/deepseek-3fs-vs-juicefs
#### 【2】 Lustre 与 JuiceFS ：架构设计、文件分布与特性比较
- https://juicefs.com/zh-cn/blog/engineering/lustre-vs-juicefs
#### 【3】# DeepSeek 3FS：端到端无缓存的存储新范式

### [4] # 17 为什么CPU结构也会影响Redis的性能？
- 主流的 CPU 架构
-  CPU 的 NUMA 架构对 Redis 性能的影响
![](https://learn.lianglianglee.com/%E4%B8%93%E6%A0%8F/Redis%20%E6%A0%B8%E5%BF%83%E6%8A%80%E6%9C%AF%E4%B8%8E%E5%AE%9E%E6%88%98/assets/30cd42yy86debc0eb6e7c5b069533ab0-20221015223416-fpu2z51.jpg)
- 图中的网络中断处理程序被绑在了 CPU Socket 1 的某个核上，而 Redis 实例则被绑在了 CPU Socket 2 上。

- 此时，网络中断处理程序读取到的网络数据，被保存在 CPU Socket 1 的本地内存中，当 Redis 实例要访问网络数据时，就需要 Socket 2 通过总线把内存访问命令发送到 Socket 1 上，进行远程访问，时间开销比较大。
- 为了避免 Redis 跨 CPU Socket 访问网络数据，我们最好把网络中断程序和 Redis 实例绑在同一个 CPU Socket 上，这样一来，Redis 实例就可以直接从本地内存读取网络数据了，如下图所示

![https://time.geekbang.org/column/article/286082](https://learn.lianglianglee.com/%E4%B8%93%E6%A0%8F/Redis%20%E6%A0%B8%E5%BF%83%E6%8A%80%E6%9C%AF%E4%B8%8E%E5%AE%9E%E6%88%98/assets/41f02b2afb08ec54249680e8cac30179-20221015223416-y165yv4.jpg)

### 【5】 TIkv
- https://tikv.org/docs/6.1/deploy/performance/overview/
- ​**​TiKV 官方数据​**​：∙单副本（无 Raft）：​**​p99 ≈ 2~3ms​**​∙三副本（Raft 共识）：​**​p99 ≈ 10~20ms​**
The results show that **A 3-node TiKV cluster achieves at most 200,000 OPS within 10 ms latency in a 10M records and 10M operations YCSB workload**.
- https://github.com/juicedata/juicefs/issues/5021
- JuiceFS的测试结果： 测试中 P99.99 延迟达 1.7ms，显著高于 OSSFS 的 31µs

#### 【6】3fs
- 论文：Object Storage on CRAQ High-throughput chain replication for read-mostly workloads
- https://www.youtube.com/watch?v=hmyzjSFjxys&ab_channel=JeffTerrace # Object Storage on CRAQ - USENIX 2009 - Part 3/3

#### 【7】 沧海
- https://zhuanlan.zhihu.com/p/558966679
- [Linux高性能网络详解从 DPDK、RDMA 到 XDP（上](https://mp.weixin.qq.com/s/wynWnvKSc6SulOKp1CYPGQ)
![](https://s2.loli.net/2025/08/31/dowAPZhBWmE5tQx.png)


# 广告时间

c++周刊目的陪你一起快速冲击大厂面试

 **21天C++面试冲刺周刊**

因为，21天就够了，  
足够让我火力全开，  
- 一边补齐 C++ 知识点，  
- 一边刷爆经典面试题，  
- 一边撸穿开源项目，  
- 让自己变得不可替代！

 ![​一万年太久，只争三周​](https://s2.loli.net/2025/08/24/AMq3IdPaloSnBpv.png)

> 小提示：不要把他看成一个出售给你产品，我只出售给自己
> 在公司做任何事情事情，
> 都必须清楚拆解需求功能，开发周期，最后得到什么结果，
> 同样面试准备也是如此，给自己一个期限 21 天，给自己大纲，然后给自己 21 天学习结果，这样自己才能安心准备下去。 



曾经有一个**让我心跳加速的岗位**放在我面前，  
我没有珍惜。  
等到别人拿到 offer 的那一刻，  
我才追悔莫及！

人世间，最痛苦的事情，  
不是没钱吃饭，  
也不是没房没车，  
而是——**错过了那个能让我逆天改命的机会！**

如果上天再给我一次机会，  
我一定会对那个岗位说三个字：  
**“我要你！”**

如果非要在这份“心动”上加一个期限，  
**一万年太久了……**  
**我只想要——21天！**


**你可能面临两种选择**

### **① 犹豫不前：准备到天荒地老**

“这个岗位太难了，我先准备一下吧。”  
于是你准备1天、1周、1个月、1年……  
等再回头，**3年就这样过去了**。
- 每天忙着搬砖，没时间系统复习
- 每次想起要准备，又感觉心里没底
- 面试知识点更新太快，拿着旧地图找新机会
最后，错过了一次又一次心动的岗位。

### **② 盲目回答：机会就在眼前，却抓不住**

终于等来一场面试，  
你觉得问题很简单，张口就答，  
结果用“几千元思维”回答“百万年薪岗位”。

- 面试官问到C++底层实现，答不上来
- 设计题说到高并发架构，没实战经验
- 一紧张，连项目里真实做过的东西都讲不清

一次面试失利，也许就意味着和理想岗位失之交臂。

### **更残酷的是**

在你犹豫的这几年里，  
找工作的成本越来越高：
- 一个部门、一个领导，可能坚持一年就被解散
- 一个项目，可能在10年、20年后，  
    曾经复杂的业务规则、先进的架构，早已被淘汰
- 市场上新的技术和面试要求，每年都在不断升级

等你回过头来，发现不仅机会没了，  
**连准备的方向都变了**。


### **核心方法论**：
>让你学到每个 c++知识，都关联一个经典面试，并对对应开源项目实践
- **系统备战**  
    每天 20~30 分钟，聚焦 C++ 核心知识，  
    三周时间完成高效梳理。
- **经典面试题**  
    每个知识点都关联一个**高频面试题**，  
    让你知道“为什么考”和“怎么答”。
- **开源项目实践**  
    通过真实项目理解底层原理，  
    不背答案，而是用**实践打动面试官**。
- **场景驱动学习**  
    还原真实面试场景，  
    帮你学会“怎么说服面试官”。
## **21天，你会获得什么？**

- 一份**完整的C++面试知识地图**
- 一套**高频题+解析+项目实践**组合拳
- 一次**全链路模拟面试体验**
- 三周后，面对面试官，你能自信说出：  
    **“问吧，准备好了。”**



>这也是我的面试方法：
- 如果一开始就直接学某个知识点，我常常感觉不到它的实际价值。
- 所以我会先尝试树立一个整体的大局观，就算过程中被现实“啪啪打脸”了又怎样？
- 把每一次面试都当成一场陪练，用面试官的专业视角和真实项目来反推和校正自己的理解，不是更好吗？这种即时、高质量的反馈，是你看多少书、自己一个人闷头琢磨多久，都很难获得的。
整体知识看板（欢迎提供更多线索）：
- 第一周:c++基础知识高频面试题解析【当前位置】
![](https://s2.loli.net/2025/08/24/ZKvcR4F1yoe9th8.png)


- 第二周: 专注分布式存储,数据库广告搜索 Ai 辅助驾驶 大厂热门后端领域项目（基本功)拆解
![](https://mmbiz.qpic.cn/mmbiz_png/DMCnyCrhia37yjSL2xfQSbz0QT05IVAvsibibvAT7xFUzAhCqEKviblv6YpjuPT2VCCzZgMLDtDzGnFmHjuKOjHtTg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1)

- 第三周：系统架构设计，用未来 10 年发展目标，重新设计原来系统

![](https://mmbiz.qpic.cn/mmbiz_png/DMCnyCrhia36iaMAgdk7ib4vIrpf6urrNhdmqVHib41GQAGXDMTPDvib0Ue3LROIocBSCflxytXXVoiap04lR7kOMfwA/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&tp=webp)

## 最动人的作品，为自己而写，刚刚好打动别人



 1️⃣ 如果有更多疑问，联系小王，一起交流，进步

![个人联系方式](https://s2.loli.net/2025/08/13/veChAocQwJONWKE.png)

2️⃣  关注公众号:后端开发成长指南(回复"面经"获取）获取过去我**全部面试录音**和面试复盘。
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)


#### 抬头看天：走暗路、耕瘦田、进窄门、见微光
- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。
# QA
## 1. 请问 ext4 文件系统 和 gpfs Lustre 文件系在计算和存储区别是什么？

Q2：
## 提问：
在元数据管理 JuiceFS vs  DeepSeek 3FS vs 沧海架构区别
### 角色：

你是一位在硅谷以“独角兽捕手”著称的传奇CTO。 
你面试过上千名工程师，其中不乏业界大神。
你的风格是：层层深入，精准设陷，直至探明对方的真实能力边界和思维底层。 
你对技术谎言和知识“泡沫”有天生的嗅觉

擅长分布式数据存储 和分布式数据库，精通 liunx c++


## ​​任务


根据用户【提问】 输入问题，按照从新生到大师角度【要求】回答问题




## [要求]

- 根据事实回答，不清楚不要捏造
- 
- 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

二、这个技术的优势和劣势分别是什么 

三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比
