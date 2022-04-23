---
title: "OBCP准备"
date: 2022-04-09
draft: false
categories: ["code_reading"]
---



> 多看、多问、多总结，肯定是可以攻克的。



文章地址：https://wangcy6.github.io/post/oceanbase/obcp_2/





![img](https://gw.alipayobjects.com/zos/oceanbase/06cb789a-b615-43df-9f3f-05b23126b737/image/2022-01-04/5eb552d4-dcfb-4239-892b-7277c9edd400.png)



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



~~~
alter system set sql_audit_memory_limit = '3G';
alter system set enable_sql_audit = true;
desc gv$sql_audit;
desc oceanbase.gv$plan_cache_plan_stat;
set ob_enable_trace_log =1
set ob_enable_trace_log = 1;
show trace ;
MySQL [oceanbase]> select * from gv$plan_cache_plan_explain;

select *  from v$plan_cache_plan_stat 
                          where tenant_id= 1001 
                                and statement like 'insert into t1 values%'\G
select * 
                            from v$plan_cache_plan_explain
                            where tenant_id = 1001 and plan_id = 7;
                            
查看集群 SQL 请求流量是否均衡 


第一次执行：


MySQL [oceanbase]> select count(*) from t2;
+----------+
| count(*) |
+----------+
|        0 |
+----------+
1 row in set (0.009 sec)

MySQL [oceanbase]> show trace \G;
*************************** 1. row ***************************
   Title: process begin
KeyValue: in_queue_time:46, receive_ts:1650083124563031, enqueue_ts:1650083124563040
    Time: 0
*************************** 2. row ***************************
   Title: query begin
KeyValue: trace_id:YB427F000001-0005D94DCC13DCFD
    Time: 4
*************************** 3. row ***************************
   Title: parse begin
KeyValue: stmt:"select count(*) from t2", stmt_len:23
    Time: 144
*************************** 4. row ***************************
   Title: pc get plan begin
KeyValue: 
    Time: 12
*************************** 5. row ***************************
   Title: pc get plan end
KeyValue: 
    Time: 27
*************************** 6. row ***************************
   Title: transform_with_outline begin
KeyValue: 
    Time: 2
*************************** 7. row ***************************
   Title: transform_with_outline end
KeyValue: 
    Time: 111
*************************** 8. row ***************************
   Title: resolve begin
KeyValue: 
    Time: 101
*************************** 9. row ***************************
   Title: resolve end
KeyValue: 
    Time: 498
*************************** 10. row ***************************
   Title: transform begin
KeyValue: 
    Time: 167
*************************** 11. row ***************************
   Title: transform end
KeyValue: 
    Time: 500
*************************** 12. row ***************************
   Title: optimizer begin
KeyValue: 
    Time: 8
*************************** 13. row ***************************
   Title: get location cache begin
KeyValue: 
    Time: 592
*************************** 14. row ***************************
   Title: get location cache end
KeyValue: 
    Time: 91
*************************** 15. row ***************************
   Title: optimizer end
KeyValue: 
    Time: 1283
*************************** 16. row ***************************
   Title: cg begin
KeyValue: 
    Time: 0
*************************** 17. row ***************************
   Title: cg end
KeyValue: 
    Time: 373
*************************** 18. row ***************************
   Title: execution begin
KeyValue: arg1:false, end_trans_cb:false
    Time: 274
*************************** 19. row ***************************
   Title: do open plan begin
KeyValue: plan_id:8682172
    Time: 37
*************************** 20. row ***************************
   Title: sql start stmt begin
KeyValue: 
    Time: 2
*************************** 21. row ***************************
   Title: sql start stmt end
KeyValue: 
    Time: 617
*************************** 22. row ***************************
   Title: execute plan begin
KeyValue: 
    Time: 0
*************************** 23. row ***************************
   Title: execute plan end
KeyValue: 
    Time: 85
*************************** 24. row ***************************
   Title: sql start participant begin
KeyValue: 
    Time: 0
*************************** 25. row ***************************
   Title: sql start participant end
KeyValue: 
    Time: 4
*************************** 26. row ***************************
   Title: do open plan end
KeyValue: 
    Time: 1
*************************** 27. row ***************************
   Title: start_close_plan begin
KeyValue: 
    Time: 3016
*************************** 28. row ***************************
   Title: start_end_participant begin
KeyValue: 
    Time: 56
*************************** 29. row ***************************
   Title: start_end_participant end
KeyValue: 
    Time: 0
*************************** 30. row ***************************
   Title: start_close_plan end
KeyValue: 
    Time: 42
*************************** 31. row ***************************
   Title: start_auto_end_plan begin
KeyValue: 
    Time: 2
*************************** 32. row ***************************
   Title: start_auto_end_plan end
KeyValue: 
    Time: 47
*************************** 33. row ***************************
   Title: execution end
KeyValue: 
    Time: 9
*************************** 34. row ***************************
   Title: query end
KeyValue: 
    Time: 178
*************************** 35. row ***************************
   Title: NULL
KeyValue: PHY_SCALAR_AGGREGATE
    Time: 
*************************** 36. row ***************************
   Title: NULL
KeyValue:  PHY_PX_FIFO_COORD
    Time: 
*************************** 37. row ***************************
   Title: NULL
KeyValue:   PHY_PX_REDUCE_TRANSMIT
    Time: 
*************************** 38. row ***************************
   Title: NULL
KeyValue:    PHY_MERGE_GROUP_BY
    Time: 
*************************** 39. row ***************************
   Title: NULL
KeyValue:     PHY_GRANULE_ITERATOR
    Time: 
*************************** 40. row ***************************
   Title: t2
KeyValue:      PHY_TABLE_SCAN
    Time: 
40 rows in set (0.006 sec)

ERROR: No query specified

MySQL [oceanbase]> show trace \G;

select * from v$plan_cache_plan_stat limit 1 \G

show variables like '%plan_cache%' ;

 select
unit_id,tenant_id,tenant_name,svr_ip,svr_port,min_memory/(1024*1024*1024),max_memory/(1024*1024*
1024) from oceanbase.gv$unit ;

select tenant_id,svr_ip,svr_port,sql_num,mem_used/(1024*1024*1024),mem_limit/(1024*1024*1024),
access_count,hit_count,hit_rate,plan_num from oceanbase.gv$plan_cache_stat;
~~~



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



![img](https://gw.alipayobjects.com/zos/oceanbase/06cb789a-b615-43df-9f3f-05b23126b737/image/2022-01-04/5eb552d4-dcfb-4239-892b-7277c9edd400.png)

### 1. 问题

[问：Clog 盘满问题运维处理方法](https://open.oceanbase.com/ask/detail?id=13200020&search=clog%20slog%20ilog&pageNum=2)



Commit Log，所有Partition共用，日志可能是乱序的， 记录事务、PartitionService提供的原始日志内容。此目 录下的日志基于Paxos协议在多个副本之间同步



- 同样是一致性协议，Paxos 和 Raft 的区别是什么?

  



### 2. 如何解决的





- [x] 实验9。p84 https://gw.alipayobjects.com/os/bmw-prod/d5635d4b-9d2a-46c8-bab3-7527195f8635.pdf
- [x] 通过/proc查看Linux内核态调用栈来定位问题
- [x] docs/docs/docs-cn/6.administrator-guide/3.basic-database-management/2.zone-management/1.zone-management-overview.md



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

show parameters like 'clog_disk_utilization_threshold';
ALTER SYSTEM SET clog_disk_utilization_threshold=80。

ALTER SYSTEM SET clog_disk_utilization_threshold=20

ALTER SYSTEM SET  enable_syslog_recycle=true
ALTER SYSTEM SET  max_syslog_file_count=20;

 select * from __all_zone ;
 
 select * from __all_server \G;
 select * from __all_server_event_history \G;
 
 服务停止（停机运维）
 alter system set server_permanent_offline_time = ‘ 86400s‘
 alter system stop server‘ ip地址:2882’
 
select count(*) from __all_virtual_table t, __all_virtual_meta_table m
where t.table_id=m.table_id and role=1 and m.svr_ip=' ip地址' ; ）

select * from __all_virtual_table t, __all_virtual_meta_table m where t.table_id=m.table_id and role=1 ;

select * from __all_user \G; 
alter system start zone zone1;

desc gv$memstore;

磁盘使用情况：

select svr_ip, total_size/1024/1024/1024 total_G,
free_size/1024/1024/1024 free_G, (total_size - free_size)
/1024/1024/1024 used_G from __all_virtual_disk_stat; 

desc  __all_virtual_meta_table;

select tenant_id, svr_ip, unit_id, table_id, sum(data_size) /1024/1024/1024 size_G from __all_virtual_meta_table group by 1, 2, 3, 4;

select sql_id, query_sql,count(*), avg(elapsed_time), avg(execute_time), avg(queue_time), avg(user_io_wait_time)
from gv$sql_audit where tenant_id=1002 group by sql_id having count(*)>1 order by 5 desc limit 10\G;

select sql_id, avg(execute_time) avg_exec_time, count(*) cnt, avg(execute_time-TOTAL_WAIT_TIME_MICRO)
cpu_time from gv$sql_audit where tenant_id=1002 group by 1 order by avg_exec_time * cnt desc limit 5;


select * from __all_rootservice_event_history where module = 'server';

select * from __all_rootservice_event_history order by gmt_create desc limit 10;

show variables like 'recycleb%' ;

show recyclebin;

set global recyclebin = on;

select max(frozen_timestamp) from oceanbase.__all_virtual_freeze_info;
flashback table t1 to before drop 

show variables like 'resource_soft_limit' ;


resource_soft_limit 用于设置是否开启 Unit 均衡。
~~~

- [ ] OceanBase 社区版入门教程第九期 如何快速拿下 OBCA & OBCP 认证

https://open.oceanbase.com/blog/10900233?currentPage=2

![img](https://gw.alipayobjects.com/zos/oceanbase/06cb789a-b615-43df-9f3f-05b23126b737/image/2022-01-04/5eb552d4-dcfb-4239-892b-7277c9edd400.png)







摘要：

- Zone 本身是一个逻辑概念，是对物理机进行管理的容器，一般是同一机房的一组机器的组合

- 分区主副本的选择策略由租户的 `primary_zone` 属性决定，在创建租户的命令中可以指定 `primary_zone` 属性，也可以用 `ALTER` 语句来修改。

- 每一份叫做分区的一个副本。每个副本，包括存储在磁盘上的静态数据（SSTable）、存储在内存的增量数据（MEMTable）

- Clog 落盘
- **强一致**：发生脑裂、网络分区、宕机、磁盘故障等异常时，在 Clog 写入位置上保证数据强一致，不丢失数据。



- [ ] [0133.O OceanBase社区版clog使用空间调整](https://zhuanlan.zhihu.com/p/466337239)

​         [可靠分布式系统-paxos的直观解释](https://mp.weixin.qq.com/s/DlhpF0Vu5agtrR2Jx2cynQ)

​    摘要：

- 一份数据存多份. 多副本保证了可靠性, 而副本之间的一致, 就需要paxos这类分布式一致性算法来保证.
- **主从异步复制**是最简单的策略之一, 它很容易实现, 但存在一个问题: 客户端收到一个**数据已经安全**(OK)的信息, 跟**数据真正安全**(数据复制到全部的机器上)在时间上有一个空隙,
-  **主从同步复制**，缺点就是整个系统中有任何一个机器宕机, 写入就进行不下去了
- **半同步复制**





OceanBase 社区版入门教程第九期 如何快速拿下 OBCA & OBCP 认证

https://open.oceanbase.com/blog/10900233?currentPage=2







### 3. 收获

