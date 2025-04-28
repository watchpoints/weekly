---
title: 深入理解BlueStore
date: 2025-04-27
description: you can
draft: false
tags:
  - CEPH
categories:
  - CEPH
---
### 一、BlueStore出现的背景、初衷

### 看版本变化

![Ceph 12.2.0 正式版本发布](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250427125143.png)


- 2016年4月21 Jewel版本10.2.0，首次引入 **BlueStore 存储引擎**

-  2017年8月29，Ceph 12.2.0 正式版本发布， 代号 Luminous，**BlueStore 成为默认存储引擎**

<font color="#ff0000">BlueStore 是一个从头开始设计的存储后端，</font>
<font color="#ff0000">旨在解决使 用本地文件系统后端所面临的挑战。</font>

BlueStore 仅用了两年时间就实现了所有这些目标， 并成为 Ceph 的默认存储后端


![ 文件系统作为分布式存储后端：从 Ceph 十年进化中汲 取的教训](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250427162927.png)




BlueStore 的一些主要目标包括：
1. 快速元数据操作（第 4.1 节）
2. 对象写入无一致 性开销（第 4.1 节）
3. 基于写时复制的克隆操作 （第 4.2 节） 
4. 无日志记录的双重写入（第 4.2 节）
5. 优化了 HDD 和 SSD 的 I/O 模式（第 4.2 节）

对比如下：

| **BlueStore vs FileStore** | FileStore (基于文件系统)  | BlueStore (直接管理裸设备)          | 核心优势说明                |
| -------------------------- | ------------------- | ---------------------------- | --------------------- |
| ​**​系统架构​**​               | XFS + LevelDB       | RocksDB + 块设备                | 绕过文件系统直接管理磁盘，降低软件栈复杂度 |
| **​双写问题​**                 | 存在                  | 无（原子写操作）                     | 消除写放大，提升性能            |
| ​**​批量读取**​                | 通过 XFS 文件系统读取（百万级别） | RocksDB 元数据 + 直接块设备访问        | 减少 I/O 路径延迟           |
| ​**​对象列举​**​               | 需对 XFS 文件排序         | RocksDB 预排序键值(​**​LSM 树结构**) | 列表操作快 10 倍以上          |
| ​**​批量删除​**​               | 逐个删除 XFS 文件         | 批量删除 RocksDB 键值，批量事务         | 删除速度提升 50-100 倍       |
| ​**​对象读缓存​**​              | 依赖 XFS 缓存机制         | 需用户态实现（如 LRU 缓存）             | 灵活性更高，可定制化            |


#### 克服本地文件系统面临什么挑战
##### 挑战 1：高效事务
##### 挑战 2：快速元数据操作
- Ceph 与 FileStore 后端的关键元数据 挑战之一来自在大目录上的缓慢目录枚举（ readdir ） 操作，
- 以及返回结果的缺乏排序。
##### 挑战 3：支持新的存储硬件

### 二、这个技术的优势和劣势分别是什么 

- 还有更强的


![# Crimson: Next-generation Ceph OSD for Multi-core Scalability](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250427165317.png)



Crimson 是基于 Seastar C++ 框架实现的 Ceph 下一代 OSD，用以提升多核可扩展性与 I/O 性能。

Crimson 被设计为高性能对象存储守护程序 （OSD），针对 NVMe 等快速存储设备进行了优化。

为了实现这些目标，Crimson 使用包括 SPDK、DPDK 和 C++ Seastar 框架在内的尖端技术从头开始构建。

Crimson 继续沿用 BlueStore 作为底层存储后端

**并在未来计划通过原生 Seastore 存储后端摆脱对 BlueStore 的依赖**


### 三、技术的组成部分和关键点。


架构如下：
![](https://blog.wjin.org/assets/img/post/ceph_bluestore.png)



<font color="#ff0000"> BlueStore主要4个技术点</font>：

#### 用户态直接写快设备( BlockDevice.)

1. BlueStore 直接读写原始块设备（如 SSD/NVMe）
2. 在用户态使用linux aio直接操作块设备，去除了本地文件系统的消耗

  **聪明你可能你会疑问**：
  1.  绕过文件系统，在用户态，怎么完成**IO读写**？
  2. RocksDB本身并不支持对裸设备的操？还是使用文件系统？矛盾
  3. 远离文件系统不能更好支持 - 对全SSD及全NVMe SSD闪存适配？
  
####  轻量文件系统
  
  1. BlueFS基于裸设备模拟一个简易的用户态文件系统，用于支持RocksDB。
  2. 支持 SSD/NVMe新硬件

 **聪明你可能你会疑问** ：
 为了使用RocksDB，有写一个文件系统？
 
1.  POSIX文件系统作为通用的文件系统，其很多功能对于RocksDB来说并不是必须的，为了进一步提升RocksDB的性能，RocksDB的场景量身定制一套本地文件系统，BlueFS也就应运而
2. BlueRocksEnv封装文件系统的抽象接

#### 文件系统磁盘空间管理和缓存管理
1. 失去默认文件系统支持，要一个磁盘的空间管理系统，Bluestore采用Allocator进行裸设备的空间管理，目前支持StupidAllocator和BitmapAllocator两种方式；
2. BlueStore 实现为用户态并通过 DirectIO 直接访问裸盘，因此不实用 kernel 提供的 page cache，为了提升读的性能，需要自行管理Cache。

**聪明你可能你会疑问** ：
1. 为了规避一个问题，出现更多问题，我复习基本Direct I/O 是啥吧？

#### ​​RocksDB 键值存储元数据​​：

​​1. 慢速层（Block）​​：存储对象数据，通常使用 HDD。
​​2. 高速层（DB）​​：存储 RocksDB 的 SST 文件，通常使用 SSD。
​​3. 超高速层（WAL）​​：存储 RocksDB 日志，通常使用 NVMe

BlueStore是基于RocksDB和BlockDevice实现的ceph的对象存储


### 四、技术的底层原理和关键实现

## IO流程

IO力促优化

### 六、已有的实现和它之间的对比



## 参考

###### 【1】 File Systems Unfit as Distributed Storage Backends: Lessons from 10 Years of Ceph Evolution 
- 文件系统作为分布式存储后端：从 Ceph 十年进化中汲 取的教训
- https://dl.acm.org/doi/pdf/10.1145/3341301.3359656
###### 【2】Ceph OSD Crimson: Next-generation Ceph OSD for Multi-core Scalabilit 
- https://ceph.io/en/news/blog/2023/crimson-multi-core-scalability/
- ceph-osd-cpu-scaling
- https://ceph.io/en/news/crimson/
- https://seastar.io/