---
title: "Tidb_reading_02"
date: 2020-09-20
description: ""
draft: false
categories: ["Tidb]
---







TiDB 源码阅读系列文章（三）SQL 的一生

https://pingcap.com/blog-cn/tidb-source-code-reading-3

https://pingcap.com/blog-cn/tidb-source-code-reading-5/

sig/execution && challenge-program

## 流程图

![SQL 层执行过程](https://download.pingcap.com/images/blog-cn/tidb-source-code-reading-3/2.png)





![执行器树](https://download.pingcap.com/images/blog-cn/tidb-source-code-reading-3/1.png)

![SQL 语句处理流程](https://download.pingcap.com/images/blog-cn/tidb-source-code-reading-5/1.png)

## 代码



### 函数func (s *session) Execute(



- 调用关系

func (s *session) Execute(ctx context.Context, sql string)

  --->func (s *session) Parse(ctx context.Context, sql string) 

 --->func (s *session) ExecuteStmt(ctx context.Context, stmtNode ast.StmtNode) 





- DDL 函数入口

  // Next implements the Executor Next interface.

  func (e *DDLExec) Next(ctx context.Context, req *chunk.Chunk) (err error) 

​             func (d *ddl) CreateTable(ctx sessionctx.Context, s *ast.CreateTableStmt) (err error) {

- 数据结构



~~~
// OptBinary is used for parser.
type OptBinary struct {
	IsBinary bool
	Charset  string
}

// FuncNode represents function call expression node.
type FuncNode interface {
	ExprNode
	functionExpression()
}

// StmtNode represents statement node.
// Name of implementations should have 'Stmt' suffix.
type StmtNode interface {
	Node
	statement()
}

// DDLNode represents DDL statement node.
type DDLNode interface {
	StmtNode
	ddlStatement()
}

// ExecStmt implements the sqlexec.Statement interface, it builds a planner.Plan to an sqlexec.Statement.
type ExecStmt struct {
~~~



# 参考

- http://blog.minifish.org/posts/tidb4/
- 