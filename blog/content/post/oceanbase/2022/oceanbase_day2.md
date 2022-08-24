---
title: "成为oceanbase贡献者第二天："
date: 2022-08-14
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---



> 慢慢来，多看、多问、多总结，肯定是可以攻克的。



文章地址：

http://localhost:1313/post/oceanbase/2022/oceanbase_day2/



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





### 开发环境



#### 虚拟机

~~~
[root@node1 oceanbase]# obd cluster list
+--------------------------------------------------+
|                   Cluster List                   |
+------+-------------------------+-----------------+
| Name | Configuration Path      | Status (Cached) |
+------+-------------------------+-----------------+
| test | /root/.obd/cluster/test | stopped         |
+------+-------------------------+-----------------+

VMware虚拟机解决空间不足，增加磁盘空间(磁盘扩容)
[root@node1 oceanbase]# lsblk
NAME        MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda           8:0    0   60G  0 disk
├─sda1        8:1    0    1G  0 part /boot
├─sda2        8:2    0   19G  0 part
│ ├─rl-root 253:0    0   17G  0 lvm  /
│ └─rl-swap 253:1    0    2G  0 lvm  [SWAP]
└─sda3        8:3    0 37.2G  0 part
sr0          11:0    1 10.4G  0 rom
mount /dev/sda3  /oceanbase

如何查看和修改 OceanBase 参数集群

debug mode
bash build.sh debug --init --make -j8
cd build_debug
/oceanbase/oceanbase/build_debug
以本地目录为基础创建一个镜像。此命令主要用于使用 OBD 启动自行编译的 OceanBase 开源软件，您可以通过此命令将编译产物加入本地仓库，之后就可以使用 obd cluster 相关的命令启动它
make DESTDIR=./ install 

obd mirror create -n oceanbase-ce -V 3.1.4-1 -p ./usr/local -t my-oceanbase


obd mirror list


+------------------------------------------------------------------+
|                      Mirror Repository List                      |
+----------------------------+--------+---------+------------------+
| SectionName                | Type   | Enabled | Update Time      |
+----------------------------+--------+---------+------------------+
| oceanbase.community.stable | remote | False   | 2022-08-01 19:49 |
| oceanbase.development-kit  | remote | False   | 2022-08-01 19:49 |
| local                      | local  | -       | 2022-08-02 10:59 |


禁用远程仓库
obd mirror disable remote
Disable remote ok

[ERROR] Repository(/root/.obd/repository/my-oceanbase/3.1.4-1/my-oceanbase) existed

[root@node1 build_debug]# obd mirror create -n oceanbase-ce -V 3.1.4-1 -p ./usr/local -t 
[root@node1 build_debug]# obd mirror create -n oceanbase-ce -V 3.1.4-1 -p ./usr/local -t my-oceanbase
Package ok
name: oceanbase-ce
version: 3.1.4-1
release:20220814195653
arch: x86_64


配置文件：【1】
https://gitee.com/oceanbase/obdeploy/blob/master/example/mini-distributed-example.yaml

obd cluster deploy test -c /oceanbase/mini-local-example.yaml
obd cluster start test
# Connect to the OceanBase Database by using a MySQL client.
mysql -h127.1 -uroot -P2883

+----------------------------------------------------------------------------+
|                                  Packages                                  |
+--------------+---------+----------------+----------------------------------+
| Repository   | Version | Release        | Md5                              |
+--------------+---------+----------------+----------------------------------+
| oceanbase-ce | 3.1.4-1 | 20220814195653 | f22bfe957eedf03bf5858c035cae8526 |

~~~





## 任务：

https://github.com/oceanbase/oceanbase/issues/1035



~~~
CREATE TABLE v0 ( v2 INTEGER PRIMARY KEY , v1 VARCHARACTER ( 30 ) , UNIQUE v0 ( v1 ( 30 ) ) ) ;


obclient [oceanbase]> ALTER TABLE v0 DROP COLUMN v2 ;
ERROR 4016 (HY000): Internal error
=INSERT INTO __all_server_event_history (g

~~~



### 设置断点



~~~shell
## 设置断点
## PROCESS
=
dir /oceanbase/oceanbase
set pagination off //
thread apply all break obmp_query.cpp:414
OB_INLINE int ObMPQuery::do_process
thread apply all break obmp_query.cpp:554
## handle_physical_plan （(这个断点不能设置)）
thread apply all break ob_sql.cpp:1200
//thread apply all break ob_sql.cpp:3160 （(这个断点不能设置)）
int ObSql::handle_physical_plan

thread apply all break ob_sql.cpp:3217(这个断点不能设置)
else if (OB_FAIL(generate_physical_plan(parse_result, &pc_ctx, context, result))) {

create table sex (sex bit(60) primary key,name bit(60)) partition by hash(sex) partitions 4;

    
inline int ObSql::handle_text_query 不合适太多内部函数
 thread apply all break ob_sql.cpp:1144
## handle_large_query
thread apply all break ob_sql.cpp:1197 if ret==-4016  


##stmt_query
thread apply all break ob_sql.cpp:628 if ret==-4016  

## handle_physical_plan

## 问题 其他sql 干扰
thread apply all break ob_sql.cpp:1205 if ret==-4016  
thread apply all break ob_sql.cpp:1206 if ret==-4016 
thread apply all break ob_sql.cpp:1217 if ret==-4016  
d 【6】

ObAlterTableExecutor::execute
thread apply all break ob_table_executor.cpp:1021

thread apply all break ob_table_executor.cpp:785

ObAlterTableExecutor::alter_table_rpc_v2
thread apply all break ob_table_executor.cpp:670 
thread apply all break ob_table_executor.cpp:698 
common_rpc_proxy->alter_table
alter_table_rpc_v2(alter_table_arg, res, allocator, common_rpc_proxy, my_session, is_sync_ddl_user)) 【问题不在这里】

D:\oceanbase\src\rootserver\ob_ddl_service.cpp
thread apply all break ob_ddl_service.cpp:5967
thread apply all break ob_ddl_service.cpp:6090
thread apply all break ob_ddl_service.cpp:6120
thread apply all break ob_ddl_service.cpp:6291
thread apply all break ob_ddl_service.cpp:6350
thread apply all break ob_ddl_service.cpp:6285  kaaaa

thread apply all break ob_ddl_service.cpp:6294


thread apply all break ob_ddl_service.cpp:6301 //up 
thread apply all break ob_ddl_service.cpp:6433 if ret <0

dir /oceanbase/oceanbase
set pagination off /
thread apply all break ob_cmd_executor.cpp:130 
~~~

D:\oceanbase\src\sql\executor\ob_cmd_executor.cpp



~~~
set ob_enable_trace_log = 1
show trace;
obclient [oceanbase]> select * from __all_server_event_history;



obclient [test]> ALTER TABLE v0 DROP COLUMN v2;
No connection. Trying to reconnect...
Connection id:    3221487617
Current database: test

ERROR 4694 (HY000): check drop column failed



(gdb) bt
#0  oceanbase::rootserver::ObDDLService::alter_table (this=0xc5f4550 <oceanbase::observer::ObServer::get_instance()::THE_ONE+9585616>, alter_table_arg=..., frozen_version=1) at ./src/rootserver/ob_ddl_service.cpp:6433
#1  0x000000000809aaf1 in oceanbase::rootserver::ObRootService::alter_table (this=0xbe37e80 <oceanbase::observer::ObServer::get_instance()::THE_ONE+1473792>, arg=..., res=...) at ./src/rootserver/ob_root_service.cpp:4684

#2  0x000000000a7d36d2 in oceanbase::rootserver::ObRpcAlterTableP::leader_process (this=0x7f1642a6d1d0) at ./src/rootserver/ob_rs_rpc_processor.h:364
#3  0x000000000a7b1edd in oceanbase::rootserver::ObRootServerRPCProcessorBase::process_ (this=0x7f1642a6f6f0, pcode=oceanbase::obrpc::OB_ALTER_TABLE) at ./src/rootserver/ob_rs_rpc_processor.h:181

#4  0x000000000a7d35a3 in oceanbase::rootserver::ObRootServerRPCProcessor<(oceanbase::obrpc::ObRpcPacketCode)517>::process (this=0x7f1642a6d1d0) at ./src/rootserver/ob_rs_rpc_processor.h:249
#5  0x000000000b6c492d in oceanbase::rpc::frame::ObReqProcessor::run (this=0x7f1642a6d1d0) at ./deps/oblib/src/rpc/frame/ob_req_processor.cpp:50
#6  0x000000000b6c58ae in oceanbase::rpc::frame::ObReqQHandler::handlePacketQueue (this=0xbdb8470 <oceanbase::observer::ObServer::get_instance()::THE_ONE+951024>, req=0x7f172a8e1f40) at ./deps/oblib/src/rpc/frame/ob_req_qhandler.cpp:78
#7  0x000000000b6c625e in oceanbase::rpc::frame::ObReqQueue::process_task (this=0x7f1833fd1ff0, task=0x7f172a8e1f40) at ./deps/oblib/src/rpc/frame/ob_req_queue_thread.cpp:124

~~~



## 最少知识：一个sql是如何执行的



### 代码摘要



~~~mermaid

graph TB
    1[开始] --> 2[结束] -->3[ObSql::handle_text_query] -->4[执行SQL->ObSQL::handle_text_query]
~~~



~~~
int ObMPQuery::process()


const ObMySQLRawPacket& pkt = reinterpret_cast<const ObMySQLRawPacket&>(req_->get_packet());

ObParser parser(
            THIS_WORKER.get_sql_arena_allocator(), session.get_sql_mode(), session.get_local_collation_connection());

ObParser::pre_parse(sql_, pre_parse_result))


 ret = process_single_stmt(
                ObMultiStmtItem(false, 0, sql_), session, has_more, force_sync_resp, async_resp_used, need_disconnect);
			


int ObMPQuery::process_single_stmt(
unittest\sql\resolver\sql\test_resolver_alter_table.test

~~~



### 小细节：

- K(retry_type_), K_(retry_times) 含义 

  ~~~c
  
  retry_type_=0, retry_times=1,
  
  语法：宏拼接
  #define K(x) #x, x
  =从哪里来的，这个不清楚 感觉有问题
  // When key is the name of value, can use K(value) for key and value.
  // example:
  //   SQL_LOG(INFO, "test for K", K(value));
  // When key is the name of a member in class, can use K_
  // example:
  //   value_ is a member of class T.Then in class T, can use
  //   SQL_LOG(INFO, "test for K_", K_(value))
  #define K(x) #x, x
  
  【遗留】从哪里来的，这个不清楚 感觉有问题
  
  ~~~

  

- ret 函数执行的返回值，这个从哪里设置的 看不到呀？

~~~c#

OB_FAIL(gctx_.sql_engine_->stmt_query(sql, ctx_, *result))

#define OB_FAIL(statement) (OB_UNLIKELY(::oceanbase::common::OB_SUCCESS != (ret = (statement))))

ret = gctx_.sql_engine_->stmt_query(sql, ctx_, *result
~~~



- Q3  unittest\CMakeLists.txt 文件中的环境变量${DEP_DIR}



~~~c++
unittest\CMakeLists.txt
    
target_link_libraries(${case} PRIVATE oceanbase gtest gmock ${DEP_DIR}/lib/mariadb/libmariadb.so.3)

#!/bin/bash
TOPDIR="$(dirname $(readlink -f "$0"))"
BUILD_SH=${TOPDIR}/build.sh
DEP_DIR=${TOPDIR}/deps/3rd/usr/local/oceanbase/deps/devel
    
    
TOPDIR = /oceanbase/oceanbase
BUILD_SH = /oceanbase/oceanbase/build.sh
DEP_DIR = /oceanbase/oceanbase/deps/3rd/usr/local/oceanbase/deps/devel
~~~

- gtest使用

~~~
https://github.com/google/googletest
ld.lld: error: undefined symbol: testing::internal::EqFailure(char 
~~~

 

```
case stmt::T_ALTER_TABLE: {
      DEFINE_EXECUTE_CMD(ObAlterTableStmt, ObAlterTableExecutor);
      break;
 }   
 
#define DEFINE_EXECUTE_CMD(Statement, Executor)       \
  Statement& stmt = *(static_cast<Statement*>(&cmd)); \
  Executor executor;                                  \
  sql_text = stmt.get_sql_stmt();                     \
  ret = executor.execute(ctx, stmt);
```





## Further reading

【1】 obd 官方手册 

 	https://github.com/oceanbase/obdeploy/blob/master/example/single-example.yaml

  https://www.cnblogs.com/imdba/p/15066267.html

​     https://github.com/oceanbase/obdeploy

​     https://ask.oceanbase.com/t/topic/20400019

   https://github.com/oceanbase/obdeploy/blob/master/docs/en-US/4.configuration-file-description.md

  https://github.com/oceanbase/oceanbase/issues/87

【2】 离线部署

- https://blog.csdn.net/carefree2005/article/details/125408455
- https://github.com/oceanbase/oceanbase/issues/19-

【3】 sql 语法

- https://www.sqlservertutorial.net/sql-server-basics/sql-server-alter-table-drop-column/

 [4] 如何绘制流程图

-  https://www.jianshu.com/p/7864c1cf5660

[5] （2）一条SQL的一生（续）

- https://zhuanlan.zhihu.com/p/381020426
- 开源数据库OceanBase代码导读: sql engine概览

https://zhuanlan.zhihu.com/p/385240752

https://www.geeksforgeeks.org/sql-ddl-dql-dml-dcl-tcl-commands/

【6】https://sourceware.org/gdb/current/onlinedocs/gdb/Set-Breaks.html#Set-Breaks
