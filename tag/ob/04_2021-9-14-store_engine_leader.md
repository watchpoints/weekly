https://www.oceanbase.com/training/detail?level=OBCA







# 开源数据库OceanBase代码导读

https://zhuanlan.zhihu.com/p/407940672

## （15）集群自举





### 青铜：了解基本概念



1. [两地三中心（2:2:2）](https://help.aliyun.com/apsara/enterprise/v_3_10_0_20200506/oceanbase/enterprise-technical-whitepaper/three-data-centers-in-the-same-city-1.html?spm=a2c4g.14484438.10001.41)是啥？



![这个是2:2:1 每个分区总共包含五个副本](https://static-aliyun-doc.oss-cn-hangzhou.aliyuncs.com/assets/img/zh-CN/4144591951/p27725.png)







- 假设深圳有两个机房，上海一个机房。对于每个分区，深圳机房分别部署两个副本，上海机房只部署一个副本，形成**2 + 2 + 1**部署。每个分区总共包含五个副本

-  2:2:2我需要创建一个集群，集群有三个zone，这三个zone分布在两个region中，每个zone内有两个observer

  小白疑问：6个机器，5个服务，不是奇数呀 ？

  

> 结果：zone 分区和 observer还是不清楚



2. rootservice是啥？



寻找

- [OceanBase 数据库中 RootService 服务常见问题](https://open.oceanbase.com/docs/knowledgeBase/faq-about-rootservice-in-oceanbase-databases)

- https://open.oceanbase.com/docs/knowledgeBase/observer-restart-failure-troubleshooting-steps

```sql
SELECT * FROM __all_virtual_core_meta_table;
```

- RootService的功能主要包括：服务器与 Zone管理、分区管理、每日合并控制、系统自举和DDL操作等。

  https://help.aliyun.com/apsara/enterprise/v_3_13_0_20201215/oceanbase/enterprise-technical-whitepaper/rootservice-1.html

  

  - https://www.oceanbase.com/training/Course?courseUniqueId=k8yjnbx1&lessonId=7

  ![image.png](https://i.loli.net/2021/09/15/oyIlhTcPO9kVsEU.png)

  

  

  

  三个zone ，假如zone1 是主节点，其他的zone2 和zone3 都是副节点吗？在zone1都是主 的分区不可能是副本?

  

  如果每个租户的primary_zone都是 zone1，那么就可以说  zone1 是主节点。那么这个集群里所有租户（实例）的数据的主副本都在 zone1 的机器里。

  

  

  

> 结果：还是不明白？





3. 系统自举（BootStrap）

 

BootStrap是系统的初始化安装过程，主要用于创建系统内部表，并初始化系统配置。



10.115.37.60
alias cdob="obclient -u root -h 127.0.0.1 -P2881"
use OceanBase
select * from __all_server \G; //服务器的状态
select * from __all_virtual_server_stat \G; //资源分配情况
select * from __all_unit_config \G; //资源定义情况

select * from __all_unit \G; //资源分配情况
select * from __all_resource_pool \G; //资源分配情况





## 白银：



~~~flow
  start=>start: ob_bootstrap.cpp
  s0=>operation: bootstrap的入口::ObService::bootstrap
  s1=>operation: 创建__all_core_table::ObPreBootstrap::prepare_bootstrap
  s2=>operation: 执行bootstrap::ObRootService::execute_bootstrap
  s3=>operation: 创建系统租户：ObBootstrap::execute_bootstrap()
  s4=>operation: rootservice上任：ObRootService::do_restart()
  end=>end: 结束
  start->s0->s1->s2->s3->s4->end
~~~



- ob_bootstrap.cpp







## sql



~~~sql
MySQL [OceanBase]> desc __all_core_table;
+--------------+----------------+------+-----+----------------------+-----------------------------+
| Field        | Type           | Null | Key | Default              | Extra                       |
+--------------+----------------+------+-----+----------------------+-----------------------------+
| gmt_create   | timestamp(6)   | YES  |     | CURRENT_TIMESTAMP(6) |                             |
| gmt_modified | timestamp(6)   | YES  |     | CURRENT_TIMESTAMP(6) | ON UPDATE CURRENT_TIMESTAMP |
| table_name   | varchar(128)   | NO   | PRI | NULL                 |                             |
| row_id       | bigint(20)     | NO   | PRI | NULL                 |                             |
| column_name  | varchar(128)   | NO   | PRI | NULL                 |                             |
| column_value | varchar(65536) | YES  |     | NULL                 |                             |
+--------------+----------------+------+-----+----------------------+-----------------------------+
~~~



