https://docs.qq.com/sheet/DSlBwS3VCb01kTnZw?tab=BB08J2&confirmed=1

https://docs.qq.com/sheet/DSlBwS3VCb01kTnZw?tab=BB08J2

https://github.com/pingcap/blog

https://github.com/pingcap/tidb/issues/20804

https://github.com/pingcap/tidb/issues/20804#sig/DDL



# 基本操作



```javascript
https://github.com/watchpoints/parser
https://github.com/pingcap/parser
git reset --hard origin/master // git 放弃本地修改 强制更新
git remote add upstream //https://github.com/pingcap/parser.git
#切换到主分支
git checkout master
git fetch upstream
git merge upstream/master
git push origin master
git checkout dev_study

git@github.com:watchpoints/parser.git	
git remote -v 
git checkout -b wangchuanyi


```



## 使用tiup 部署集群

## vpn 环境 



### 准备

1. 使用tiup 部署集群

~~~shell
cd /data/tidb/tiup
tiup cluster deploy tidb-test  v5.0.2 ./topology1.yaml --user root -p ##部署

root@money:/data/tidb/tiup# cat topology1.yaml
# # Global variables are applied to all deployments and used as the default value of
# # the deployments if a specific deployment value is missing.
global:
 user: "tidb"
 ssh_port: 29761
 deploy_dir: "/data/tidb/tiup/tidb-deploy"
 data_dir: "/data/tidb/tiup/tidb-data"

# # Monitored variables are applied to all the machines.
monitored:
 node_exporter_port: 9100
 blackbox_exporter_port: 9115

server_configs:
 tidb:
   log.slow-threshold: 300
 tikv:
   readpool.storage.use-unified-pool: false
   readpool.coprocessor.use-unified-pool: true
 pd:
   replication.enable-placement-rules: true
 tiflash:
   logger.level: "info"

pd_servers:
 - host: 127.0.0.1

tidb_servers:
 - host: 127.0.0.1

tikv_servers:
 - host: 127.0.0.1
   port: 20170
   status_port: 20180




monitoring_servers:
 - host: 127.0.0.1

grafana_servers:
 - host: 127.0.0.1
 
 
tiup cluster list ##查询

tiup cluster display tidb-test  ##查询
tiup cluster start tidb-test ##启动
tiup cluster stop tidb-test


cd /data/tidb/tiup/tidb-deploy/tidb-4000/bin


## 对之前部署tidb-test集群进行参数 
tiup cluster edit-config  tidb-test
tiup cluster reload  tidb-test

# TiDB 集群可以在不中断线上服务的情况下进行扩容和缩容。
## 执行扩容命令  https://docs.pingcap.com/zh/tidb/dev/scale-tidb-using-tiup
tiup cluster scale-out tidb-test scale-out.yaml
ssh-keygen //设置 SSH 通过密钥登录

## 缩容 TiDB/PD/TiKV 节点 
画外音：配置太低，需要缩容

tiup cluster display  tidb-test
tiup cluster scale-in tidb-test --node 127.0.0.1:4001
提示：Scaled cluster `tidb-test` in successfully
查看 /data/tidb/tiup/tidb-deploy 目录 tidb-4001 已经被删除

https://asktug.com/t/topic/36950



~~~



2. 编译源码

#### tidb



~~~shell
## go 
https://golang.google.cn/dl/
cd /Users/wangchuanyi/local


### tidb
git clone https://github.com/pingcap/tidb.git
cd /Users/wangchuanyi/code/src/github.com/watchpoints/tidb
make

./tidb-server -V 
Release Version: v4.0.0-beta.2-949-g3f38fa5d0-dirty
Edition: Community
Git Commit Hash: 3f38fa5d02b618666efce714be129b1afacd582d
Git Branch: master



 git remote add  wangchuanyi https://github.com/pingcap/tidb.git
 git remote -v
 

//切换到本地分支 

git checkout -b wangchuanyi

//下载远程代码
git fetch wangchuanyi
//合并master到自己分支

git merge wangchuanyi/master
//合并upstream的master
// 提交远程master分支代码
 git push origin master
 
 http://senzhangai.github.io/programming/build-tidb-by-source
  git remote -v
  
 
 
 取代默认tidb：
 
 data/tidb/tiup/tidb-deploy/tidb-4000/bin
 
 

~~~



# ## vpn环境

~~~go
# vpn环境
## go环境
cd /root/local
https://golang.google.cn/dl/go1.16.5.linux-amd64.tar.gz
tar -xvf go1.16.5.linux-amd64.tar.gz 

## tidb
cd /data/tidb/src/github.com/watchpoints/tidb
git clone https://github.com/pingcap/tidb.git

git remote add  wangchuanyi https://github.com/pingcap/tidb.git


git fetch wangchuanyi
git merge wangchuanyi/master
git push origin master
 

make 






~~~



### 替换成自己编译

~~~shell
### tidb 
cp /data/tidb/src/github.com/watchpoints/tidb/bin/tidb-server  /data/tidb/tiup/tidb-deploy/tidb-4000/bin/tidb-server 

tiup cluster destroy tidb-test
tiup cluster stop tidb-test
tiup cluster start tidb-test 
tiup cluster display tidb-test 


https://asktug.com/t/topic/34904/22

第 5 步：查看 TiUP 管理的集群情况

tiup cluster deploy tidb-test v5.0.2 ./topology1.yaml --user root -p 


tiup cluster list
tiup cluster display tidb-test

~~~



#### 连接：



mysql -h 127.0.0.1 -P 4000 -u root -D test

Cdmysql

设置密码：
mysql -h 127.0.0.1 -P 4000 -u root

SET PASSWORD FOR 'root'@'%' = '123456'

mysql -h 127.0.0.1 -P 4000 -u root  -p 

~~~
~~~

## 如何单元测试

`go test -check.f $TEST_NAME` 来指定测试用例



## ##  Q1之前根本没接触过parser，该从哪里开始呀？





### 步骤1 ： 阅读 三十分钟成为Contributor | 提升TiDB Parser对MySQL 8.0语法兼容性



- https://segmentfault.com/a/1190000017467536
- https://zhuanlan.zhihu.com/p/77255479
- http://mysql.taobao.org/monthly/2017/02/04/
- [十分钟成为 Contributor 系列 | 支持 AST 还原为 SQL](https://segmentfault.com/a/1190000017467536)



### 步骤2: CreateTableStmt为例子tiDB是如何新建一张表的。



 	

#### 2.1 修改 `parser.y` 文件

~~~go
/*******************************************************************
 *
 *  Create Table Statement
 *
 *  Example:
 *      CREATE TABLE Persons
 *      (
 *          P_Id int NOT NULL,
 *          LastName varchar(255) NOT NULL,
 *          FirstName varchar(255),
 *          Address varchar(255),
 *          City varchar(255),
 *          PRIMARY KEY (P_Id)
 *      )
 // https://dev.mysql.com/doc/refman/5.7/en/create-table.html
 *******************************************************************/
CreateTableStmt:
	"CREATE" OptTemporary "TABLE" IfNotExists TableName TableElementListOpt CreateTableOptionListOpt PartitionOpt DuplicateOpt AsOpt CreateTableSelectOpt OnCommitOpt
	{
		stmt := $6.(*ast.CreateTableStmt)
		stmt.Table = $5.(*ast.TableName)
		stmt.IfNotExists = $4.(bool)
		stmt.TemporaryKeyword = $2.(ast.TemporaryKeyword)
		stmt.Options = $7.([]*ast.TableOption)
		if $8 != nil {
			stmt.Partition = $8.(*ast.PartitionOptions)
		}
		stmt.OnDuplicate = $9.(ast.OnDuplicateKeyHandlingType)
		stmt.Select = $11.(*ast.CreateTableStmt).Select
		if ($12 != nil && stmt.TemporaryKeyword != ast.TemporaryGlobal) || (stmt.TemporaryKeyword == ast.TemporaryGlobal && $12 == nil) {
			yylex.AppendError(yylex.Errorf("GLOBAL TEMPORARY and ON COMMIT DELETE|PRESERVE ROWS must appear together"))
		} else {
			if stmt.TemporaryKeyword == ast.TemporaryGlobal {
				stmt.OnCommitDelete = $12.(bool)
			}
		}
		$$ = stmt
	}
~~~



#### 2.2 ddl.go

~~~go
// CreateTableStmt is a statement to create a table.
// See https://dev.mysql.com/doc/refman/5.7/en/create-table.html
type CreateTableStmt struct {
	ddlNode

	IfNotExists bool
	TemporaryKeyword
	// Meanless when TemporaryKeyword is not TemporaryGlobal.
	// ON COMMIT DELETE ROWS => true
	// ON COMMIT PRESERVE ROW => false
	OnCommitDelete bool
	Table          *TableName
	ReferTable     *TableName
	Cols           []*ColumnDef
	Constraints    []*Constraint
	Options        []*TableOption
	Partition      *PartitionOptions
	OnDuplicate    OnDuplicateKeyHandlingType
	Select         ResultSetNode
}

// Restore implements Node interface.
func (n *CreateTableStmt) Restore(ctx *format.RestoreCtx) error {
	switch n.TemporaryKeyword {
	case TemporaryNone:
		ctx.WriteKeyWord("CREATE TABLE ")
	case TemporaryGlobal:
		ctx.WriteKeyWord("CREATE GLOBAL TEMPORARY TABLE ")
	case TemporaryLocal:
		ctx.WriteKeyWord("CREATE TEMPORARY TABLE ")
	}
	if n.IfNotExists {
		ctx.WriteKeyWord("IF NOT EXISTS ")
	}

	if err := n.Table.Restore(ctx); err != nil {
		return errors.Annotate(err, "An error occurred while splicing CreateTableStmt Table")
	}

	if n.ReferTable != nil {
		ctx.WriteKeyWord(" LIKE ")
		if err := n.ReferTable.Restore(ctx); err != nil {
			return errors.Annotate(err, "An error occurred while splicing CreateTableStmt ReferTable")
		}
	}
	lenCols := len(n.Cols)
	lenConstraints := len(n.Constraints)
	if lenCols+lenConstraints > 0 {
		ctx.WritePlain(" (")
		for i, col := range n.Cols {
			if i > 0 {
				ctx.WritePlain(",")
			}
			if err := col.Restore(ctx); err != nil {
				return errors.Annotatef(err, "An error occurred while splicing CreateTableStmt ColumnDef: [%v]", i)
			}
		}
		for i, constraint := range n.Constraints {
			if i > 0 || lenCols >= 1 {
				ctx.WritePlain(",")
			}
			if err := constraint.Restore(ctx); err != nil {
				return errors.Annotatef(err, "An error occurred while splicing CreateTableStmt Constraints: [%v]", i)
			}
		}
		ctx.WritePlain(")")
	}

	for i, option := range n.Options {
		ctx.WritePlain(" ")
		if err := option.Restore(ctx); err != nil {
			return errors.Annotatef(err, "An error occurred while splicing CreateTableStmt TableOption: [%v]", i)
		}
	}

	if n.Partition != nil {
		ctx.WritePlain(" ")
		if err := n.Partition.Restore(ctx); err != nil {
			return errors.Annotate(err, "An error occurred while splicing CreateTableStmt Partition")
		}
	}

	if n.Select != nil {
		switch n.OnDuplicate {
		case OnDuplicateKeyHandlingError:
			ctx.WriteKeyWord(" AS ")
		case OnDuplicateKeyHandlingIgnore:
			ctx.WriteKeyWord(" IGNORE AS ")
		case OnDuplicateKeyHandlingReplace:
			ctx.WriteKeyWord(" REPLACE AS ")
		}

		if err := n.Select.Restore(ctx); err != nil {
			return errors.Annotate(err, "An error occurred while splicing CreateTableStmt Select")
		}
	}

	if n.TemporaryKeyword == TemporaryGlobal {
		if n.OnCommitDelete {
			ctx.WriteKeyWord(" ON COMMIT DELETE ROWS")
		} else {
			ctx.WriteKeyWord(" ON COMMIT PRESERVE ROWS")
		}
	}

	return nil
}

// Accept implements Node Accept interface.
func (n *CreateTableStmt) Accept(v Visitor) (Node, bool) {
	newNode, skipChildren := v.Enter(n)
	if skipChildren {
		return v.Leave(newNode)
	}
	n = newNode.(*CreateTableStmt)
	node, ok := n.Table.Accept(v)
	if !ok {
		return n, false
	}
	n.Table = node.(*TableName)
	if n.ReferTable != nil {
		node, ok = n.ReferTable.Accept(v)
		if !ok {
			return n, false
		}
		n.ReferTable = node.(*TableName)
	}
	for i, val := range n.Cols {
		node, ok = val.Accept(v)
		if !ok {
			return n, false
		}
		n.Cols[i] = node.(*ColumnDef)
	}
	for i, val := range n.Constraints {
		node, ok = val.Accept(v)
		if !ok {
			return n, false
		}
		n.Constraints[i] = node.(*Constraint)
	}
	if n.Select != nil {
		node, ok := n.Select.Accept(v)
		if !ok {
			return n, false
		}
		n.Select = node.(ResultSetNode)
	}
	if n.Partition != nil {
		node, ok := n.Partition.Accept(v)
		if !ok {
			return n, false
		}
		n.Partition = node.(*PartitionOptions)
	}

	return v.Leave(n)
}
~~~





#### 2.3 parser_test.go



- TestDMLStmt



#### 步骤2.4 查看历史 issue



- https://github.com/pingcap/tidb/issues/8532

- https://github.com/pingcap/tidb/pull/21064

  

- https://github.com/pingcap/tidb/issues/21063

- https://github.com/pingcap/tidb/pull/21064/commits [ok]



  lexer: Format syntax errmsg when value out of range #1110

- https://github.com/pingcap/parser/pull/1110



## Q2：如何了解 planner模块



https://github.com/watchpoints/tidb



## 步骤2 查看代码

-  lanner/core/preprocess_test.go



#### planner/core/preprocess.go

~~~go

// preprocessor is an ast.Visitor that preprocess
// ast Nodes parsed from parser.
type preprocessor struct {
	ctx    sessionctx.Context
	flag   preprocessorFlag
	stmtTp byte

	// tableAliasInJoin is a stack that keeps the table alias names for joins.
	// len(tableAliasInJoin) may bigger than 1 because the left/right child of join may be subquery that contains `JOIN`
	tableAliasInJoin []map[string]interface{}
	withName         map[string]interface{}

	// values that may be returned
	*PreprocessorReturn
	err error
}

func (p *preprocessor) Enter(in ast.Node) 

 func (p *preprocessor) checkCreateTableGrammar(

   
   
   planner/core/preprocess_test.go
   

~~~



## Q3：如何了解DDL 模块



#### 步骤1 阅读文档。了解整个处理过程

- TiDB 源码阅读系列文章（十七）DDL 源码解析

​    https://pingcap.com/blog-cn/tidb-source-code-reading-17/





#### 步骤2 根据文档看代码

https://github.com/pingcap/tidb/blob/source-code/ddl/ddl_api.go#L739



~~~sql

func (e *DDLExec) Next:
	case *ast.CreateTableStmt:-->func (e *DDLExec) executeCreateTable -->func (d *ddl) CreateTable(
    
  
    
func (d *ddl) CreateTableWithInfo(
  buildTableInfo 获取 global table ID，生成 tableInfo , 即 table 的元信息，然后封装成一个 DDL job，
  这个 job 包含了 table ID 和 tableInfo，
  并将这个 job 的 type 标记为
  ActionCreateTable。

 checkTableInfoValidExtra：
d.doDDLJob(ctx, job) 函数中的 d.addDDLJob(ctx, job) 会先给 job 获取一个 global job ID 然后放到 job queue 中去。
  
~~~







# 我的issue

##  issue 18149 [合并]

- DDL: create table should not allow explicit empty name index #20617

- 加速查询：https://phoenix2.yizimg.com/pingcap/tidb/pull/18149
- https://github.com/pingcap/tidb/pull/20617

~~~go
if constraint.IsEmptyIndex {
				p.err = ddl.ErrWrongNameForIndex.GenWithStackByArgs(constraint.Name)
				return
			}
// issue 18149
func (s *testValidatorSuite) TestValidator(c *C) 

func (s *testValidatorSuite) runSQL(c *C, sql string, inPrepare bool, terr error) {
	stmts, err1 := session.Parse(s.ctx, sql)
	c.Assert(err1, IsNil, Commentf("sql: %s", sql))
	c.Assert(stmts, HasLen, 1)
	stmt := stmts[0]
	var opts []core.PreprocessOpt
	if inPrepare {
		opts = append(opts, core.InPrepare)
	}
	err := core.Preprocess(s.ctx, stmt, append(opts, core.WithPreprocessorReturn(&core.PreprocessorReturn{InfoSchema: s.is}))...)
	c.Assert(terror.ErrorEqual(err, terr), IsTrue, Commentf("sql: %s, err:%v", sql, err))
}
~~~





## [ssues/9815](https://github.com/pingcap/tidb/issues/9815)

#### 进展





### 输出



- https://github.com/pingcap/tidb/issues/9815

- https://github.com/pingcap/tidb/pull/9328/files

- https://github.com/pingcap/tidb/issues/9798

- https://blog.csdn.net/nangeali/article/details/76359601 MySQL修改视图

  

~~~~
What did you expect to see?
mysql [localhost:8015] {msandbox} (test) > use test;
Database changed
mysql [localhost:8015] {msandbox} (test) > DROP VIEW IF EXISTS v1;
Query OK, 0 rows affected (0.01 sec)

mysql [localhost:8015] {msandbox} (test) > CREATE VIEW v1 AS SELECT 1;
Query OK, 0 rows affected (0.01 sec)

mysql [localhost:8015] {msandbox} (test) > SELECT * FROM v1;
+---+
| 1 |
+---+
| 1 |
+---+
1 row in set (0.00 sec)

mysql [localhost:8015] {msandbox} (test) > ALTER VIEW v1 AS SELECT 2;
Query OK, 0 rows affected (0.01 sec)

mysql [localhost:8015] {msandbox} (test) > SELECT * FROM v1;
+---+
| 2 |
+---+
| 2 |
+---+
1 row in set (0.00 sec)

// DropTableStmt is a statement to drop one or more tables.
// See https://dev.mysql.com/doc/refman/5.7/en/drop-table.html
type DropTableStmt struct {
	ddlNode

	IfExists         bool
	Tables           []*TableName
	IsView           bool
	TemporaryKeyword // make sense ONLY if/when IsView == false
}

DropViewStmt:
	"DROP" "VIEW" TableNameList RestrictOrCascadeOpt
	{
		$$ = &ast.DropTableStmt{Tables: $3.([]*ast.TableName), IsView: true}
	}
|	"DROP" "VIEW" "IF" "EXISTS" TableNameList RestrictOrCascadeOpt
	{
		$$ = &ast.DropTableStmt{IfExists: true, Tables: $5.([]*ast.TableName), IsView: true}
	}
~~~~







