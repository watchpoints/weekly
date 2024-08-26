---
title: "TiKV Contributors day5"
date: 2024-08-19
description: "Tidb"
draft: false
tags: ["Tidb"] 
---







tikv_kv % tree     
.
├── Cargo.toml
└── src
    ├── btree_engine.rs
    ├── cursor.rs
    ├── lib.rs
    ├── raftstore_impls.rs
    ├── rocksdb_engine.rs
    └── stats.rs





目标：那https://github.com/tikv/tikv/pull/5541

为例子， 看别人如何写代码的 把自觉当作review，







## 背景:写作的目的

认同开源，但是迟迟不提交一个有价值pr很难，看不到成果，这是我遇到三个问题

1. 缺少资料，看着都是一头雾水，可能是外行原因  无法小白语言 描述项目是什么，遇到什么问题，怎么解决的，都是大量名词解释。


2. 不更重要是面对百万行代码项目之间吓，不会抽象，不会划分具体模块


3. 慢慢看 结果就是不看这是自欺欺人。



要认清一个事实：你怎么看业余 ，不像tidb公司内部人员一样 场景，业务驱动都有效支持。

怎么办？下面是我采取方式

> 管中窥豹，坐井观天  虽然是贬义词，
>
> 因为足够小，才可以足够行动，才人人参与。
>
> 投入时间 3周 写一篇，这样不会自觉很多压力。
>
> 因此读别人提交pr ，然后写出自己理解





https://github.com/tikv/tikv/pull/5541



https://github.com/tikv/tikv/pull/9600



https://github.com/tikv/tikv/pull/14486



https://github.com/tikv/tikv/issues/8944

https://github.com/tikv/tikv/pull/16309



 

https://github.com/tikv/tikv/pull/16253



https://github.com/tikv/tikv/issues/17309







### engine_rocks: log SST corruption reason #16253

https://github.com/tikv/tikv/pull/16253

https://github.com/tikv/tikv/issues/16308



- 了解概念

<div  style="border: 2px solid  #add8e6;font-size: 17px; align-items: center;">  
https://github.com/facebook/rocksdb/wiki 是由 Facebook 基于 LevelDB 开发的一款提供键值存储与读写功能的 LSM-tree 架构引擎。
用户写入的键值对会先写入磁盘上的 WAL (Write Ahead Log)，然后再写入内存中的跳表（SkipList，这部分结构又被称作 MemTable）。
 LSM-tree 引擎由于将用户的随机修改（插入）转化为了对 WAL 文件的顺序写，因此具有比 B 树类存储引擎更高的写吞吐。
内存中的数据达到一定阈值后，会刷到磁盘上生成 SST 文件 (Sorted String Table)，
 SST 又分为多层（默认至多 6 层），
每一层的数据达到一定阈值后会挑选一部分 SST 合并到下一层，每一层的数据是上一层的 10 倍（因此 90% 的数据存储在最后一层） 
</div>




https://github.com/facebook/rocksdb/wiki/RocksDB-Overview#3-high-level-architecture

![img](https://user-images.githubusercontent.com/62277872/119747261-310fb300-be47-11eb-92c3-c11719fa8a0c.png)



The three basic constructs of RocksDB are **memtable**, **sstfile** and **logfile**. 

The [**memtable**](https://github.com/facebook/rocksdb/wiki/MemTable) is an in-memory data structure - new writes are inserted into the *memtable* and are optionally written to the [**logfile** (aka. Write Ahead Log(WAL))](https://github.com/facebook/rocksdb/wiki/Write-Ahead-Log-(WAL)). 

The logfile is a sequentially-written file on storage. When the memtable fills up, it is flushed to a [**sstfile**](https://github.com/facebook/rocksdb/wiki/Rocksdb-BlockBasedTable-Format) on storage and the corresponding logfile can be safely deleted. The data in an sstfile is sorted to facilitate easy lookup of keys.



- 阅读题目可以做到

~~~R

[2023/12/27 04:41:51.140 +08:00] [WARN] [event_listener.rs:127] ["detected rocksdb background error"] [err="Corruption: block checksum mismatch: stored = 2981909476, computed = 324654415, type = 1  in /data1/data/db/38457359.sst offset 8497742 size 3971"] [sst=/38457359.sst]

[2023/12/27 04:41:52.381 +08:00] [WARN] [store.rs:243] ["detected damaged regions overlapping damaged file ranges"] [id="{592132084, 442186929, 607655650}"]

[2023/12/27 04:42:01.909 +08:00] [FATAL] [lib.rs:509] ["Failed to recover sst file: /38457359.sst, error: file still exists, it may belong L0, damaged_files:[name:\"/38457359.sst\", smallest_key:[122, 116, 128, 0, 
             at /home/jenkins/agent/workspace/build-common/go/src/github.com/pingcap/tikv/components/tikv_util/src/lib.rs:508:18
   1: <alloc::boxed::Box<F,A> as core::ops::function::Fn<Args>>::call
             at /rust/toolchains/nightly-2022-11-15-x86_64-unknown-linux-
   4: rust_begin_unwind
~~~



## 参考

[1] https://docs.pingcap.com/zh/tidb/stable/rocksdb-overview



