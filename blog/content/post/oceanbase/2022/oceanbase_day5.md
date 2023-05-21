---
title: "OceanBase Contributor day5："
date: 2023-01-30
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase探索之旅"]
---



oceanbase探索之旅开始，成为oceanbase贡献者第5天

# 历史文章

- https://www.yuque.com/yiyezhou/rgak0e/ykp9grn90kvx59c2



# # 一、Overview (任务是什么)

> 前言

大家好,今天是成为oceanbase贡献者第5天。



https://github.com/watchpoints/oceanbase/commit/eb331d4f0d8d7887201b835b07ea1f3528b934c2

https://github.com/oceanbase/oceanbase/issues/1383

[status: confirmed](https://github.com/oceanbase/oceanbase/labels/status%3A confirmed)

https://github.com/oceanbase/oceanbase/issues/1355

https://github.com/oceanbase/oceanbase/issues?q=is%3Aissue+is%3Aopen+label%3A%22type%3A+bug%22







今天主要任务是：

- https://github.com/oceanbase/oceanbase/issues/1246

- https://github.com/oceanbase/oceanbase/issues/1119

  https://github.com/oceanbase/oceanbase/issues/1365
  
  - https://github.com/oceanbase/oceanbase/issues/1365

请教下，在查询数据时候ob有办法绕过kvcache缓存吗？我想做一个不常用的查询但是数据量太大，避免影响其它查询的命中率。

--------------------------------------------------------------------------------------



###  为解决该问题 查缺补漏

- https://zhuanlan.zhihu.com/p/392107745

-------------------------------------------------------------------------------------------------------------------------------

###  任务拆分

todo1： 在虚拟机搭建一个mysql最新版本，验证mysql语----结果：提示语法错误

todo2： 源码编译验证最新代码---结果：已经被官方修复，你看如何修复的

todo3： ssues/1119源码分析

# #二 、 思考分析过程（why）

---------------------------------------------------------------------------------



todo1： 在虚拟机搭建一个mysql最新版本，验证mysql语法。

root/root   mydb/root 

1. 虚拟机设置固定ip

~~~scala
cd /etc/sysconfig/network-scripts
vi fcfg-ens160

TYPE=Ethernet
PROXY_METHOD=none
BROWSER_ONLY=no
BOOTPROTO=static
IPADDR=192.168.192.163
NETMASK=255.255.255.0
GATEWAY=192.168.192.2
DEFROUTE=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
NAME=ens160
UUID=1a6a9936-2ba7-46d3-9b40-f2681f0ee9ab
DEVICE=ens160
ONBOOT=yes

设置dns：
vi /etc/resolv.conf

//重启
dnf install network-scripts
ifdown ens160
~~~





Q1:小王遇到这个问题：NETMASK，GATEWAY，dns怎么 这个设置。

NETMASK=255.255.255.0
GATEWAY=10.141.163.254

A1：

 在虚拟网络编译器查看

https://www.jianshu.com/p/6fdbba039d79



2. mysql8安装

   ~~~
   https://blog.csdn.net/DDJ_TEST/article/details/115867125
   
   wget https://repo.mysql.com//mysql80-community-release-el8-4.noarch.rpm
   禁用默认的MySQL模块、
   sudo yum module disable mysql
   yum install mysql80-community-release-el8-4.noarch.rpm
   systemctl start mysqld（可以运行在1G单机内存上）
   
   查看’root’@'localhost创建的超级用户帐户及临时密码
   
   sudo grep 'temporary password' /var/log/mysqld.log
   2023-01-30T08:16:07.224630Z 6 [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: k2D+&9hpd+wc
   
   使用生成的临时密码登录
   mysql -uroot -p
   
   进入mysq修改密码
   ALTER USER 'root'@'localhost' IDENTIFIED BY 'Qwer@1234';
   
   select host, user, authentication_string, plugin from user;
   update user set host='%' where user='root';
   
   mysql -uroot -pQwer@1234
    show databases;
   ~~~

3.  造数据

~~~
CREATE TABLE v4 ( v5 INT, v6 INT , v7 INT) ;
INSERT INTO v4 VALUES (4, 5, 6);
SELECT DISTINCT JSON_ARRAYAGG ( DISTINCT v6 ) OVER ( PARTITION BY v7 ) FROM v4;

SELECT DISTINCT JSON_ARRAYAGG ( DISTINCT v6 ) OVER ( PARTITION BY v7 ) FROM v4;
ERROR 5798 (HY000): DISTINCT not allowed here

~~~



> 所有标志成status: confirmed的，在内部都有记录，这种issue一般我们会找内部同学修复 官方更新记录。
>
> 官方说：代码修复了



~~~
~~~

### todo2： 源码编译验证最新代码。



```c
git reset --hard origin/master 　　//把HEAD指向最新下载的版本
    
# 编译代码

//1. clean cluster
obd cluster destroy  test

//2 make

bash build.sh debug --init --make -j8
cd /root/src/oceanbase/build_debug

//3 make install
 
make DESTDIR=./ install 
cd /root/src/oceanbase/build_debug/usr/local/bin
./observer --version
observer (OceanBase_CE 4.1.0.0)

REVISION: 1-f61eb3f9672a877c9d6bf543c459d74e43fd839b
BUILD_BRANCH: master
BUILD_TIME: Nov 24 2022 12:38:57
BUILD_FLAGS: Debug
BUILD_INFO:

Copyright (c) 2011-2022 OceanBase Inc.


禁用远程镜像仓库
obd mirror disable remote


- 镜像版本
make DESTDIR=./ install && 

    
obd mirror create -n oceanbase-ce -V 4.1.0.0 -p ./usr/local -t my-oceanbase

Package ok
name: oceanbase-ce
version: 4.1.0.0
release:20221125211638
arch: x86_64
md5: 5379c1f155f3a7a49a3cf1a14093f3ab

//部署

mkfs.ext4 /dev/nvme3n1 
mkdir -p /data/
mount /dev/sda /data/
obd cluster destroy  test
obd cluster deploy test -c ./mini-local-example.yaml

https://gitee.com/oceanbase/obdeploy/blob/master/example/mini-local-example.yaml
oceanbase-ce:
  tag: my-oceanbase
  global:
    home_path: /root/src/observer
    memory_limit: 8G
    system_memory: 4G
obd cluster list
obd cluster start test

启动失败：
obd cluster edit-config test

//后续每次更新就替换
cd /root/src/oceanbase/build_debug/usr/local/bin
cp observer /root/.obd/repository/oceanbase-ce/3.1.4-1/my-oceanbase/bin/

 //后续每次更新就替换
cd /root/src/oceanbase/build_debug/usr/local/bin
cp observer /root/.obd/repository/oceanbase-ce/3.1.4-1/my-oceanbase/bin/
  
  //后续每次更新就替换
cd /root/src/oceanbase/build_debug/usr/local/bin
cp observer /root/.obd/repository/oceanbase-ce/3.1.4-1/my-oceanbase/bin/





obclient -h127.0.0.1 -P2881 -uroot -Doceanbase -A
obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase
        
obd cluster start test

Load cluster param plugin ok
Check before start observer ok
[WARN] (127.0.0.1) clog and data use the same disk (/data/storage)

 # 客户端操作
        

 修改日志级别
  obd cluster edit-config test
  tail -f /data/storage/observer/log/observer.lo
```

~~~



~~~



### todo3： ssues/1119源码分析

- https://github.com/oceanbase/oceanbase/commit/481c969b7dfb2744dc3c3b87211fed95771c37b6



### 第一次gdb调试

~~~~
 
src/sql/engine/aggregate/ob_aggregate_processor.cpp

dir /root/src/oceanbase
set pagination off 
SELECT DISTINCT JSON_ARRAYAGG ( DISTINCT v6 ) OVER ( PARTITION BY v7 ) FROM v4;
SELECT DISTINCT JSON_ARRAYAGG ( EmpSalary)  OVER(PARTITION BY EmpDepartment) sum_sala FROM Employee;

# int ObSql::handle_physical_plan
thread apply all break ob_sql.cpp:1947
thread apply all break ob_sql.cpp:3700
# ObSql::handle_text_query
thread apply all break ob_sql.cpp:1902
    
#  int ObSql::handle_text_query(const ObString &stmt, ObSqlCtx &context, ObResultSet &result)

thread apply all break ob_sql.cpp:1830
    
D\note_oceanbase\src\sql\ob_sql.cpp

    
# ObMPQuery::do_process
thread apply all break obmp_query.cpp:701
thread apply all break obmp_query.cpp:777
thread apply all break obmp_query.cpp:828  
逻辑：
stmt_query
response_result



# ObMPQuery::process_single_stmt
 thread apply all break obmp_query.cpp:501 

逻辑：
first_exec_sql true -do_process 
没有逻辑



dir /root/src/oceanbase
set pagination off 


CREATE TABLE v4 ( v5 INT, v6 INT , v7 INT) ;
INSERT INTO v4 VALUES (4, 5, 6);
SELECT DISTINCT JSON_ARRAYAGG ( DISTINCT v6 ) OVER ( PARTITION BY v7 ) FROM v4;
obclient [test]> SELECT DISTINCT JSON_ARRAYAGG ( DISTINCT v6 ) OVER ( PARTITION BY v7 ) FROM v4;
ERROR 5798 (HY000): DISTINCT not allowed here
~~~~

### sql 查询返回结果流程

1. 入库：gdb调试

~~~
dir /root/src/oceanbase
set pagination off 
# ObMPQuery::response_result
thread apply all break obmp_query.cpp:1258
流程：


ob_sync_plan_driver.cpp
int ObSyncPlanDriver::response_result
~~~

2. 流程代码



~~~c++
// 通过判断 plan 是否为 null 来确定是 plan 还是 cmd
// 针对 plan 和 cmd 分开处理，逻辑会较为清晰。
result.get_physical_plan()) 有物理计划的就普通的查询sql
    
ObSyncPlanDriver::response_result(ObMySQLResultSet &result)
    
~~~



2

流程分析：

~~~
 ObQueryDriver::response_query_result
----->
    | for 循环获取每一行。
    | while (result.get_next_row(result_row)) ) 
    
    如果是第一行，则先给客户端回复field等信息
    response_query_header //返回查询的列 --没仔细看回到住流程
    |
    |
    根据列 获 每row的 字段 ObObj& value = row->get_cell(i);
         文件类型：
        | 和miniob 逻辑一致  判断 值的类型 和列的类型是不一致。
        | ObObjCaster::to_type 每看
    
    | ObSMRow sm
    | 发送给客户端 sender_.response_packet
    

------------------------------ob_result_set.cpp ------------------------------           
ObResultSet::get_next_row ---->ObResultSet::inner_get_next_row

                                |  ObPhysicalPlan* physical_plan --->ObIExecuteResult *exec_result_;
                                |  ObICmd *cmd_;

------------------------------thread apply all break ob_execute_result.cpp:38------------------------------     
ObExecuteResult::get_next_row --->
            ObOperator *static_engine_root_;
            | 1 创建 row_.cells_
            | 2 创建 new (&row_.cells_[i]) ObObj(); 
              3 转换 int ObExpr::eval

------------------------------thread apply all break ob_operator.cpp:1293------------------------------     
ObOperator *op_;
-------> ObOperator::get_next_batch --->virtual int inner_get_next_batch(const int64_t max_row_cnt)
ObHashDistinctOp::inner_get_next_batch --->

    ob_hash_distinct_op.cpp
                                              
    ------------------------------thread apply all break ob_hash_distinct_op.cpp:675------------------------------     
(gdb) p get_next_batch_func_
$3 = (int (oceanbase::sql::ObHashDistinctOp::*)(oceanbase::sql::ObHashDistinctOp * const, const int64_t)) 0x84dffc0 <oceanbase::sql::ObHashDistinctOp::do_unblock_distinct_for_batch(long)>

OB_FAIL((this->*get_next_batch_func_)(batch_size))

    
------------------------------thread apply all break ob_window_function_op.cpp:2926------------------------------     
thread apply all break ob_window_function_op.cpp:2926
thread apply all break ob_window_function_op.cpp:3099
thread apply all break ob_window_function_op.cpp:2971
thread apply all break ob_window_function_op.cpp:1373
thread apply all break ob_window_function_op.cpp:1373


------------------------------thread apply all break ob_aggregate_processor.cpp:1422------------------------------     

~~~



## #  sql 查询返回结果流程

### 1. gdb

~~~
dir /root/src/oceanbase
set pagination off 
# ObMPQuery::response_result
thread apply all break obmp_query.cpp:1258
流程：


ob_sync_plan_driver.cpp
int ObSyncPlanDriver::response_result
~~~



### 2. ob_sync_plan_driver.cpp -->int ObSyncPlanDriver::response_result



~~~

和miniob返回解决过类似 

读取：result.open()

1. 标记指定阶段  in_sql_execution_

union {
    uint64_t exec_phase_; // phase of execution bitmap
    struct {
      uint64_t in_parse_          : 1;
      uint64_t in_pl_parse_       : 1;
      uint64_t in_get_plan_cache_ : 1;
      uint64_t in_sql_optimize_   : 1;
      uint64_t in_sql_execution_  : 1;
      uint64_t in_px_execution_   : 1;
      uint64_t in_sequence_load_  : 1;
    };
  };
  
2、 
result.is_with_rows 这个是查询有多个列意思，多少行。
    -------const common::ColumnsFieldIArray *p_field_columns_;

---2.1 ObQueryDriver::response_query_result
 class ObQueryDriver 这是个虚函数 
   D:\db\oceanbase\src\observer\mysql\ob_sync_cmd_driver.cpp ---没看
   
   src\observer\mysql\ob_query_driver.cpp （看这里）

3. ObQueryDriver::response_query_result


  循环读取每一行记录 get_next_row ---这个没什么逻辑   
      
     is_first_row  如果是第一行，则先给客户端回复field等信息。 
      
     循环读取每一行的列  -----2个for循环这里没有什么逻辑
       ObObj& value ，类型不一致 ObObjCaster::to_type ---->具体怎么cast 这里跳过没看，看主要逻辑
       
      没有压缩  charset  lob text转化 ----> 具体什么类型，没看
 
     | 
      2个最基本的for循环，list各种东西都没有，这里结束来了吗？奇怪
     | 
     
    3.3 返回结果 sender_.response_packet 
    
 4. 每一行是如何获取的,这里有什么逻辑！！！
 
    
    ObResultSet::get_next_row ---->ObResultSet::inner_get_next_row

                                |  ObPhysicalPlan* physical_plan --->ObIExecuteResult *exec_result_;
                                |  ObICmd *cmd_;
      

  这个完全是不符合标准定义，纯靠自己看得懂 physical_plan_ 和cmd区分，谁这样写代码。 
      https://docs.pingcap.com/tidb/v6.3/dev-guide-unstable-result-set
      参考文章：https://cn.pingcap.com/blog/tidb-source-code-reading-3 TiDB 源码阅读系列文章（三）SQL 的一生
      TiDB 源码阅读系列文章（二十三）Prepare/Execute 请求处理
      TiDB 源码阅读系列文章（十）Chunk 和执行框架简介 
      https://cn.pingcap.com/blog/tidb-source-code-reading-10

  src\sql\executor\ob_execute_result.cpp


----------------src\sql\executor\ob_execute_result.cpp---------------
        --------> int ObExecuteResult::get_next_row(ObExecContext &ctx, const common::ObNewRow *&row)  

int ObExecuteResult::get_next_row(ObExecContext &ctx, const common::ObNewRow *&row)


~~~



###  如何读取一行



~~~~
------------------------------thread apply all break ob_execute_result.cpp:38------------------------------ 

ObExecuteResult::get_next_row --->
            ObOperator *static_engine_root_;
            | 1 创建 row_.cells_
            | 2 创建 new (&row_.cells_[i]) ObObj(); 
              3 转换 int ObExpr::eval
~~~~



~~~
mkdir -p /data/storage
mkfs.ext4 /dev/nvme3n1 
mount /dev/nvme3n1 /data/storage

vi mini-local-example.yaml
obd cluster edit-config  test
obd cluster redeploy test

obd cluster deploy test -c ./mini-local-example.yaml
obd cluster start test
# Connect to the OceanBase Database by using a MySQL client.
mysql -h127.1 -uroot -P2883
alias cdob="obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase"


启动失败：
ERROR] (127.0.0.1): when production_mode is True, memory_limit can not be less then 16.0G

解决：

production_mode false


编译：
bash build.sh debug --init --make -j8
~~~



# # 三. 如何解决的（how） 



- https://github.com/oceanbase/oceanbase/issues/1365
- https://github.com/oceanbase/oceanbase/issues























 













### 

~~~


~~~









> 慢慢来，多看、多问、多总结，肯定是可以攻克的。

### 沟通步骤

1. 准备好一个ppt，在写代码之前演示最终目标 和架构设计 就是如何去实现的 【不要说公司部门环境不对 着就是最终结果，不要试着看看，一定是可以完全上线的项目，非demo和一个知识点。自己认为真的 不是闹着玩的。。】

   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比

   

2. 经过领导，专家 进行鸡蛋里挑骨头。【自己做好了别人路了胡扯，不会对别人产生任何影响，做事和做人一样，无论熟悉人，还是老师，领导，不相关人 反对 他们反馈信号，接受质疑，经过九九八十一难考验，并且你还在坚持认为对的。】

3. 最后融合别人建议，然后完善你项目。【不听老人言，吃亏在眼前，不敢接受别人批评，说明自己完全没有把握，才去否定 愤怒方式】
