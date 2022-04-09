---
title: "OBCP考试2"
date: 2022-04-24
draft: false
categories: ["code_reading"]
---



> 多看、多问、多总结，肯定是可以攻克的。



麻烦点评，第8章，甘洛

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

- [x] [ppt 第四章：OB SQL 调优](https://gw.alipayobjects.com/os/bmw-prod/439ef28e-5d44-460c-b85a-1dd7d5770e61.pdf)

  - [x] 实验：https://www.yuque.com/yiyezhou/rgak0e/cl41r7
- [x] [视频：OceanBase 社区版入门到实战教程](https://open.oceanbase.com/blog/10900164?currentPage=1) 

- [x]    7.1 OceanBase 数据库 SQL 诊断和优化

​        视频地址： https://open.oceanbase.com/docs/videoCenter/5900015'

- [ ] pdf 对应的doc：
- 4.4 局部索引与全局索引 5.data-distribution-and-link-management/1.partition-table-and-partitioned-index-management/6.create-an-index-on-a-partition-table/1.local-index.md ✅
- docs/docs/docs-cn/6.administrator-guide/5.data-distribution-and-link-management/1.partition-table-and-partitioned-index-management/6.create-an-index-on-a-partition-table/2.global-index.md ✅

- docs/docs/docs-cn/12.sql-optimization-guide-1/4.sql-optimization-1/6.manage-execution-plans-1/1.optimizer-hint-1.md  ✅
- docs/docs/docs-cn/12.sql-optimization-guide-1/4.sql-optimization-1/6.manage-execution-plans-1/2.plan-binding-1.md ✅



- [ ] OceanBase 社区版入门教程第九期 如何快速拿下 OBCA & OBCP 认证

  https://open.oceanbase.com/blog/10900233

### output：

- obclient -uroot@tpcc -h127.0.0.1 -P2881 -p123456 -Dtpcc -c

- 全局索引允许指定自己的分区规则和分区个数，不一定需要跟表分区规则保持一致
- 想利用局部索引建立关于emp_name的唯一约束是无法实现的 .并且会在全局范围内造成CPU和IO资源的浪费
- 局部索引与全局索引的执行计划的比较
- 如果需要“不包含完整分区键”的唯一约束， 1. 用全局索引 2. 或者本地索引，且需要索引列上必须带上表的分区键
- Hint只影响数据库优化器生成计划的逻辑，而不影响SQL语句本身的语义



### 3.小总





## 挑战02 OB 分布式事务高级技术



## ## 阅读资料



- [x] https://www.cnblogs.com/ZhangZiSheng001/p/15727027.html

- [x] 阅读 https://gw.alipayobjects.com/os/bmw-prod/ab80d72a-801b-4bb4-959f-81a6fa282485.pdf

- [x] 阅读 **Paxos + 两阶段提交协议** docs/docs/docs-cn/5.overview/5.transaction-management-1/5.distributed-transactions.md

- [x] https://gw.alipayobjects.com/os/bmw-prod/d5635d4b-9d2a-46c8-bab3-7527195f8635.pdf p67

- [x] 阅读：https://zhuanlan.zhihu.com/p/78402011

  



输出

- OceanBase数据库是利用一个集中式服务来提供全局一致的版本号。虽然集中 但是不会出现单点故障。
- OceanBase两阶段提交协议 :. 业务数据模型设计原则：尽量避免跨机分布式事务

![image.png](https://s2.loli.net/2022/03/30/EMsyK2nxpY9VUjb.png)



- 让每个分布式事务的第一个参与者承担两阶段提交协议中协调者的工作
- 选举出另外一个副本代替原有参与者继续提供服务，并恢复原有参与者的状态





### 测试

~~~
obclient -uroot@tpcc -h127.0.0.1 -P2881 -p123456 -Dtpcc -c
ALTER SESSION SET ISOLATION_LEVEL = SERIALIZABLE
select /*+read_consistency(weak)*/ * from test where c1=1;
set @@ob_read_consistency=2，其中1=FROZEN、2=WEAK、3=STRONG；

set @@session.ob_query_timeout = 900000000;
set @@session.ob_trx_timeout = 900000000;
set @@session.ob_trx_idle_timeout = 300000000;

SHOW VARIABLES LIKE 'tx_isolation';
+---------------+----------------+
| Variable_name | Value          |
+---------------+----------------+
| tx_isolation  | READ-COMMITTED |



ALTER SYSTEM ALTER ZONE zone1 SET REGION='beijing',IDC='z1';

select * from oceanbase.__all_zone;
OceanBase 3.1.1

 select * from __all_zone;
~~~





### day3:



### 问题是什么

- 假如客户端连接ob，如果leader挂掉，客户端不知道请求是否成功还是失败？重复请求 ob怎么处理

   https://open.oceanbase.com/ask/detail?id=28800053

- 

### 如何解决的

- [x]  阅读  [7.2 OBProxy SQL 路由原理](https://open.oceanbase.com/docs/tutorials-cn/V1.0.0/10000000000012279)

- [x]  obproxy实验 p72

- [x]  [OceanBase弹性伸缩和分区负载均衡](https://www.modb.pro/video/389) 杨传辉（日照)

  杨传辉（日照） -《OceanBase弹性伸缩和分区负载均衡》 —— 墨天轮视频：https://www.modb.pro/video/389?slink

- [x]  阅读：https://mp.weixin.qq.com/s/m3p5XI-7HbFZd8PPYEwybw

- [ ]  阅读：[同样是一致性协议，Paxos 和 Raft 的区别是什么?](https://open.oceanbase.com/ask/detail?id=167&search=%E5%BC%B1%E4%B8%80%E8%87%B4%E8%AF%BB%E5%8C%BA&pageNum=1)

  

  

### 有什么收获





~~~
grep -i 'slow query' obproxy.4147.log | sed "s/, /,\n/g"
https://www.cnblogs.com/tureno/articles/6677942.html
~~~





### Day4:[第七章：OB 备份恢复](https://gw.alipayobjects.com/os/bmw-prod/961e4de9-6a6f-4c1c-8f86-62e3901c0499.pdf)



### 问题是什么？

#### 如何解决的

- [x]  [ppt：](https://gw.alipayobjects.com/os/bmw-prod/961e4de9-6a6f-4c1c-8f86-62e3901c0499.pdf)

- [x] [OceanBase 社区版入门到实战教程 |视频合辑](https://open.oceanbase.com/blog/10900164?currentPage=1)

  - #### **4.如何迁移数据到 OceanBase 社区版**

  - 5.3 [如何对 OceanBase 集群进行备份]()

- [x]  实验 p77 https://gw.alipayobjects.com/os/bmw-prod/d5635d4b-9d2a-46c8-bab3-7527195f8635.pdf



#### 有什么收获

- nothing



## Day5:第8章节：



### 1. 问题

[问：Clog 盘满问题运维处理方法](https://open.oceanbase.com/ask/detail?id=13200020&search=clog%20slog%20ilog&pageNum=2)



### 2. 如何解决的



~~~
show grants for root; //登录用户名，不是租户
GRANT ALL ON db_name.tbl_name
iostat -x -k 1

Clog 盘满问题：数据不变，提高上线
ALTER SYSTEM SET clog_disk_usage_limit_percentage=98 server='svr_ip:2882';

SHOW parameters LIKE 'clog_disk_usage_limit_percentage';
SELECT svr_ip,svr_port,count(*) FROM __all_virtual_clog_stat WHERE is_offline = 0 AND is_in_sync = 0 GROUP BY 1;


[root@spa-65-177-112 store]# du -sh *
64M	clog
12M	ilog
148M	slog
404G	sstable

开始回收：reuse clog files
show parameters like 'clog_disk_utilization_threshold';
具体是否会瘦查看__all_rootservice_event_history】
将降低磁盘占用空间。

ALTER SYSTEM SET clog_disk_utilization_threshold=80

ALTER SYSTEM SET clog_disk_utilization_threshold=20

ALTER SYSTEM SET  enable_syslog_recycle=true
ALTER SYSTEM SET  max_syslog_file_count=20;
~~~

- [ ] OceanBase 社区版入门教程第九期 如何快速拿下 OBCA & OBCP 认证

摘要：

- Clog 落盘
- **强一致**：发生脑裂、网络分区、宕机、磁盘故障等异常时，在 Clog 写入位置上保证数据强一致，不丢失数据。



- [ ] [0133.O OceanBase社区版clog使用空间调整](https://zhuanlan.zhihu.com/p/466337239)

​         [可靠分布式系统-paxos的直观解释](https://mp.weixin.qq.com/s/DlhpF0Vu5agtrR2Jx2cynQ)

​    摘要：

- 一份数据存多份. 多副本保证了可靠性, 而副本之间的一致, 就需要paxos这类分布式一致性算法来保证.
- **主从异步复制**是最简单的策略之一, 它很容易实现, 但存在一个问题: 客户端收到一个**数据已经安全**(OK)的信息, 跟**数据真正安全**(数据复制到全部的机器上)在时间上有一个空隙,
-  **主从同步复制**，缺点就是整个系统中有任何一个机器宕机, 写入就进行不下去了
- **半同步复制**



### 3. 收获

