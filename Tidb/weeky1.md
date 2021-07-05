
>说明：该文章随时在更新。
### 任务
~~~
本地下载 TiDB，TiKV，PD 源代码，改写源码并编译部署以下环境：

1 TiDB
1 PD
3 TiKV
改写后：使得 TiDB 启动事务时，能打印出一个 “hello transaction” 的 日志
~~~

### 输出：
- 搭建输出
![select * from information_schema.cluster_info](https://i.loli.net/2020/08/15/974tcSm58WuoeC2.png)
- 修改代码，输出日志
![image.png](https://i.loli.net/2020/08/16/jSLJvU4OhBgsHlT.png)
![image.png](https://i.loli.net/2020/08/16/uNnlLsX6Tq83KA5.png)

![image.png](https://i.loli.net/2020/08/16/udTbjSv1m98XHfV.png)

### 目录

>1. 一个普通2G内存云主机如何编译Tidb过程。机器配置太低，只启动一个Tikv

>2. 一个sql语句是如何执行的。

### 环境准备



| 软件   | 版本  | 备注                     |
| ------ | ----- | ------------------------ |
| Ubuntu | 18.04 | 1G内存不行改为2G，vpn    |
| go     | 1.15  | export GOPATH=/data/tidb |
| gcc    | 7.5.0 |                          |


![TiDB 整体架构](https://user-images.githubusercontent.com/5937331/90312675-dc92eb00-df38-11ea-89ea-df88785e0133.png)




###  编译步骤

#### go

~~~shell
#根目录
export GOROOT=/root/local/go
#bin目录
export GOBIN=$GOROOT/bin
#工作目录
export GOPATH=/data/tidb //设置工程目录
export PATH=$PATH:$GOBIN //添加可执行程序

go version
go version go1.15 linux/amd64
~~~





####  tidb（vpn下载第三方包）

~~~shell

mkdir -p /data/tidb/src/github.com/pingcap

cd /data/tidb/src/github.com/pingcap

git clone https://github.com/pingcap/tidb.git
cd tidb
make

~~~

#### pd

~~~shell
git clone https://github.com/pingcap/pd
make
~~~



#### TiKV （重点）

>TiKV 是一个分布式事务型的键值数据库，提供了满足 ACID 约束的分布式事务接口，并且通过 [Raft 协议](https://raft.github.io/raft.pdf) 保证了多副本数据一致性以及高可用,TiKV 参考 Spanner 设计了 multi raft-group 的副本机制 https://raft.github.io/

~~~shell

apt-get install -y cargo gcc-multilib
curl https://sh.rustup.rs -sSf | sh -s

git clone https://github.com/pingcap/tikv 


TiKV的Makefile中
release:
	cargo build --no-default-features --release --features "${ENABLE_FEATURES}"
改为

release:
	env RUSTFLAGS=-Ztime-passes cargo build --no-default-features --release --features "${ENABLE_FEATURES}"
可以打印详细时间

SYSTEM_ALLOC=1 make release //去掉jemalloc，https://github.com/tikv/tikv/pull/6211 

Finished release [optimized] target(s) in 85m 22s //编译85分钟

~~~





FQA

- [raft 协议中， 候选人角色能参与投票吗](https://xie.infoq.cn/article/0a8c579d8110b2025b536f79e)

#### mysql

~~~shell
主要使用客户端命令.
~~~



#### 启动

~~~shell
cd /data/tidb/src/github.com/pingcap
cp ./pd/bin/pd-server  /data/tidb/bin/
cp ./tikv/target/release/tikv-server /data/tidb/bin/

cp ./bin/tidb-server /data/tidb/bin/
~~~



- 步骤一，启动 PD

```
nohup pd-server --data-dir=pd \
                --log-file=./pd.log 2>&1 &
```

- 步骤二，启动 TiKV

```shell
nohup tikv-server --pd="127.0.0.1:2379" \
                  --data-dir=tikv \
                  --log-file=tikv.log 2>&1 &
                  
                  
```



./tidb-server  --data-dir=./temp  --log-file=./tikv.log 



- 步骤三，启动 TiDB

```
cp ./bin/tidb-server /data/tidb/bin/
cd /data/tidb/bin/
nohup ./tidb-server --store=tikv \
                  --path="127.0.0.1:2379" \
                  --log-file=tidb.log 2>&1 &
                  
cp /data/tidb/src/github.com/watchpoints/tidb/bin/tidb-server /data/tidb/tiup/tidb-deploy/tidb-4000/bin/tidb-server
cd /data/tidb/tiup
tiup cluster start tidb-test  -R pd ## 单独启动
tiup cluster stop tidb-test
```

- 步骤四，测试

```shell

mysql -h 127.0.0.1 -P 4000 -u root -D test

设置密码：
mysql -h 127.0.0.1 -P 4000 -u root

SET PASSWORD FOR 'root'@'%' = '123456'

mysql -h 127.0.0.1 -P 4000 -u root  -p 

```




## 一个sql是如何执行的【任务：启动事务时候打印日志】

目录：

- 服务器如何接受客户端连接
- 如何从IO中读取sql
- 一个sql是如何执行的
- 事务部分

--------------------------------------------------------------------



![image](https://user-images.githubusercontent.com/5937331/90312522-be78bb00-df37-11ea-96fa-5931a847fe14.png)

![SQL 层执行过程](https://download.pingcap.com/images/blog-cn/tidb-source-code-reading-3/2.png)



![image](https://user-images.githubusercontent.com/5937331/90312699-0e0bb680-df39-11ea-9888-6fc0313a45cf.png)

#### 代码部分(最新代码)

s.listener.Accept() -->go s.onConn(clientConn)-->dispatch()--->handleQuery()

一条 SQL 语句需要经过，语法解析–>合法性验证–>制定查询计划–>优化查询计划–>根据计划生成查询器–>执行并返回结果 等一系列流程。



这个主干对应于 TiDB 的下列包：

![image](https://user-images.githubusercontent.com/5937331/90312730-6347c800-df39-11ea-9d79-6f361e9ebd8f.png)

cd /data/tidb/src/github.com/pingcap/go-ycsb

./bin/go-ycsb load mysql -P ./workloads/workloada -p recordcount=10000 -p mysql.host=127.0.0.1 -p mysql.port=4000  -p mysql.user=root -p mysql.password=123456 -p mysql.db=test --threads 3

./bin/go-ycsb run mysql -P ./workloads/workloada -p recordcount=10000 -p mysql.host=127.0.0.1 -p mysql.port=4000  -p mysql.user=root -p mysql.password=123456 -p mysql.db=test --threads 3 

./bin/go-ycsb run mysql -P ./workloads/workloadb -p recordcount=10000 -p mysql.host=127.0.0.1 -p mysql.port=4000  -p mysql.user=root -p mysql.password=123456 -p mysql.db=test --threads 3









### ref

- TiDB 源码阅读系列文章
  TiDB 源码阅读系列文章（三）SQL 的一生
  TiDB 源码阅读系列文章（四）Insert 语句概览
- http://129.28.165.167:1313/post/book/tiDB
- https://github.com/wangcy6/leetcode/blob/master/study/TiDB/weeky1.md

--------------------------------下面是第二周的预习----------------------------

--------------------------------下面是第二周的预习----------------------------

--------------------------------下面是第二周的预习----------------------------

### update 2020年8月18日19:22:17


### tidb-dashboard

- 学习中
http://127.0.0.1:2379/dashboard
http://www.mytidb.com:2379/dashboard


~~~

 [dynamic_config_manager.go:168] ["Failed to load dynamic config from etcd"] [error="context deadline exceeded"]

~~~


### sysbench 安装

- 学习中
~~~

apt-get install libmysqlclient-dev
mkdir -p /tmp/sysbench
cd /tmp/sysbench
wget https://github.com/akopytov/sysbench/archive/1.0.14.tar.gz
yum -y install make automake libtool pkgconfig libaio-devel
yum -y install mariadb-devel
./autogen.sh
./configure
make -j
make install
sysbench --version 
~~~

- ERROR: cannot find MySQL libraries. If you want to compile with MySQL support,
apt-get install libmysqlclient-dev


### 使用 TiUP 部署 TiDB 集群

- 学习中

tiup cluster deploy <cluster-name> <tidb-version> ./topo.yaml --user root -p
参数 <cluster-name> 表示设置集群名称
参数 <tidb-version> 表示设置集群版本，可以通过 tiup list tidb 命令来查看当前支持部署的 TiDB 版本

tiup cluster deploy tidb-test v4.0.0 ./topology.yaml --user root [-p] [-i /home/root/.ssh/gcp_rsa]

~~~
# # Global variables are applied to all deployments and used as the default value of
# # the deployments if a specific deployment value is missing.
global:
 user: "tidb"
 ssh_port: 22
 deploy_dir: "/tidb-deploy"
 data_dir: "/tidb-data"

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
 - host: 10.0.1.1

tidb_servers:
 - host: 10.0.1.1

tikv_servers:
 - host: 10.0.1.1
   port: 20160
   status_port: 20180

 - host: 10.0.1.1
   port: 20161
   status_port: 20181

 - host: 10.0.1.1
   port: 20162
   status_port: 20182

tiflash_servers:
 - host: 10.0.1.1

monitoring_servers:
 - host: 10.0.1.1

grafana_servers:
 - host: 10.0.1.1
~~~

```shell
tiup cluster start <cluster-name>
tiup cluster start tidb-test

http://www.mytidb.com:2379/dashboard/
访问集群：

访问 TiDB 数据库，密码为空：

mysql -h 10.0.1.1 -P 4000 -u root
访问 TiDB 的 Grafana 监控：

通过 http://www.mytidb.com:3000 访问集群 Grafana 监控页面，默认用户名和密码均为 admin。
 http:/www.mytidb.com:3000
访问 TiDB 的 Dashboard：

通过 http://{pd-ip}:2379/dashboard 访问集群 TiDB Dashboard 监控页面，默认用户名为 root，密码为空。
http://www.mytidb.com:2379/dashboard/
执行以下命令确认当前已经部署的集群列表：

tiup cluster list
执行以下命令查看集群的拓扑结构和状态：

tiup cluster display <cluster-name>
```

### ref

 - TIDB 优化--TiKV 性能参数调优