https://www.oceanbase.com/training/detail?level=OBCA



# to_do



## 参考别人阅读记录

- 淘宝数据库OceanBase SQL编译器部分 源码阅读--解析SQL语法树
  https://blog.csdn.net/qq910894904/article/details/28658421 
- 淘宝数据库OceanBase SQL编译器部分 源码阅读--生成逻辑计划
  https://www.cnblogs.com/chenxueyou/p/3776203.html

- 淘宝数据库OceanBase SQL编译器部分 源码阅读--生成物理查询计划
  https://blog.csdn.net/qq910894904/article/details/30215665?spm=1001.2014.3001.5501
- 淘宝数据库OceanBase SQL编译器部分 源码阅读--Schema模式

https://blog.csdn.net/qq910894904/article/details/32322909?spm=1001.2014.3001.5501

 ## 参考：

 TiDB 源码阅读系列文章（六）Select 语句概览
 https://pingcap.com/blog-cn/tidb-source-code-reading-6/

 

 TiDB 源码阅读系列文章（五）TiDB SQL Parser 的实现

 TiDB 源码阅读系列文章（二十三）Prepare/Execute 请求处理
 https://pingcap.com/blog-cn/tidb-source-code-reading-23/

 TiDB 源码阅读系列文章（四）Insert 语句概览
 https://pingcap.com/blog-cn/tidb-source-code-reading-4/



# 开源数据库OceanBase代码导读



https://www.zhihu.com/people/yang-zhi-feng-79/posts



### sql 执行过程



split_multiple_stmt

首先通过ObParser的一个快速解析入口split_multiple_stmt把每条语句拆分出来，对每条语句process_single_stmt



~~~c++
int ObMPQuery::process_single_stmt(const ObMultiStmtItem& multi_stmt_item, ObSQLSessionInfo& session,

  bool has_more_result, bool force_sync_resp, bool& async_resp_used, bool& need_disconnect)
~~~









开源数据库OceanBase代码导读（1）

https://zhuanlan.zhihu.com/p/379437192

