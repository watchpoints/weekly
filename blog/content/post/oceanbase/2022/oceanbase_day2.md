---
title: "成为oceanbase贡献者第二天：源码解析 DDL语句执行过程"
date: 2022-09-03
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---



> 慢慢来，多看、多问、多总结，肯定是可以攻克的。

文章地址：

http://localhost:1313/post/oceanbase/2022/oceanbase_day2/

https://wangcy6.github.io/post/oceanbase/2022/oceanbase_day2/



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

4. 

# #一、Overview 

> 前言

大家好，

这篇文章描述的是在解决issues/1035 对sql一生的 ddl部分理解，问题出现在哪里.重在2.1章节 步骤4 5 6

不能提供最终是如何解决。愿对你有帮助，仅供参考

主要使用gdb 一步步跟踪，根据cmd 类型 if else switch 判断

- 题目如下： ALTER TABLE v0 DROP COLUMN v2 ;

```sql
https://github.com/oceanbase/oceanbase/issues/1035

CREATE TABLE v0 ( v2 INTEGER PRIMARY KEY , v1 VARCHARACTER ( 30 ) , UNIQUE v0 ( v1 ( 30 ) ) ) 

obclient [oceanbase]> ALTER TABLE v0 DROP COLUMN v2 ;
ERROR 4016 (HY000): Internal error

```



- 大纲

![](https://img-blog.csdnimg.cn/0a65a963b1624ae5a8b2cd8c6fe641a4.png)



![](https://img-blog.csdnimg.cn/189eba2a894d49d9a605a66498960e6a.png)

![](https://img-blog.csdnimg.cn/459a5f68c83f42069745ebb07e9dd4f1.png)



### 讨论问题

- https://ask.oceanbase.com/t/topic/35601148
- 

# # 二、MVP

> 最少知识



## 2.1 sql 一生



#### 整体流程 



![](https://img-blog.csdnimg.cn/fe015aa9d8d04d9996bbf2a070cffd0c.png)

#### 详细流程



![入口](https://img-blog.csdnimg.cn/f1dee96b6097419c8feff51e28c3137d.png)

![ddl部分](https://img-blog.csdnimg.cn/811f75e628944066963b50d662e9dc89.png)





~~~mermaid

graph TB
    1[ObMPQuery::process]-->2[ObMPQuery::do_process ]  -->5[ObSql::handle_text_query] -->4[handle_physical_plan] -->5[ ObMPQuery::response_result] -->6[ObMPQuery::register_callback_with_async] -->7[ObCmdExecutor::execute] -->8[ObDDLOperator::alter_table_column]
~~~



#### 步骤1  函数统一入口 ObMPQuery::process() 这个大家一看就懂.就是这样

~~~shell
文件：src\observer\mysql\obmp_query.cpp
函数：int ObMPQuery::process()

关键逻辑：
## 解析网络包
const ObMySQLRawPacket& pkt = reinterpret_cast<const ObMySQLRawPacket&>(req_->get_packet());
## 单个sql执行 ，如果多个sql 变成单个SQL执行
 ret = process_single_stmt(
                ObMultiStmtItem(false, 0, sql_), session, has_more, force_sync_resp, async_resp_used, need_disconnect);
			
## 封装一下 没有什么业务逻辑
文件:src\observer\mysql\obmp_query.cpp
函数： ObMPQuery::process_single_stmt(
功能：单个函数执行
关键逻辑：
 ObMPQuery::do_process

~~~



#### 步骤2： 调用sql引擎  输入sql语句字符串，输出一个包含物理执行计划和元信息的ResultSet。



ObMPQuery::do_process  

~~~c++
核心逻辑：
# 获取sql语句
const ObString& sql = ctx_.multi_stmt_item_.get_sql();

ObPhysicalPlan* plan = nullptr; //物理计划
ObQueryExecCtx* query_ctx = nullptr;

gctx_.sql_engine_->stmt_query(sql, ctx_, *result)) //调用sql引擎 
    
stmt_query方法：
    输入sql语句字符串，输出一个包含物理执行计划和元信息的ResultSet。
    外层打开并迭代结果集，把每一行结果发送给客户端

  
~~~



​       -   位置：src\obmp_query.cpp -->src\sql\ob_sql.cpp

~~~C++
int ObSql::stmt_query(const common::ObString &stmt, ObSqlCtx &context, ObResultSet &result)

handle_text_query(stmt, context, resul) //名字奇怪，不管他，就是要给函数调用


inline int ObSql::handle_text_query:
//SQL 
1. ObString trimed_stmt = const_cast<ObString &>(stmt).trim();
init_result_set
    
2. ObPlanCacheCtx *pc_ctx = NULL;
3. handle_large_query  //名字奇怪，不管他，就是要给函数调用
4. handle_physical_plan(trimed_stmt, context, result, *pc_ctx, get_plan_err))
5. after_get_plan
    
   
~~~

> 提示： 看到这里，还是很清晰的。不涉及解析 物理计划等等，你看到什么就是什么



#### 步骤3：查询sql执行



- ```c
  ObSql::handle_physical_plan
  
  handle_parser 
   ObParser parser()
   parser.parse(sql, parse_result) //没继续看 
      
      
  ObSql::generate_physical_plan(ParseResult &parse_result, ObPlanCacheCtx *pc_ctx, ObSqlCtx &sql_ctx, ObResultSet &result）
      
        ObLogPlan *logical_plan = NULL;
        ObPhysicalPlan *phy_plan = NULL;                        
        optimize_stmt
                              
          ObOptimizer optimizer(optctx);
          
            transform_stmt
                                
            code_generate
                                
   
  ```

  

  > 好奇： set ob_enable_trace_log = 1  show trace;跟踪查询sql执行过程，
  >
  > ddl sql呢？

![](https://img-blog.csdnimg.cn/e423a396e3be4e659a885d0ed81fc9a8.png)

![](https://img-blog.csdnimg.cn/dcfb6a4bd280418dad2b1c30e494b010.png)



![](https://img-blog.csdnimg.cn/f43a468108504016b441bb6cef633d0f.png)

#### 步骤4 ddl去哪里了

> 提问：ddl 去哪里了  ? ObCmdExecutor::execute
>
> ObCmdExecutor::execute(get_exec_context(), *cmd_))



- 原文：

对于非select和dml之外的语句，如大多数ddl，解析到这里就可以执行了。

这类简单语句类型统称为“命令”，

由engine/cmd下的executor直接执行。

ddl是通过rootservice（RS）执行的，所以其executor实际是发送RPC。事务控制语句则本机直接调用事务层。





![](https://img-blog.csdnimg.cn/be86cca8ef174493ab91e7f05ee84683.png)



- ObMPQuery::do_process---> ObMPQuery::response_result-->ObMPQuery::register_callback_with_async--->ObResultSet::execute()--->ObResultSet::open_cmd() --->ObCmdExecutor::execute(get_exec_context(), *cmd_))--->cmd.get_cmd_type()

> 回答：ddl 变成cmd命令 命令通过在  O
>
> bMPQuery::response_result-->ObMPQuery::register_callback_with_async

~~~shell
 switch (cmd.get_cmd_type()) {
    case stmt::T_CREATE_RESOURCE_POOL: {
      DEFINE_EXECUTE_CMD(ObCreateResourcePoolStmt, ObCreateResourcePoolExecutor);
      break;
    }
    case stmt::T_DROP_RESOURCE_POOL: {
      DEFINE_EXECUTE_CMD(ObDropResourcePoolStmt, ObDropResourcePoolExecutor);
      break;
    }
    case stmt::T_SPLIT_RESOURCE_POOL: {
      DEFINE_EXECUTE_CMD(ObSplitResourcePoolStmt, ObSplitResourcePoolExecutor);
      break;
    }
    case stmt::T_MERGE_RESOURCE_POOL: {
      DEFINE_EXECUTE_CMD(ObMergeResourcePoolStmt, ObMergeResourcePoolExecutor);
      break;
    }
    ## ALTER TABLE v0 DROP COLUMN v2 ;
    case stmt::T_ALTER_TABLE: {
      DEFINE_EXECUTE_CMD(ObAlterTableStmt, ObAlterTableExecutor);
      break;
    }
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

#### 步骤5 alter sql执行过程 

> 提问：你已经知道了 ddl 变成 cmd 命令  通过type ObAlterTableExecutor::execute 被调用的，
>
> 下面具体过程



- 主要逻辑： ObAlterTableStmt& stmt【sql引擎 cmd部分】

  

~~~
每种语句类型有一个Resolver和一个Stmt

函数：int ObAlterTableExecutor::execute(ObExecContext& ctx, ObAlterTableStmt& stmt)
路径：src\sql\engine\cmd\ob_table_executor.cpp
逻辑：

1. ObTaskExecutorCtx* task_exec_ctx = NULL; 
obrpc::ObCommonRpcProxy* common_rpc_proxy = NULL; //rpc 这地方
ObMySQLProxy* sql_proxy = ctx.get_sql_proxy();
 
2. ObAlterTableExecutor::alter_table_rpc_v2(alter_table_arg, res, allocator, common_rpc_proxy, my_session, is_sync_ddl_user))) {

3. set_drop_constraint_ddl_stmt_str
~~~



- 函数：ObAlterTableExecutor::alter_table_rpc_v2

- 核心逻辑：

​	OB_FAIL(common_rpc_proxy->alter_table(alter_table_arg, res))

- src\share\ob_common_rpc_proxy.h

​	RPC_S(PRD alter_table, obrpc::OB_ALTER_TABLE, (ObAlterTableArg), ObAlterTableRes);

​	 RPC_S(PRD alter_database, obrpc::OB_ALTER_DATABASE, (ObAlterDatabaseArg));

> 提问：rpc请求在哪里被调用的
>
> ObAlterTableExecutor::alter_table_rpc_v2 ---->common_rpc_proxy->alter_table(alter_table_arg, res))
>
> 

src\rootserver\ob_ddl_service.cpp

- ret 函数执行的返回值，这个从哪里设置的 看不到呀？

~~~c#

OB_FAIL(gctx_.sql_engine_->stmt_query(sql, ctx_, *result))

#define OB_FAIL(statement) (OB_UNLIKELY(::oceanbase::common::OB_SUCCESS != (ret = (statement))))

ret = gctx_.sql_engine_->stmt_query(sql, ctx_, *result
~~~





#### 步骤6 drop col逻辑



- gdb位置

  thread apply all break ob_ddl_operator.cpp:3041
  thread apply all break ob_ddl_operator.cpp:3006

  

- 函数 int ObDDLOperator::alter_table_column
- 逻辑：switch case 

```c++
// cnolumn that has been alter, change or modify

const ObColumnSchemaV2* orig_column_schema = NULL;

switch (alter_column_schema->alter_type_) {
    case OB_DDL_ADD_COLUMN
    case OB_DDL_CHANGE_COLUMN:
    case OB_DDL_DROP_COLUMN: //看这里
     alter_table_drop_aux_column
    case OB_DDL_MODIFY_COLUMN:

```

#### 步骤7：找到报错位置

- gdd位置

​		thread apply all break ob_ddl_operator.cpp:1459

- 函数逻辑：alter_table_drop_aux_column



ColumnSchem 

~~~~
  const ObColumnSchemaV2* delete_column_schema =
          aux_table_schema->get_column_schema(orig_column_schema.get_column_id());
      if (NULL != delete_column_schema) {
        if (delete_column_schema->is_index_column()) {
          ret = OB_ERR_ALTER_INDEX_COLUMN;
          RS_LOG(WARN, "can't not drop index column", K(ret));
        } 

  int64_t schema_version_;
  int64_t rowkey_position_;              // greater than zero if this is rowkey column, 0 if this is common column
  int64_t index_position_;               // greater than zero if this is index column       
~~~~



>  问 在schema中 primary key 如何表示 

## 三 、gdb调试技巧

### 3.1 gdb过程



#### 第六次

~~~c
dir /root/oceanbase
set pagination off 
delete 

watch ret
Old value = 0
New value = -4016
ALTER TABLE v0 DROP COLUMN v2 ;
ObDDLOperator::alter_table_drop_aux_column

thread apply all break ob_ddl_operator.cpp:3041
thread apply all break ob_ddl_operator.cpp:3006
thread apply all break ob_ddl_operator.cpp:2895
thread apply all break ob_ddl_operator.cpp:1460
thread apply all break ob_ddl_operator.cpp:1476
    
thread apply all break ob_ddl_operator.cpp:1428
thread apply all break ob_ddl_operator.cpp:1445
define PRIMARY_VP_COLUMN_FLAG (INT64_C(1) << 4)
    
    
// true: primary key/hidden primary key(pk_increment/cluster_id/parition_id)/partitioned key of no-pk tables
  inline bool is_rowkey_column() const
  {
    return rowkey_position_ > 0;
  }

~~~

 case OB_DDL_DROP_COLUMN:



- int ObDDLOperator::alter_table_drop_aux_column 什么意思

  

#### 第五次测试



~~~~shell

dir /root/oceanbase
set pagination off 
delete 

watch ret
Old value = 0
New value = -4016

src\rootserver\ob_ddl_operator.cpp
int ObDDLOperator::alter_table_column
ALTER TABLE v0 DROP COLUMN v2 ;
thread apply all break ob_ddl_operator.cpp:3036
thread apply all break ob_ddl_operator.cpp:3038


obclient [test]> ALTER TABLE v0 DROP COLUMN v2 ;
ERROR 4016 (HY000): Internal error



thread apply all break ob_ddl_operator.cpp:2269
thread apply all break ob_ddl_operator.cpp:3003
# here 
thread apply all break ob_ddl_operator.cpp:3036
thread apply all break ob_ddl_operator.cpp:3038

 if (OB_FAIL(alter_table_drop_aux_column(
                      *schema_service, new_table_schema, *orig_column_schema, trans, USER_INDEX))) {
                RS_LOG(WARN, "failed to drop index column");
                

RS_LOG(WARN, "failed to drop index column");

DML 语句. DML（Data Manipulation Language）语句：数据操纵语句。. 用途：用于添加、修改、删除和查询数据库记录，并检查数据完整性。. 常用关键字：insert、updat


DML 操作是指对数据库中表记录的操作，
主要包括表记录的插入（insert）、更新（update）、删除（delete）和查询（select），
是开发人员日常使用最频繁的操作。

~~~~



~~~shell
## 设置断点
## PROCESS
=
dir /oceanbase/oceanbase
dir /root/oceanbase
set pagination off //

//////////第五次测试
delete 
watch ret

src\rootserver\ob_ddl_operator.cpp
int ObDDLOperator::alter_table_column
thread apply all break ob_ddl_operator.cpp:2269
thread apply all break ob_ddl_operator.cpp:3003
thread apply all break ob_ddl_operator.cpp:30383


DML 语句. DML（Data Manipulation Language）语句：数据操纵语句。. 用途：用于添加、修改、删除和查询数据库记录，并检查数据完整性。. 常用关键字：insert、updat


DML 操作是指对数据库中表记录的操作，
主要包括表记录的插入（insert）、更新（update）、删除（delete）和查询（select），
是开发人员日常使用最频繁的操作。


-------------------------

 ObDDLService::alter_table
delete
thread apply all break ob_ddl_service.cpp:4456
thread apply all break ob_ddl_service.cpp:4457
thread apply all break ob_ddl_service.cpp:4458
thread apply all break ob_ddl_service.cpp:4463
thread apply all break ob_ddl_service.cpp:4464

thread apply all break ob_ddl_service.cpp:4465 
obclient [test]> ALTER TABLE v0 DROP COLUMN v2 ;
ERROR 4016 (HY000): Internal error


thread apply all break ob_ddl_service.cpp:4473 
thread apply all break ob_ddl_service.cpp:4480
thread apply all break ob_ddl_service.cpp:4507 if ret <0
thread apply all break ob_ddl_service.cpp:4535 if ret <0

thread apply all break ob_ddl_service.cpp:4726 if ret <0
ERROR 4550 (HY000): check drop column failed
---------------------第四次测试 -----
delete
thread apply all break ob_ddl_service.cpp:6284
thread apply all break ob_ddl_service.cpp:6286
thread apply all break ob_ddl_service.cpp:6291
thread apply all break ob_ddl_service.cpp:6292  
thread apply all break ob_ddl_service.cpp:6304   if ret <0  
thread apply all break ob_ddl_service.cpp:6304   if ret <0 
thread apply all break ob_ddl_service.cpp:6350    if ret <0
thread apply all break ob_ddl_service.cpp:6433 if ret <0

----
static const int32_t OB_PARSER_ERR_UNEXPECTED = -4016;
DEFINE_ERROR_EXT(OB_ERR_UNEXPECTED, -4016, -1, "HY000", "Internal error", "%s");
int ObMultiVersionSchemaService::get_schema
int ObTableSqlService::add_single_column

thread apply all break ob_ddl_service.cpp:6284   
thread apply all break ob_ddl_service.cpp:6304   if ret <0  //up
thread apply all break ob_ddl_service.cpp:6350    if ret <0  //up

constexpr int OB_NOT_ENOUGH_STORE = -4106;
 ERROR_CAUSE[-OB_NOT_ENOUGH_STORE] = "Internal Error";
 SQLSTATE[-OB_NOT_ENOUGH_STORE] = "HY000";
 DEFINE_ERROR(OB_NOT_ENOUGH_STORE, -4106, -1, "HY000", "not enough commitlog store");
 
thread apply all break ob_ddl_service.cpp:6394    if ret <0
thread apply all break ob_ddl_service.cpp:6433 if ret <0


thread apply all break ob_ddl_service.cpp:6081  if ret==-4016 
thread apply all break ob_ddl_service.cpp:6291   f ret==-4016 
thread apply all break ob_ddl_service.cpp:6301   f ret==-4016 
thread apply all break ob_ddl_service.cpp:6823    if ret==-4016 
thread apply all break ob_ddl_service.cpp:6349    if ret==-4016 
thread apply all break ob_ddl_service.cpp:6394    if ret==-4016 
thread apply all break ob_ddl_service.cpp:6433 if ret==-4016  



thread apply all break ob_ddl_service.cpp:5977
thread apply all break ob_ddl_service.cpp:6301
thread apply all break ob_ddl_service.cpp:6081
thread apply all break ob_ddl_service.cpp:6823   // do alter table in transaction
thread apply all break ob_ddl_service.cpp:6349   // submit asyn
thread apply all break ob_ddl_service.cpp:6394   // submit asyn
thread apply all break ob_ddl_service.cpp:6433 if ret <0

which 函数
thread apply all break  ob_log_part_mgr.cpp:213 ----设置断点段不上。
thread apply all break ob_root_service.cpp:4659
thread apply all break ob_ddl_service.cpp:5965
Cannot execute this command while the selected thread is running.
thread apply all break ob_ddl_service.cpp:6433 if ret <0
thread apply all break ob_ddl_service.cpp:6301
thread apply all break ob_ddl_service.cpp:5965
thread apply all break ob_ddl_service.cpp:6081

fail to get schema guard with version in inner table
(3)只运行当前线程：set scheduler-locking on

(4)运行全部的线程：set scheduler-locking off

show scheduler-locking
set scheduler-locking on
set scheduler-locking off
set scheduler-locking step
 
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
thread apply all break ob_table_executor.cpp:696  # 

thread apply all break ob_table_executor.cpp:703  # // 5 = -4016 $7 = -4018


thread apply all break ob_table_executor.cpp:744 if ret==-4016  不是这里
thread apply all break ob_table_executor.cpp:1021 if ret==-4016 
ERROR 4018 (HY000): Entry not exist

thread apply all break ob_table_executor.cpp:785

ObAlterTableExecutor::alter_table_rpc_v2
thread apply all break ob_table_executor.cpp:670 
thread apply all break ob_table_executor.cpp:698 
common_rpc_proxy->alter_table
alter_table_rpc_v2(alter_table_arg, res, allocator, common_rpc_proxy, my_session, is_sync_ddl_user)) 【问题不在这里】


int ObDDLService::alter_table
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

//RPC_S(PRD alter_table, obrpc::OB_ALTER_TABLE, (ObAlterTableArg), ObAlterTableRes);

which 函数
thread apply all break  ob_log_part_mgr.cpp:213
thread apply all break  ob_root_service.cpp:4659
thread apply all break  ob_ddl_service.cpp:5965

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

### 3.2 单元测试



### 1. 流程图：

![](https://img-blog.csdnimg.cn/558ffaa34d7e4e4590d395d2e51ca4b2.png)



~~~mermaid
graph TB
    1[begin] --> 2[sql::test_resolver_alter_table.test] -->3[resolver::test_resolver.cpp] -->4[sql执行过程:  do_load_sql--do_resolve --do_drop_table]-->5[result::test_resolver_alter_table.result]
~~~

### 2. 代码分析：

-  三个测试文件：const char* postfix[] = {"test", "tmp", "result"};

- 读取第一个文件内容：test_resolver_alter_table.test

  sql::test_resolver_alter_table.test：  while (std::getline(if_sql, line))

  

- 执行命令 输出到第二个文件 ：test_resolver_alter_table.temp

  

  ~~~c#
  TestSqlUtils::do_load_sql
  TestSqlUtils::do_resolve
  TestSqlUtils::do_drop_table
  ~~~

  

- 第二个文件 test_resolver_alter_table.temp 和第三个文件test_resolver_alter_table.result  对比结果 

  ASSERT_NO_FATAL_FAILURE(is_equal_content(file_name[1], file_name[2]));



### 3 .验证结果

- 执行命令

  

  cd /root/oceanbase/build_debug/src/observer （编译好新代码）

  cp observer /root/observer/bin/observer 

  

  /root/observer/bin/observer -r 127.0.0.1:2882:2881 -p 2881 -P 2882 -z zone1 -c 1 -d /root/observer/store

测试结果：自己写代码没有生效 是不是不正确呀

~~~
 if (delete_column_schema->is_index_column()) {
          ret = OB_ERR_ALTER_INDEX_COLUMN;
          RS_LOG(WARN, "can't not drop index column", K(ret));
        } else if (delete_column_schema->is_rowkey_column()) {
          ret = OB_ERR_ALTER_PRIMARY_KEY_COLUMN;
          RS_LOG(WARN, "can't not drop primary key column", K(ret));
        } else {
~~~



- 执行 run_tests.sh 通过

### 3.3 开发环境

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

源码编译的安装依赖
https://www.oceanbase.com/docs/community/observer-cn/V3.1.4/10000000000449658
yum install wget rpm* cpio make glibc-devel glibc-headers
yum install -y flex bison

# 在源码目录下执行debug版的预制构建命令
sh build.sh debug
# 进入生成的 debug 构建目录
cd build_debug
# 进行构建
make -j{N} observer
# 查看构建产物
stat src/observer/observer
bash build.sh debug --init 

debug mode
bash build.sh debug --init --make -j8
cd build_debug
/oceanbase/oceanbase/build_debug
以本地目录为基础创建一个镜像。此命令主要用于使用 OBD 启动自行编译的 OceanBase 开源软件，您可以通过此命令将编译产物加入本地仓库，之后就可以使用 obd cluster 相关的命令启动它
make DESTDIR=./ install 

1.1 在线安装 OBD

sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://mirrors.aliyun.com/oceanbase/OceanBase.repo
sudo yum install -y ob-deploy # 该操作只需要在 OB 中控机上执行，因为 OBD 是一个黑屏部署、管理和运维工具，统一从一台机器上发起操作，所以不需要每台机器均安装

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

离线安装部署，必须禁用远程仓库：
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
echo 'fs.aio-max-nr=1048576' >> /etc/sysctl.conf
sysctl -p

echo -e "root soft nofile 655350\nroot hard nofile 655350\n* soft nofile 655350\n* hard nofile 655350">> /etc/security/limits.conf


echo -e "root soft nofile 655350\nroot hard nofile 655350\n* soft nofile 655350\n* hard nofile 655350">> /etc/security/limits.conf


obd cluster deploy test -c /oceanbase/mini-local-example.yaml
obd cluster deploy test -c ./mini-local-example.yaml
obd cluster start test
# Connect to the OceanBase Database by using a MySQL client.
mysql -h127.1 -uroot -P2883
alias cdob="obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase"
+----------------------------------------------------------------------------+
|                                  Packages                                  |
+--------------+---------+----------------+----------------------------------+
| Repository   | Version | Release        | Md5                              |
+--------------+---------+----------------+----------------------------------+
| oceanbase-ce | 3.1.4-1 | 20220814195653 | f22bfe957eedf03bf5858c035cae8526 |



sudo yum install -y libobclient
sudo yum install -y obclient


cd /root/oceanbase/build_debug/src/observer
cp /observer /root/.obd/repository/oceanbase-ce/3.1.4-1/my-oceanbase/bin/


# 在源码目录下执行 release 版的预制构建命令
sh build.sh release

# 进入生成的 release 构建目录
cd build_release

# 进行构建
make -j{N} observer

# 查看构建产物
stat src/observer/observer


~~~

### 3.4 提交代码



git config user.name 
git config user.email



~~~
docker exec -it oceanbase-ce bash
service sshd restart
ssh root@10.2.131.60 -p 22 （宿主机器IP）


## 关联远程仓库，保证代码可以和远程更新和提交
git remote add oceanbase https://github.com/oceanbase/oceanbase.git
git remote -v
#把远程仓库最新的代码及分支拉取一下 
git fetch oceanbase 
git merge oceanbase/master

#查看远程分支
git branch -r 
git branch -a

## 本地创建分支，进行开发
git checkout -b issue1014


## 提交
git commit -m "fix Rocky is not supported yet #1014"
git push -u  origin issue_1035 

## 冲突解决
git reset --hard HEAD  （不建议）

cp /root/oceanbase/build_debug/src/observer/observer /root/observer/bin/observer 
~~~

问题描述：

![lQLPJxalWB4_ZoXMhM0CS7Bkz6aHA9uu5AMP910mwMoA_587_132.png](https://s2.loli.net/2022/09/01/l9ZrhaUEy4dgfqN.png)



原理分析：



执行步骤：

git config user.name watchpoints

git config user.email wang_cyi@163.com

# # 三、 Conclusion:



1. 方法 不停gdb调试

2. ob代码中 主键和rowkey关系？

​    来源：https://github.com/oceanbase/oceanbase/issues/1035

3. ddl是通过rootservice（RS）执行的，所以其executor实际是发送RPC

​         src\rootserver\ob_ddl_operator.cpp



# # 四、Further reading

> 举一反三



题目：https://github.com/oceanbase/oceanbase/issues/1034

```
CREATE TABLE v0 ( v2 VARCHARACTER ( 3 ) PRIMARY KEY , v1 VARCHARACTER ( 26 ) ) COLLATE utf8_general_ci ;
ALTER TABLE v0 ;

SET @@sql_mode = X'00FFFFFF' ;
ERROR 1231 (42000): Variable 'sql_mode' can't be set to the value of '\x00\xFF\xFF\xFF'

select @@sql_mode;
```

- 理解这个概念

~~~
https://www.cnblogs.com/kiko2014551511/p/11527480.html
https://developer.aliyun.com/article/710525
select @@sql_mode;

ANSI_QUOTES：
启用后，不能用双引号来引用字符串，因为"（双引号）将被解释为标识符
CREATE TABLE a ( a char(5));
INSERT INTO a SELECT 'abc';

cd /root/oceanbase/build_debug/src/observer
cp observer /root/observer/bin/observer 
~~~

- 分析过程

  https://www.yuque.com/yiyezhou/rgak0e/yl6dx8/edit





参考阅读



- https://github.com/oceanbase/obdeploy/blob/master/docs/zh-CN/3.user-guide/3.obd-command/2.command-group-for-mirroring-and-warehousing.md

- #### [构建/打包 OceanBase 数据库](https://www.oceanbase.com/docs/community/observer-cn/V3.1.4/10000000000449658)
