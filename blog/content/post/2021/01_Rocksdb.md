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



### 第一天

ubuntu

```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local/rocksdb ..
make && make install
```





查看：

virtual void Get(const LookupKey& k, void* callback_args,

Status DBImpl::GetImpl



RocksDB如何在sstable中查找到对应的数据



我们来看一下如何针对磁盘上的SST进行搜索：

- 首先我们需要遍历搜索Level 0(L0)层所有的文件，假设没有找到则继续
- 我们来到了L1层，该层数据文件是有序的，每一个文件都会有一个FileMetaData保存着该文件的最小key和最大key，同时还有IndexUnit。我们通过对L1层文件进行二分查找来找到第一个largest key大于要查找key的文件。我们继续对此SST文件内的key进行查找，假如没有找到，则继续。理论上到L1层我们可以再次进行二分查找来找到对应的key，但是借助IndexUnit我们可以快速过滤不需要查找的文件，提升查找速度。这里还有一点，假设要查找的key小于第一个文件的smallest key，则基准文件为第一个文件；假设要查找的key大于最后一个文件的largest key，则基准文件为最后一个文件。针对于key与基准文件key范围的比较结果，我们又有以下的几种场景：
  - 当前要查找的key小于基准文件的smallest key。这种场景下，假设基准文件为该层第一个文件则我们需要查找的文件范围为 `[First file on L(N+1), smallest_rb of current sst]`;若不是第一个文件，则要查找的范围为`[largest_lb of previous sst, smallest_rb of current sst]`。
  - 当前要查找的key在基准文件的smallest key和largest key范围内。这种场景下，我们要查找的文件范围为 `[smallest_lb of current sst, largest_rb of current sst]`。
  - 当前要查找的key大于基准文件的largest key。这种场景下，唯一的可能性为基准文件为该层的最后一个文件，则我们需要查找的文件范围为`[largest_lb of current sst, Last file on L(N+1)]`
- 针对上层的范围，对下层的SST进行搜索





### 肩膀：为了补充基础 我阅读过的文档

1. 基于leveldb谈谈MVCC多版本控制 https://yuerblog.cc/2017/09/26/leveldb-mvcc/ ，[10分]
   - https://yuerblog.cc/wp-content/uploads/leveldb%E5%AE%9E%E7%8E%B0%E8%A7%A3%E6%9E%90.pdf 【10分、
  
2. MySQL · RocksDB · 数据的读取(一)http://mysql.taobao.org/monthly/2018/11/05/ 【10分--开始】
   
   
   
   MySQL · RocksDB · 数据的读取(二)
   
   

http://mysql.taobao.org/monthly/2018/12/08/

https://gocode.cc/project/13/article/187

3. [leveldb源码学习5-dbformat和comparator](http://zrainy.top/2019/12/11/leveldb%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A05-dbformat%E5%92%8Ccomparator/)











# # leveldb 学习方法





https://github.com/google/leveldb

git@github.com:watchpoints/leveldb.git



### 1. 查看别人的文档 



~~~
git clone git@github.com:watchpoints/leveldb.git
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
~~~



- 代码位置：/Users/wangchuanyi/doc/daily_read_coding/leveldb

- [Leveldb之Put实现](https://kernelmaker.github.io/Leveldb_Put)

- https://www.cnblogs.com/xueqiuqiu/p/8296324.html

- # [LevelDB的源码阅读（一）](https://www.cnblogs.com/xueqiuqiu/p/8287008.html)

- # [LevelDB的源码阅读（二） Open操作](https://www.cnblogs.com/xueqiuqiu/p/8289046.html)

- https://www.cnblogs.com/xueqiuqiu/p/8268814.html

- https://wingsxdu.com/post/database/leveldb/#gsc.tab=0

~~~

项目结构
db/, 数据库逻辑
doc/, MD文档
helpers/, LevelDB内存版, 通过namespace覆盖
port/, 平台相关代码
table/, LSM有关的
LevelDB选择用跳跃表（skiplist）实现memtable和immemtable, 用有序行组来实现SSTable。
~~~





leveldb的写入速度很快，主要因为它将随机写转化为顺序写，

一个写入操作会首先在log文件中顺序写oplog，然后在内存的memtable中插入该数据就可返回

疑问：



1. 随机写转化为顺序写 怎么转化，用户write可任意位置【10，30，1，80】，追加位置，【1，2，3，4，5】
2. 内存的memtable是cache吗？



```

DBImpl::Put -> DB::Put -> DBImpl::Write

/Users/wangchuanyi/doc/daily_read_coding/leveldb/db/db_impl.cc

Status DBImpl::Write(


```



### 3 LevelDB之LSM-Tree

https://zouzls.github.io/2016/11/23/LevelDB%E4%B9%8BLSM-Tree/



- LSM被设计来提供比传统的B+树或者ISAM更好的写操作吞吐量，通过消去随机的本地更新操作来达到这个目标。

那么为什么这是一个好的方法呢？这个问题的本质还是磁盘随机操作慢，顺序读写快的老问题。这二种操作存在巨大的差距，无论是磁盘还是SSD。







1. 问：结构是什么？



- memtable 的核心结构就是用了跳表

  elasticsearch 的高速搜索就是基于跳表,

- sstable 全名 sort-string table, bigtable 使用的存储技术. 顾名思义, sstable 中的数据都是有序的










阅读：[LSM-Tree 与 LevelDB 的原理和实现 · Analyze](https://wingsxdu.com/post/database/leveldb/#gsc.tab=0)



![c0是什么](https://wingsxdu.com/post/database/leveldb/Two-Component-LSM-Tree@2x.png)



- 两组件 LSM-Tree（Two-Component LSM-Tree）在内存中有一个 C0 组件，它可以是 AVL 或 SkipList 等结构，所有写入首先写到 C0 中
- 中 Level-0 层比较特殊，它是由 ImMemTable 直接 dump 到磁盘中形成的，每个文件大小约为 4MB
- 每个 SSTable 文件大小为 2MB，Level1 的文件总大小为 10MB，随后每一级都比上一级大 10 倍，Level6 达到 1TB。



![](https://wingsxdu.com/post/database/leveldb/LevelDB-Minor-Compaction@2x.png)



void DBImpl::CompactMemTable()





## 第一天输出：





















