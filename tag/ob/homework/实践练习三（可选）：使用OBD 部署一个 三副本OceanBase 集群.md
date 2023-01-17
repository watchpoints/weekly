## 题目

- https://open.oceanbase.com/answer/detail?id=13700844

- 2.7 如何使用 OBD 自动化部署多节点集群
https://open.oceanbase.com/docs/tutorials/quickstart/V1.0.0/2-7-how-to-use-obd-to-deploy-a-multi-node-cluster

- 2-6-如何使用OBD自动部署OceanBase三副本集群

https://www.bilibili.com/video/BV1ML4y1W7fL/?spm_id_from=333.788.recommend_more_video.0


~~~
（必选）部署 OBD 软件，离线或者在线都可以。
（必选）使用 OBD 部署一套三副本集群（单机3节点或者3节点都可以），部署至少1个 OBProxy。
（必选）部署至少一个 OBAgent 。
（可选）部署 Promethues ，能查看采集的 OB 性能或状态数据。
（可选）部署 Grafana，能使用 OB 的性能模板查看 OB性能数据。
~~~


## 环境 资源不够 

> 从配置文件开始，后面补充


## ob部署单节点完成（省略）

## OceanBase三副本集群配

- 部署三节点 observer 进程：
https://github.com/oceanbase/obdeploy/blob/master/example/mini-distributed-example.yaml

部署三节点 observer 和 obproxy 进程：

https://github.com/oceanbase/obdeploy/blob/master/example/mini-distributed-with-obproxy-example.yaml



### 思考

1、 data_dir 和redo_dir区别？上个实验

回答：

- The directory for data storage.【sstable】
- The directory for clog, ilog, and slog. The default value is the same as the data_dir value.


~~~
# 创建相关目录
mkdir -p /data
mkdir -p /redo
mkdir -p /app/oceanbase

# 修改目录所有者权限

chown -R admin:admin /data

chown -R admin:admin /redo

chown -R admin:admin /app/oceanbase
~~~

2. datafile_size: 50G 如何申请的。？

回答：启动则占用。






### 相关命令

obd cluster deploy test -c mini-distributed-with-obproxy-example.yaml

obd cluster start test

obd cluster display test


obclient -h 172.16.100.51 -P 2881 -uroot@sys -p -c -A
