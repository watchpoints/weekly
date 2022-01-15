不好意思，在打扰一次
昨天看https://pingcap.com/blog-cn/tidb-source-code-reading-5/ 和https://github.com/pingcap/parser/blob/master/docs/quickstart.md，运行上面例子
parser我没看明白具体过程。

问题：tidb从parer返回StmtNode ast.StmtNode到DDL ast.CreateTableStmt转变过程在哪里？
我单步跟踪好像直接映射出来了。

