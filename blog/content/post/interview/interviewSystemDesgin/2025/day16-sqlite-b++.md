---
title: 面试必备之深入理解thread local
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
> 知识地图：分布式数据库-- sqlite


参考来源：codedump
## 小思考




# 笔记

## sqlite3.36版本 btree实现（零）- 起步及概述


来源：https://www.codedump.info/post/20211217-sqlite-btree-0/

Leveldb这个项目在LSM领域属于入门级别的生产级实现，即这个领域最精简、但是又能放心在某些要求不高的场景下用于生产的项

### sqlite的btree架构概述

下面简单描述一下sqlite的btree架构，从高往低大体分为以下几个部分：
![image.png](https://s2.loli.net/2025/07/21/WxiJU4HsvOSNFqZ.png)
- 页面管理：顾名思义，页面管理模块的最基本单位是”页面“，页面的读、写、缓存、落盘、恢复、回滚等，都由页面模块负责。上一层依赖页面管理模块的btree模块，不需要关心一个页面何时缓存、何时落盘等细节。即：**页面模块负责页面的物理级别的操作**。页面管理模块：btree存储引擎，其操作文件的最基本单位就是页

btree：
- 负责按照btree算法，来组织页面，即负责的是页面之间逻辑关系维护。
- 除此以外，一个页面内部的数据的物理、逻辑组织，也是btree模块来负责的

![页面管理模块”无疑是这里最大最复杂的部分](https://www.codedump.info/media/imgs/20211217-sqlite-btree-0/page-module.png)
那些很成熟的存储引擎，都是怎么处理崩溃恢复问题的呢，比如写数据落盘到一半，进程崩了，该如何恢复呢？
##  sqlite3.36版本 btree实现（一）- 管理页面缓存
- https://www.codedump.info/post/20211217-sqlite-btree-1-pagecache/
![](https://www.codedump.info/media/imgs/20211217-sqlite-btree-1-pagecache/pagecache.png)

![](https://www.codedump.info/media/imgs/20211217-sqlite-btree-1-pagecache/pagecache_module.png)
 页面所在的数据结构

缓存中的页面，可能存在于以下三种数据结构中：

- 脏页面链表：该链表维护所有当前在使用的页面，由“页面缓存管理器”维护。
- hash数组：作用是以页面编号为键来查询页面，由默认的“页面缓存算法”来维护。
- LRU链表：越是常被访问的页面，在LRU链表中就越往前，从LRU链表中淘汰数据都是从链表尾部开始的，也是由默认的“页面缓存算法”来维护
![](https://www.codedump.info/media/imgs/20211217-sqlite-btree-1-pagecache/page_ds.png)
### 参考
