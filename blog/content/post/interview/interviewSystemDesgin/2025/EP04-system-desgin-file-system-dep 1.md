---
title: 从零开发分布式文件系统(三) :国产文件系统JuiceFS｜沧海｜3FS 能相互替代吗?百万 OPS如何满足（2）(ceph 默认 5 千)
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

##  **本期任务​**​：

- 从零开发分布式文件系统（三）：JuiceFS｜沧海｜3FS 架构差异对比
    
- **小思考**1：诸如 DDN 公司的 Lustre 和 IBM 的 GPFS 这类系统，虽在架构上实现了计算与存储分离，但其计算节点和存储节点仍处于同一集群内部，**模块之间依然存在较强的绑定关系**。

- **小思考2**：像 JuiceFS、沧海存储、3FS 这样的国产存储系统，会不会走和 GPFS、Lustre 完全一样的技术路线？  显然不会——一味模仿注定难以超越。它们必须找到适合自己的应用场景




##  **一页 ppt 汇报：**



| 视角     | 序号  | 误区                          | 真相                                                                            | 解决办法 / 适用场景                                                                               |
| ------ | --- | --------------------------- | ----------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| **新生** | 1   | JuiceFS 自带强大元数据引擎           | JuiceFS 元数据外包给 Redis/TiKV/MySQL/Postgres，性能取决于数据库                             | 小规模 / 原型期：Redis + Lua 原子操作；大规模：TiKV + 多 Region，多副本部署；容量规划参考：内存 ≈ inode_count × 300B × 副本数 |
|        | 2   | 3FS = HDFS 升级版              | 3FS 无状态 Meta + CRAQ 链式数据复制，整体更像 “FDB + RDMA + POSIX 翻译层”，不是简单 HDFS            | 面试时追问 rename 跨目录写集、事务冲突等，不可简单类比 HDFS                                                      |
|        | 3   | JuiceFS/3FS 一致性都一样          | JuiceFS 数据默认 close-to-open，客户端缓存可能读旧值；3FS 基于 FDB 事务和 CRAQ 保证 Serializable 一致性 | JuiceFS：关闭元缓存保证 open() 立即可见；3FS：高并发 rename 场景设计事务冲突重试                                     |
| **进阶** | 4   | Redis + JuiceFS 可以撑无限 inode | Redis 单实例单核 ~10^5 ops/s，热点目录 + rename 风暴 CPU 拉满；容量受限                          | inode >1亿：选 TiKV/Postgres；rename 风暴：Lua 原子操作合并请求或目录散列                                     |
|        | 5   | 3FS 天然抗 rename 风暴           | FDB 是乐观事务，rename 同目录会冲突，尾延迟可能爆炸                                               | 分散 rename 操作到不同 key-range，热点目录分片或业务规避大规模 rename                                           |
|        | 6   | JuiceFS/3FS 小文件性能差不多        | JuiceFS 数据面 S3/OSS/GCS，小文件随机读写延迟高；3FS 数据面 RDMA + NVMe，延迟低                     | JuiceFS：融合小文件或增大客户端 metadata cache；3FS：直接走 NVMe + RDMA                                    |
| **大师** | 7   | JuiceFS 控制面简单，不需团队          | 社区版无独立 Meta Service，HA、分片、扩容、热点需自扛                                            | 部署 JuiceFS Enterprise 或自己做 Meta Proxy 做路由和负载均衡                                            |
|        | 8   | 3FS 用 FDB 就没有瓶颈             | FDB 单集群 Leader-based，热点 key-range 会造成瓶颈；高并发目录操作冲突指数上升                         | 设计 keyspace 避免热点；演练 rename 风暴冲突重试                                                         |
|        | 9   | JuiceFS、3FS 在 AI 场景可互换      | AI 场景元操作密集，JuiceFS Redis 内存瓶颈，TiKV 延迟高；3FS 针对大模型 RDMA + 无状态 Meta + CRAQ       | 数据湖 → JuiceFS；大模型训练/推理 → 3FS                                                              |



![JuiceFS](https://s2.loli.net/2025/08/31/NwI8DmOHCasAkKW.png)



![3FS](https://s2.loli.net/2025/08/31/gtYHo69kbFrscvn.png)

# 二、 误区1：3FS 一旦用了 FoundationDB，就没有瓶颈

>误区  “FoundationDB 有分布式事务，系统极限就是线性扩展。”

**真相**
- FoundationDB 是单集群 Leader-based 协议，热点 key-range 会导致 leader 抢占成为瓶颈。
- rename/mkdir 等目录级元操作高并发下，FDB commit 层会触发冲突指数上升。
    
**解决办法**
- **设计 keyspace** 避免热点。
- 提前演练 FoundationDB 风暴的冲突重试。

# 三、 **误区 2：3FS 可以天然抗 rename 风暴**

> “3FS 用 FDB 做事务，rename 高并发完全没问题。”

**真相**

- FoundationDB 是**乐观事务**，如果 rename 操作涉及同一个目录，冲突率会飙升。
    
- rename 风暴下，**FDB 的冲突重试**会造成尾延迟爆炸。
    

**解决办法**

- 将 rename 操作分散到不同 key-range，避免同一事务键冲突。
    
- 热点目录分片，或直接业务设计上规避大规模 rename。

# 四、 **误区3 ：JuiceFS 和 3FS 的小文件读写性能相差不大**

> “元数据都差不多，数据面性能才是瓶颈。”

**真相**

- JuiceFS 数据面是对象存储（S3/OSS/GCS），小文件随机读写会触发对象 GET/PUT，延迟高。
    
- 3FS 数据面是 RDMA + 本地 NVMe，完全不同量级。
    
**解决办法**

- **JuiceFS 小文件场景优化**：
    - **融合小文件**：用 `--small-object` 配置将小文件打包。
    - 增大客户端 metadata cache，减少 round-trip。
- **3FS**：直接走 NVMe + RDMA，毫秒级随机读写。


# 五、 **误区 4：Redis + JuiceFS 可以撑无限 inode**

> “Redis 是内存 KV，QPS 高，JuiceFS 元数据选 Redis 就能无脑扩。”

**真相**

- Redis 模式下，**单实例单核瓶颈 ~ 10^5 ops/s**。热点目录 + rename 风暴会直接把 CPU 拉满。
    
- 单实例容量瓶颈：每 inode ~300B，Redis 官方建议单实例 <64GiB，否则复制/恢复耗时过长。
    

**解决办法**

- 超过 1 亿 inode → 选 TiKV/Postgres 模式。
    
- rename 风暴 → 开启 Lua 原子操作合并请求，或业务上引入 **分层目录散列**，避免单点热点。


# 六、 **误区 5：JuiceFS 和 3FS 的一致性都一样**

>  误区：“JuiceFS/3FS 都是 POSIX 强一致，没差别。”

**真相**

- **JuiceFS**：
    - **元数据强一致**（以元数据提交点为真）。
    - 数据放对象存储 → 默认“**close-to-open**”语义，文件写完 close 后才能保证被别人看到。  
        如果开客户端 metadata cache，还可能读到旧值。
- **3FS**：
    - 基于 FDB 的事务，目录树操作天然 Serializable 级别。
    - CRAQ 确保副本一致，不依赖“close”时机。

**解决办法**

- JuiceFS 场景：
    - 如果应用要求“open() 立即可见”，**关闭元缓存**，代价是 QPS 掉 30-50%。
- 3FS 场景：
    - 高并发 rename 场景下，需设计事务冲突重试逻辑。



#### 案例：进程 A 写入文件，进程 B 立刻读取

|​**​步骤​**​|​**​JuiceFS（默认）​**​|​**​3FS​**​|
|---|---|---|
|进程 A 写文件|数据缓存本地|数据通过 CRAQ 写入所有副本|
|进程 A 未关闭文件|进程 B 读取 → ​**​可能看到旧文件/空文件​**​|进程 B 读取 → ​**​直接获取最新数据​**​|
|进程 A 关闭文件|元数据提交，进程 B 下次 open 可见新内容|无额外操作|
|​**​解决方案​**​|关闭客户端元数据缓存|无需操作|
|​**​代价​**​|读性能下降 30–50%|无性能损失|


​**​Close-to-Open (C2O) 一致性模型在 Ceph 中被明确采用​**​，尤其在 ​**​CephFS 文件系统​**​中作为核心可见性保障机制

![](https://s2.loli.net/2025/08/31/qmOXTKcrBkaZU9R.png)


| **系统​**​        | ​**​Close-to-Open 实现​**​ | ​**​优势​**​     | ​**​缺陷​**​   |
| --------------- | ------------------------ | -------------- | ------------ |
| ​**​Ceph​**​    | 强依赖 MDS + 显式 `close()`   | 成熟稳定，多 MDS 可扩展 | MDS 易瓶颈，调参复杂 |
| ​**​JuiceFS​**​ | 依赖元数据引擎 ACK              | 部署简单，引擎可选      | 关缓存后性能骤降     |
| ​**​3FS​**​     | ​**​无 Close-to-Open​**​  | 立即可见（CRAQ 写穿透） | 需处理 FDB 事务冲  |

# 参考资料

## FoundationDB

- 2018年4月20日，苹果宣布开源FoundationDB，一款支持多种数据模型、高性能、高可用、可扩展，且具备ACID事务的分布式KV NoSQL系统。
- FoundationDB已在苹果公司内部的生产环境使用三年，主要用于iCloud上的云存储服务
- FoundationDB有潜力成为下一代分布式数据库系统的底层基础设施
- 在独立的KV存储服务上实现事务ACID语义
- https://zhuanlan.zhihu.com/p/38223586
- https://www.foundationdb.org/
- https://news.ycombinator.com/item?id=37552085



# 广告时间

c++周刊目的陪你一起快速冲击大厂面试

 **21天C++面试冲刺周刊**

因为，21天就够了，  
足够让我火力全开，  
- 一边补齐 C++ 知识点，  
- 一边刷爆经典面试题，  
- 一边撸穿开源项目，  
- 让自己变得不可替代！

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


