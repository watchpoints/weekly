---
title: BlueStore write先写数据 还是元数据 一面试就迷惑 ,乔哈里视窗模型帮你理清思路，从青铜秒变王者
date: 2025-07-26
description: do book
draft: false
tags:
categories:
  - do book
---

![](https://s2.loli.net/2025/09/13/NLX9KhzSHQBrmst.png)

#  一、面试官


## 1.1  开放区（你知道，别人知道）这样回答 面试官不管兴趣

### **面试官：**


我看到你简历上写着  分布式文件系统ceph 源码 

那你能不能谈一谈：BlueStore write 一个对象过程？ 


### 小青视角回答:


![图1-BlueStore](https://s2.loli.net/2025/09/14/r5zKhpQFZx6H7af.png)

**BlueStore** 把元数据和对象数据分开写，

对象数据直接写入硬盘，

而元数据则先写入超级高速的内存数据库，后续再写入稳定的硬盘设备


![图2-类图](https://s2.loli.net/2025/09/14/13Uj45pd2xn6eC9.png)


![图-3 类图](https://s2.loli.net/2025/09/14/VX5gQ6bqYExhR1v.png)

- RocksDBStore：封装了rocksdb。BlueStore中的元数据持久化到rocksdb，BlueStore的事物依赖rocksdb实现。

- BlueFS：为对接rocksdb实现的最小文件系统，通过File、Dir、FileWriter、FileReader实现文件读取

- Allocator：磁盘空间分配器，BlueStore最小分配单元为4k(SSD)，64k(HDD)，BlueFS最小分配单元为1M。默认实现为BitmapAllocator，还有StupidAllocator、AvlAllocator、HybridAllocator、ZonedAllocator等选择，通过配置bluestore_allocator和bluefs_allocator来修改。

- KernelDevice：磁盘设备。默认实现为BlockDevice，还有PMEMDevice、NVMEDevice等选择
### 面试官视角:

（轻咳一声）……嗯，行了，到你这里，我直接说我的看法。

这些内容都忘了搜集的，文档上这么写
**这种回答，我这里是不能给通过的。**


不过我给你提示 

- 采用什么数据结构表示 文件，对象，块？

- 大小文件怎么写入的？


## 1.2. 盲区（别人知道，你不知道):基本结构


### 假如购物系统，你设计一个表存储商品，请问BlueStore 存储文件元数据内容是什么，具体表结构是什么

类比：假如购物系统，你设计一个表存储商品


类比：假如购物系统，你设计一个表存储商品。

BlueStore 直接管理裸设备，没有文件系统，  
操作系统的 page cache 使用不上，  
所以需要自己管理 **元数据和数据的缓存**。

元数据存储在 RocksDB 的 KV 中。  
具体表结构是什么？

![](https://s2.loli.net/2025/09/14/1WtsgGcpenaMm3j.png)


BlueStore uses the RocksDB key-value database to manage internal metadata, such as the mapping from **object names** to block locations on a disk



在 Ceph 的元数据里，有一个叫 **onode**（object node）的结构。

它的作用是：

- 描述一个 **对象（object）的元数据**
    
- 记录对象的 **extent 映射**（逻辑数据对应存在哪些物理 blob / extent）
    
- 保存可能存在的 **内联数据（inline data）**
    
BlueStore采用了分层的数据结构设计：

#### Onode（对象节点）

- 每个对象对应一个Onode

- 包含对象的元数据信息（大小、时间戳等）

- 管理对象的ExtentMap（逻辑到物理地址映射）

#### Extent（扩展块）

- 表示逻辑地址到物理地址的映射关系

- 包含logical_offset、blob_offset、length等字段

- 关联到具体的Blob

#### Blob（数据块）

- 表示物理存储上的数据块

- 包含多个物理扩展（Physical Extents）

- 支持压缩、校验和等特性


BlueStore使用RocksDB作为元数据存储引擎，通过以下前缀组织不同类型的元数据：

- PREFIX_OBJ ("O"): 存储Onode元数据（对象元数据）

- PREFIX_SHARED_BLOB ("X"): 存储共享Blob元数据

- PREFIX_ALLOC ("B"): 存储分配器信息

- PREFIX_STAT ("T"): 存储统计信息

- PREFIX_COLL ("C"): 存储集合元数据

完整表示


| 组件 (Component)                | 角色与功能                                                                                                                  | 缓存与持久化                                                                                                 |
| ----------------------------- | ---------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| **Collection**                | **PG 的容器**。在 OSD 内部，一个 Collection 对应一个 Ceph 的**归置组 (PG)**。它本身元数据很少，主要是作为其内部所有对象的元数据（Onode）的**命名空间和集合容器**。              | 数目较少，**全部缓存在内存**中（`coll_map`）。元数据以 `PREFIX_COLL` 为前缀持久化在 RocksDB。                                      |
| **Onode (Object Node)**       | **单个对象的元数据**。每个存储在 BlueStore 的对象都有一个 Onode，记录该对象的**所有元信息**，如大小、属性、以及指向其数据结构的指针（最重要的就是指向 Extent/Blob 的映射关系）。            | **部分缓存**。存储在所属 Collection 的 `onode_map` 中。默认使用 **LRU 算法**管理内存中的缓存。完整元数据以 `PREFIX_OBJ` 为前缀持久化在 RocksDB。 |
| **Extent**                    | **逻辑扩展映射**。描述对象**逻辑空间**的一片连续区域（偏移 `off`，长度 `len`）。它并不直接指向物理磁盘，而是指向一个 **Blob**。一个 Onode 可能包含多个 Extent，这是因为对象可能被分段写入或更新。 | 是 Onode 元数据的一部分，**不独立缓存**。随 Onode 一起被加载或驱逐。                                                            |
| **Blob**                      | **物理映射层**。是连接**逻辑空间（Extent）** 和**物理空间（pextent）** 的桥梁。一个 Blob 将一片**在逻辑上连续**的数据，映射到可能**在物理上不连续**的多个磁盘块上。它负责处理**碎片化**问题。  | 是 Onode 元数据的一部分，**不独立缓存**。随 Onode 一起被加载或驱逐。                                                            |
| **pextent (Physical Extent)** | **物理磁盘块**。描述**裸磁盘设备**上的一片**真正连续**的物理空间（偏移 `off`，长度 `len`）。一个 Blob 包含一个或多个 pextent，从而将对象的逻辑数据块组装起来。                     | 信息存储在 Blob 中，是 Onode 元数据的一部分。**数据本身**直接写在磁盘设备上。                                                        |
| **Buffer**                    | **数据缓存**。缓存的是**对象的数据内容本身**，而不是元数据。以 `<offset>` 为键，缓存特定 Blob 在特定逻辑偏移处的数据块。目的是**加速读写**，避免频繁访问慢速磁盘。                       |                                                                                                        |


## 对象 vs extent 大小 & 映射记录数

以下是一个假设的例子，结合 BlueStore 的行为模式：

- 对象大小：假设对象 `O1` 是 100 MiB。
    
- `bluestore_min_alloc_size` = 4 KiB。
    
- 写入操作可能不是一次写满整个 100 MiB，而是分数次写入，比如 3 次：一次写 30 MiB，从 offset 0；一次写 40 MiB，从 offset 30 MiB；再写 30 MiB，从 offset 70 MiB。
    

在这种情况下，extent 映射可能会被拆成三个 extent，比如：

| extent 号 | logical offset | length | physical blob/extents / physical_start |
| -------- | -------------- | ------ | -------------------------------------- |
| E1       | 0              | 30 MiB | blob A @ start P1                      |
| E2       | 30 MiB         | 40 MiB | blob B @ start P2                      |
| E3       | 70 MiB         | 30 MiB | blob C @ start P3                      |



查询：

ceph-kvstore-tool bluestore-kv /var/lib/ceph/osd/ceph-0 list PREFIX_OBJ


返回：

```shell
Key: O0000000000000001
Value:
{
    "oid": "1.23.4567",
    "size": 1048576,
    "mtime": 1694668800,
    "extent_map": [
        {"logical_offset": 0, "blob": "B0001", "blob_offset": 0, "length": 524288},
        {"logical_offset": 524288, "blob": "B0002", "blob_offset": 0, "length": 524288}
    ],
    "inline_data": null
}

Key: O0000000000000002
Value:
{
    "oid": "1.23.4568",
    "size": 2097152,
    "mtime": 1694672400,
    "extent_map": [
        {"logical_offset": 0, "blob": "B0003", "blob_offset": 0, "length": 1048576},
        {"logical_offset": 1048576, "blob": "B0004", "blob_offset": 0, "length": 1048576}
    ],
    "inline_data": "HelloWorld"
}

```

## 1.3 隐藏区(你知道，别人不知道):正常写和Deferred写

> 别人哪有这么多时间停下来看代码


### 对象类的定义

![元数据](https://s2.loli.net/2025/09/14/nTuiRdMo7rXPeDq.png)


```c++

 /// an in-memory object
  struct Onode {
    Collection *c;
    ghobject_t oid; //对象标识符
    ExtentMap extent_map; //管理逻辑到物理地址的映射 可能多个
 }

  struct ExtentMap 
  {
    Onode *onode;
    extent_map_t extent_map;        ///< map of Extents to Blobs

    //专业含义：lextent 主映射容器（按对象逻辑偏移有序），

    // 记录每个逻辑区间（Extent）对应到哪个 Blob 以及在 Blob 内的偏移与长度，

    // 是读写路径的核心索引。

    //小白版：一本“目录表”，把“对象里的哪一段”（Extent）

    // 指向“磁盘上的哪块数据”（Blob 的某段）。读数据就靠它找到磁盘位置。

    blob_map_t spanning_blob_map;   ///< blobs that span shards

    };
```



完整调用链：

![image.png](https://s2.loli.net/2025/09/14/tb3axe7OBq4Gc8Y.png)




### BlueStore确实分为正常写和Deferred写：



## **1. 正常写入流程**

正常写入 = **直接分配空间 + 立即写入数据**。

### **流程步骤**

1. **写入请求处理**
    
        
``` c++
void _do_write_data(TransContext *txc,

	  CollectionRef& c,
	
	  OnodeRef& o,
	
	  uint64_t offset,
	
	  uint64_t length,
	
	  ceph::buffer::list& bl,
	
	  WriteContext *wctx); 
```
        
2. **空间分配**
    
    - 从allocator分配物理空间
        
3. **数据写入**
    
    - 将数据写入分配的物理区域
        
    - 通过 `bdev->aio_write()` 直接写入块设备 //当启用libaio且使用直接IO
        
4. 元数据更新 (_wctx_finish)

    
	- 立即更新ExtentMap（逻辑到物理的映射）
	
	- 更新Onode元数据
	
	- 提交事务到KV存储
        
```c++
  void _wctx_finish(
	TransContext *txc,
	CollectionRef& c,
	
	OnodeRef& o,
	
	WriteContext *wctx,
	
	std::set<SharedBlob*> *maybe_unshared_blobs=0);
	
	
 void BlueStore::_txc_state_proc(TransContext *txc) {
    switch (txc->get_state()) 
    {
    case TransContext::STATE_AIO_WAIT:
        // 等待所有异步IO完成
        if (txc->ioc.has_pending_aios()) {
            return; // 继续等待
        }
        // 所有IO完成，进入下一步
        
    case TransContext::STATE_IO_DONE:
        _txc_finish_io(txc);  // 完成IO处理
        txc->set_state(TransContext::STATE_KV_QUEUED);
        // 进入元数据更新阶段
        
    case TransContext::STATE_KV_QUEUED:
        // 提交元数据更新到KV存储
        _txc_apply_kv(txc, sync_submit_transaction);
        break;
    }
}


    }
```


###  延迟写入（Deferred Write）流程

在Ceph BlueStore的延迟写入（Deferred Write）流程中，

**延迟IO数据**确实被封装在键值事务中写入RocksDB，

>数据确实会写入RocksDB，不是仅仅存储在内存中。 这是BlueStore延迟写入机制的核心设计，确保了数据的持久性和系统的一致性。

![](https://s2.loli.net/2025/09/14/5ZmH124XNUrLYyD.png)


然后给客户端返回应答，最后在后台执行实际的磁盘写入操作

### 延迟写入的事务封装

在延迟写入场景中，

系统会创建`bluestore_deferred_op_t`操作对象来封装延迟IO数据 。

**File:** src/os/bluestore/Writer.cc (L771-774)
```c
bluestore_deferred_op_t *op = bstore->_get_deferred_op(txc, data.length());
op->op = bluestore_deferred_op_t::OP_WRITE;
op->extents = disk_extents;
op->data = data;
```

- op->data中的数据暂时存储在内存中，作为bluestore_deferred_op_t结构的一部分
- **只有延迟事务的元数据（包括操作类型、磁盘位置等）会被编码并写入RocksDB**

啥意思：数据要写入位置，和数据一块被编码 


```c
DENC(bluestore_deferred_op_t, v, p) {
  DENC_START(1, 1, p);
  denc(v.op, p);        // 操作类型
  denc(v.extents, p);   // 磁盘位置信息
  denc(v.data, p);      // 实际数据内容 ← 这里！
  DENC_FINISH(p);
}
```

### 键值事务处理

延迟写入数据通过`TransContext`的键值事务进行管理。

`TransContext`包含一个`KeyValueDB::Transaction t`字段，用于将延迟操作存储到RocksDB中。

```c
DENC(bluestore_deferred_transaction_t, v, p) {
  DENC_START(1, 1, p);
  denc(v.seq, p);
  denc(v.ops, p);       // 包含所有延迟操作，包括数据
  denc(v.released, p);
  DENC_FINISH(p);
}
```
#### 写入RocksDB的过程

在BlueStore.cc:15705：
```c++
if (txc->deferred_txn) {

  txc->deferred_txn->seq = ++deferred_seq;

  bufferlist bl;

  encode(*txc->deferred_txn, bl);  // 编码整个事务，包括数据

  string key;

  get_deferred_key(txc->deferred_txn->seq, &key);

  txc->t->set(PREFIX_DEFERRED, key, bl);  // 写入RocksDB

}
```

 为什么数据要写入RocksDB？

1. 持久化保证：确保系统崩溃后能够恢复未完成的延迟写入

2. 事务完整性：延迟写入作为事务的一部分，需要持久化

3. 恢复机制：系统重启后可以从RocksDB中恢复并重放延迟写入


### 客户端应答时机

- 系统在将延迟写入操作提交到RocksDB后，会立即给客户端返回成功应答

### 后台执行机制

- 专门的线程在后台执行实际的磁盘写入。



# 总结：


BlueStore的写入策略是：

- 正常写入: 大块数据直接分配空间，立即写入和更新元数据

- 延迟写入: 小块数据优先重用现有Blob，通过读取-修改-写入的方式处理，延迟提交到存储设备

## **延迟写入的数据流转**

1. **用户数据进入**
    
    - 写入请求到来，数据先写入 `bluestore_deferred_op_t.data`
        
2. **封装事务**
    
    - 编码到 `bluestore_deferred_transaction_t`
        
3. **持久化到 RocksDB**
    
    - 写入 **RocksDB**，带前缀 `PREFIX_DEFERRED`
        
    - 这样即使宕机，延迟写入操作也不会丢
        
4. **系统重启时恢复**
    
    - BlueStore 从 RocksDB 的 `PREFIX_DEFERRED` 里读取未完成事务
        
    - 重新加载到内存
        
5. **最终落盘**
    
    - 延迟的数据被调度执行
        
    - 通过 `bdev->aio_write()` 写入物理磁盘设备

# 参考
## 1. 看资料提出疑问，然后当天找人解决


- 提出 3个疑问 模糊，只认识字，不懂含义，
- 自己解决 1了，
- 找人帮忙解决 0个
- 剩余2个
- 找人讨论大于个人独自研究

- BlueStore数据组织形式及缓存 https://blog.csdn.net/weixin_43778179/article/details/132735517
- BlueStore 的演讲资料：LUESTORE: A NEW STORAGE BACKEND FOR CEPH


#### 疑问1： Object 和 Collection 是什么？

#### 疑问2： All writes are transactions
- Atomic + Consistent + Durable 怎么保证原子性的
-  TRANSCONTEXT STATE MACHINE
![image.png](https://s2.loli.net/2025/09/14/UnbS5yOxijVq3T4.png)


#### 疑问3 文件，对象，块 怎么存储到同一个系统中呢？ 已解决


## 2.  参考电子书（关注，回复1 免费领取）
- Ceph设计原理与实现

## 3. 使用工具()
- DeepWiki 是由 Cognition Labs 公司推出的一款专为开发者打造的免费的 AI 驱动 GitHub 源码阅读与分析神器。
- https://deepwiki.com/ceph/ceph


### 最动人的作品，为自己而写，刚刚好打动别人


1️⃣ 加入我的技术交流群Offer 来碗里 (回复“面经”获取），一起抱团取暖 
![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)
2️⃣ 关注公众号:后端开发成长指南(回复“面经”获取）获取过去我**全部面试录音**和大厂面试复盘攻略
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)
3️⃣  回复 面经 获取全部电子书 或者购买正版书籍

[分布式必读经典书籍](https://mp.weixin.qq.com/s/y94rXLoJCllCXkhfL7d7Hg)

----------------我是黄金分割线-----------------------------

#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，打通任督二脉。
- 但是不要给自己这样假设：别人完成就等着自己完成了，这个逃避问题表现,裁员时候别人不会这么想。
##### 低头走路：
- 一次专注做好一个小事。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 ,把基本事情做好。
