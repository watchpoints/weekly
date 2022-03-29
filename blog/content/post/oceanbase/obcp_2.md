---
title: "OBCP考试2"
date: 2022-02-20
draft: false
categories: ["code_reading"]
---



> 多看、多问、多总结，肯定是可以攻克的。



## 环境说明

- 单机单节点部署

~~~shell
obd cluster display test
+---------------------------------------------+
|                   observer                  |
+-----------+---------+------+-------+--------+
| ip        | version | port | zone  | status |
+-----------+---------+------+-------+--------+
| 127.0.0.1 | 3.1.1   | 2881 | zone1 | active |
+-----------+---------+------+-------+--------+

 /etc/sysctl.conf 
 fs.aio-max-nr = 1048576 ##文件系统最大异步io
 fs.file-max = 6815744 ##文件系统中文件的最大个数
~~~





## 挑战01 阅读资料

### 1. 疑问与思考



- 索引不是key，如何保证唯一性？

### 2. input 

- [ ] [ppt 第四章：OB SQL 调优](https://gw.alipayobjects.com/os/bmw-prod/439ef28e-5d44-460c-b85a-1dd7d5770e61.pdf)

  - [ ] 实验：https://www.yuque.com/yiyezhou/rgak0e/cl41r7
- [ ] [视频：OceanBase 社区版入门到实战教程](https://open.oceanbase.com/blog/10900164?currentPage=1) 

- [ ]    7.1 OceanBase 数据库 SQL 诊断和优化

​        视频地址： https://open.oceanbase.com/docs/videoCenter/5900015'

- [ ] pdf 对应的doc：
- 4.4 局部索引与全局索引 5.data-distribution-and-link-management/1.partition-table-and-partitioned-index-management/6.create-an-index-on-a-partition-table/1.local-index.md ✅
- docs/docs/docs-cn/6.administrator-guide/5.data-distribution-and-link-management/1.partition-table-and-partitioned-index-management/6.create-an-index-on-a-partition-table/2.global-index.md ✅

- docs/docs/docs-cn/12.sql-optimization-guide-1/4.sql-optimization-1/6.manage-execution-plans-1/1.optimizer-hint-1.md  ✅
- docs/docs/docs-cn/12.sql-optimization-guide-1/4.sql-optimization-1/6.manage-execution-plans-1/2.plan-binding-1.md

### output：

- obclient -uroot@tpcc -h127.0.0.1 -P2881 -p123456 -Dtpcc

- 全局索引允许指定自己的分区规则和分区个数，不一定需要跟表分区规则保持一致
- 想利用局部索引建立关于emp_name的唯一约束是无法实现的 .并且会在全局范围内造成CPU和IO资源的浪费
- 局部索引与全局索引的执行计划的比较
- 如果需要“不包含完整分区键”的唯一约束， 1. 用全局索引 2. 或者本地索引，且需要索引列上必须带上表的分区键
- Hint只影响数据库优化器生成计划的逻辑，而不影响SQL语句本身的语义



### 3.小总







## 

