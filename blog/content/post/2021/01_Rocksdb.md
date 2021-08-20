---
title: "跟leveldb学C++，跟rocksdb学做引擎"
date: 2021-08-02
description: "跟leveldb学C++，跟rocksdb学做引擎"
draft: false
tags: ["book"]
---



##  

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

### 二、这个技术的优势和劣势分别是什么 

### 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

### 四、技术的组成部分和关键点。

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比



# rocksdb





# # leveldb



- 代码位置：/Users/wangchuanyi/doc/daily_read_coding/leveldb
- [Leveldb之Put实现](https://kernelmaker.github.io/Leveldb_Put)



leveldb的写入速度很快，主要因为它将随机写转化为顺序写，

一个写入操作会首先在log文件中顺序写oplog，然后在内存的memtable中插入该数据就可返回

疑问：



1. 随机写转化为顺序写 怎么转化，用户write可任意位置【10，30，1，80】，追加位置，【1，2，3，4，5】
2. 内存的memtable是cache吗？



```

DBImpl::Put -> DB::Put -> DBImpl::Write

/Users/wangchuanyi/doc/daily_read_coding/leveldb/db/db_impl.cc

```



### 3 LevelDB之LSM-Tree

https://zouzls.github.io/2016/11/23/LevelDB%E4%B9%8BLSM-Tree/



- LSM被设计来提供比传统的B+树或者ISAM更好的写操作吞吐量，通过消去随机的本地更新操作来达到这个目标。

那么为什么这是一个好的方法呢？这个问题的本质还是磁盘随机操作慢，顺序读写快的老问题。这二种操作存在巨大的差距，无论是磁盘还是SSD。







## ref

- https://kernelmaker.github.io/
- 













