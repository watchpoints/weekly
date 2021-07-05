





# to_do 何调试TIDB源码



### 基础知识

综合比较两个Golang程序调试器gdb和dlv，我认为dlv的功能更为完善，更能满足实际调试时的功能需求。两者的优缺点比较大致如下

| 调试器 |                   优势                    |                         不足                         |
| :----: | :---------------------------------------: | :--------------------------------------------------: |
|  dlv   |      对goroutine环境调试支持比较完善      |                      暂时没找到                      |
|  gdb   | 符合现有的调试习惯，类似C/C++调试指令都有 | 对goroutine场景支持不足，不能很好的应对goroutine的调 |

~~~shell
### dlv 用法

# 关闭内联优化，方便调试
$ go build -gcflags "-N -l" main.go

# 发布版本删除调试符号
go build -ldflags “-s -w”
~~~





后面调试，连接tikv，pd 可以连接服务器上的了，这个研究一下，

## 一、Centos



### 准备

1. 使用tiup 部署集群

~~~shell
cd /data/tidb/tiup
tiup cluster deploy tidb-test v4.0.0 ./topology1.yaml --user root -p ##部署
tiup cluster list ##查询

tiup cluster display tidb-test  ##查询
tiup cluster start tidb-test ##启动
tiup cluster start tidb-test  -R pd ## 单独启动
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

~~~shell

### tidb
git clone https://github.com/pingcap/tidb.git
cd /data/tidb/src/github.com/pingcap/tidb
make

./tidb-server -V 
Release Version: v4.0.0-beta.2-949-g3f38fa5d0-dirty
Edition: Community
Git Commit Hash: 3f38fa5d02b618666efce714be129b1afacd582d
Git Branch: master

小王提问：
c++ 编译有 -g 调试模式，方便gdb调试,goalng deubg如何设置呢？

https://github.com/pingcap/tidb/blob/master/Makefile


~~~

3. 替换

~~~shell
###  Text file busy
cp /data/tidb/src/github.com/pingcap/tidb/bin/tidb-server  /data/tidb/tiup/tidb-deploy/tidb-4000/bin/tidb-server 

tiup cluster stop tidb-test
tiup cluster start tidb-test 
tiup cluster display tidb-test 
~~~

画外音：

> 这里做不到更新tidb-server版本时候平滑升级呀？，还需要停止原来的服务 .
>
> https://asktug.com/t/topic/36950
>
> https://asktug.com/t/topic/36090



4. gdb调试 [失败]

```
directory /data/tidb/src/github.com/pingcap/tidb

b ddl_api.go/ddl_api.go:1559
b CreateTable
```

http://lday.me/2017/02/27/0005_gdb-vs-dlv/



5.  dlv

- cd /data/tidb/tiup/tidb-deploy/tidb-4000/bin

- dlv attach 16419
- 设置断点

函数入口：

b tidb/server/conn.go:1378

b tidb/session/session.go:1080

王传义:
tiup启动时候发现里面不停执行 其他sql命令 ，window 启动时候也是，很难进入自己执行命令，看来单元测试最靠谱。

纸君:
所以要在 用户键入命令开始下断点、









### Centos



### 单元测试

https://asktug.com/t/topic/36997



如果不涉及 failpoint 的测试，可以

cd tidb/kv
go test -check.f TestBasicFunc

如果涉及 failpoint 的，可以

make failpoint-enable
cd tidb/kv
go test -check.f TestXXX
cd …





- 测试1-main函数

~~~
server:
ifeq ($(TARGET), "")
	CGO_ENABLED=1 $(GOBUILD) $(RACE_FLAG) -ldflags '$(LDFLAGS) $(CHECK_FLAG)' -o bin/tidb-server tidb-server/main.go
else
	CGO_ENABLED=1 $(GOBUILD) $(RACE_FLAG) -ldflags '$(LDFLAGS) $(CHECK_FLAG)' -o '$(TARGET)' tidb-server/main.go
endif
go test -check.f TestRunMain
atal error: runtime: out of memory
~~~







## 二、Windows



- 单元测试

~~~
cd D:\money\src\github.com\wangcy6\tidb\executor
go test -check.f TestCastXXX //使用 go test -check.f $TEST_NAME 来指定测试用例
~~~

画外音

> 这个比较消耗内存，我2G 云主机根本无法运行，直接报错 fatal error: runtime: out of memory

> 需要设置http_proxy,多执行几次。



- vsocde 调试【该方法可以，我电脑配置不行放弃了 】

1. [安装PowerShell7](https://github.com/PowerShell/PowerShell/releases/tag/v7.0.0-preview.1) ，

   


- go test 单元测试直接显示 

- vscode deubg 单元测试 本机 vscode golang launch.json 

https://github.com/golang/vscode-go/blob/master/docs/debugging.md



- [x] vsocde 启动 tidb

```golang
修改默认配置：D:\money\src\github.com\pingcap\tidb\config\config.go
var defaultConf = Config{
	Path:                         "F:/tidb",
	
tidb-server/main.go
["new store"] [path=unistore:///tmp/tidb]

```



- [x]  安装mysql客户端Navicat，连接window下 tb server 





# 三、 提交代码



- 生成新 SSH 密钥

```shell
ssh-keygen -t rsa -b 4096 -C "wang_cyi@163.com"
cat /root/.ssh/id_rsa.pub 
```

- 在github上添加刚刚生成的公钥

- ### 提交pr准备-本地代码管理


- 如何fork-别人的项目后更新代码

git remote -v
git remote add upstream https://github.com/pingcap/tidb.git

 git remote -v
origin  git@github.com:watchpoints/tidb.git (fetch) 【自己本地仓库 远程提交路径】
origin  git@github.com:watchpoints/tidb.git (push)
upstream        https://github.com/pingcap/tidb.git (fetch) 【自己本地仓库 fork的远程提交路径】
upstream        https://github.com/pingcap/tidb.git (push)


git fetch upstream  //拉取源仓库的代码到本地

git merge upstream/master     // 稳定版本分支
git merge upstream/dev_study  //开发版本分支 不管用
git merge origin/dev_study

fatal: You have not concluded your merge (MERGE_HEAD exists).
Please, commit your changes before you merge

git status
git reset --hard origin/master


##  切换分支 不加-b则是切换到某一分支上，加上-b就是创建且切换

git branch
git checkout -b dev_study
git checkout  dev_study 


### 提交pr准备-新分支提交到远程仓库中
git push origin dev_study

git push
fatal: The current branch dev_study has no upstream branch.
To push the current branch and set the remote as upstream, use

    git push --set-upstream origin dev_study


git push origin master


### 分支代码合并到主干

1 进入分支，更新分支代码
（branch）git pull；
2 切换主干
（branch）git checkout master；
3 在主干上合并分支branch
（master）git merge branch --squash
4 提交合并后的代码
（master）git commit -m ‘合并备注’
5 将代码推送到远程仓库
（master）git push

## 案例



### parser
git remote -v
git remote add upstream https://github.com/pingcap/parser.git

git fetch upstream  
git merge upstream/master     
git reset --hard origin/master
git branch
git checkout -b dev_study



```go
GO111MODULE=on go mod edit -replace github.com/pingcap/parser=/data/tidb/src/github.com/watchpoints/parser

version "dev_study" invalid: must be of the form v1.2.3
unversioned new path must be local directory

go list -m all |grep parser  ##显示依赖关系

git remote -v
//在默认情况下，在master分支
// git push时一般会上传到远程下的master分支上
git push origin master
git push --set-upstream origin master

//在分支的情况下：
git push --set-upstream origin dev_study

git push origin dev_study //分支提交到主干
```

## FQA

- 获取不到最新代码

  