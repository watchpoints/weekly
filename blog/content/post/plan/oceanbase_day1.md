---
title: " OceanBase 开源社区"
date: 2021-06-03
description: "入门教程"
draft: false
tags: ["GTD"]
categories: ["coding"]
---









# day1

## todo：我是新手 ，我从哪里开始

### 就看这个文档 https://open.oceanbase.com/community/organization

```
开源地址:
https://gitee.com/oceanbase/oceanbase
https://github.com/oceanbase/oceanbase
```



- 源码编译 需要x天

  就看这个文档：https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/get-the-oceanbase-database-by-using-source-code



根据官网说明，只能在centos上编译，因此在mac使用docker编译

~~~c++
使用源码构建 OceanBase 数据库
  https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/get-the-oceanbase-database-by-using-source-code
sudo docker pull centos
docker run -itd  --name oceanbase  --privileged centos /bin/bash
docker attach 56bd78306b21  //进入容器
docker update --memory 8G --memory-swap -1 56bd78306b21
  docker start 56bd78306b21 
  docker attach 56bd78306b21 
cd /root/data

git clone https://github.com/watchpoints/oceanbase.git //因为github环境太慢
改为：
git clone https://gitee.com/wang_cyi/oceanbase.git


# 进入代码根目录
cd oceanbase
# 执行build.sh脚本并传入init参数
sh build.sh init
sh build.sh debug

DEBUG 构建

# 在源码目录下执行debug版的预制构建命令
sh build.sh debug

# 进入生成的 debug 构建目录
cd build_debug

# 进行构建
make -j3 observer
make  observer

# 查看构建产物
stat src/observer/observe

其他安装文档：
https://gw.alipayobjects.com/os/bmw-prod/518c88ac-e4d2-4a2d-8428-11d3671f397f.pdf

使用 OBD 部署 OceanBase 数据库
https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/deploy-the-oceanbase-database-by-using-obd


# 安装 OBD
 https://github.com/oceanbase/obdeploy/blob/master/README-CN.md

# 什么是 OceanBase Client
https://github.com/oceanbase/obclient/blob/master/README.md
安装 OBClient
sudo yum install obclient

 docker update --memory 8G --memory-swap -1 57577713a6ef

  
docker attach 56bd78306b21
如果您采用本地安装，即中控机器和目标机器是同一台机器

https://github.com/oceanbase/obdeploy/blob/master/example/mini-local-example.yaml
https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/deploy-the-oceanbase-database-by-using-obd

# 修改 home_path， 这是 OceanBase 数据库的工作目录。
# 修改 mysql_port，这是 OceanBase 数据库 SQL 服务协议端口号。后续将使用此端口连接数据库。
# 修改 rpc_port，这是 OceanBase 数据库集群内部通信的端口号。

启动 OceanBase 数据库
运行以下命令部署集群：

obd cluster destroy obtest
obd cluster deploy obtest -c /root/data/mini-local-example.yaml 

 运行以下命令启动集群：

obd cluster start obtest -v

obd cluster edit-config obtest
obd cluster redeploy obtest
  
  运行以下命令查看集群状态：


  https://github.com/oceanbase/obdeploy

  

# 遇到问题
  error: RPC failed; curl 18 transfer closed with outstanding read data remaining
    error: RPC failed; curl 18 transfer closed with outstanding read data remaining
  
  https://github.com/oceanbase/oceanbase/issues/19


[root@56bd78306b21 etc]# obd cluster start obtest
Get local repositories and plugins ok
[WARN] (127.0.0.1) not enough memory
Start observer ok
observer program health check x
[WARN] failed to start 127.0.0.1 observer
[ERROR] oceanbase-ce start failed
[root@56bd78306b21 etc]# ls

~~~



- https://github.com/oceanbase/oceanbase/issues/92







### 挑战01--:直接使用镜像：**使用 Docker 镜像在 Mac 平台上部署 OceanBase 试用版**

~~~
https://www.yuque.com/oceanbase-site/oceanbase2.2.50-trial/gp7hkg 测试没有问题耗时一天
~~~



#### 挑战02--:直接镜像：自己搭建单机版的。











### 阅读：OceanBase 数据库概览 需要 x天

就看这个文档：https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/cluster-management











