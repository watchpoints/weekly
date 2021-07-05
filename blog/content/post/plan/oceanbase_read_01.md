---
title: " OceanBase 开源社区"
date: 2021-06-03
description: "入门教程"
draft: false
tags: ["GTD"]
categories: ["coding"]
---

# day1



## 总结：Mac:使用 OBD 部署 OceanBase 数据库

参考文档：https://github.com/oceanbase/obdeploy

预备知识：

- OceanBase 不支持mac，使用docker

- obd是安装工具，不需要重现编译源码（定制版本除外）

- 单机 本地安装不需要配上ssh 密码登陆 配置文件 

- 对内存和cpu都要要求，满足最低配置  mini-local-example.yaml 执行不会报错。 需要结合日志和代码解决。

  

环境准备：

- 系统环境配置: memory_limit cannot be less than 8G

~~~
问题：
[WARN] (127.0.0.1) fs.aio-max-nr must not be less than 1048576 (Current value: 65536)
[WARN] (127.0.0.1) open files number must not be less than 655350 (Current value: 1024)
[WARN] (127.0.0.1) not enough memory

办法：
vi /etc/security/limits.conf
*	 soft	nofile		655360
*	 hard	nofile		655360
*  soft nproc    655360
*  hard nproc    655360

vi /etc/sysctl.conf
fs.file-max = 655360
fs.aio-max-nr=1048576

sysctl -p//生效
ulimit -a //新窗口查看结果

~~~

- mini-local-example.yaml 变量 里面配置是最低配置，不能修改



~~~yaml
[root@56bd78306b21 data]# obd cluster edit-config obtest
oceanbase-ce:
  servers:
    # Please don't use hostname, only IP can be supported
    - 127.0.0.1
  global:
    home_path: /root/data/observer
    # Please set devname as the network adaptor's name whose ip is  in the setting of severs.
    # if set severs as "127.0.0.1", please set devname as "lo"
    # if current ip is 192.168.1.10, and the ip's network adaptor's name is "eth0", please use "eth0"
    devname: lo
    mysql_port: 2881
    rpc_port: 2882
    zone: zone1
    cluster_id: 1
    # please set memory limit to a suitable value which is matching resource.
    memory_limit: 8G //不能修改，至少不多 
    system_memory: 4G //不能修改，至少不多
    stack_size: 512K  //不能修改，至少不多
    cpu_count: 6
    cache_wash_threshold: 500M
    __min_full_resource_pool_memory: 268435456  //不能修改，至少不多
    workers_per_cpu_quota: 10
    schema_history_expire_time: 1d
    # The value of net_thread_count had better be same as cpu's core number.
    net_thread_count: 2
    sys_bkgd_migration_retry_num: 3
    minor_freeze_times: 10
    enable_separate_sys_clog: 0
    enable_merge_by_turn: FALSE
    datafile_disk_percentage: 20
    syslog_level: INFO
    enable_syslog_recycle: true
    max_syslog_file_count: 1

~~~





安装部署



~~~shell
docker update --memory 8G --memory-swap -1 56bd78306b21
docker run -itd  --name oceanbase  --privileged centos /bin/bash
docker start 56bd78306b21 
docker attach 56bd78306b21  //进入容器


obd cluster destroy obtest
obd cluster deploy obtest -c /root/data/mini-local-example.yaml 

obd cluster edit-config obtest
obd cluster redeploy obtest
obd cluster display obtest

obd cluster start obtest
~~~



连接 OceanBase 数据库

https://open.oceanbase.com/quickStart



~~~mysql
obclient -h 127.0.0.1  -P 2881  -uroot 
use test;
CREATE TABLE person ( id int, name int );
insert into person values(1,1);
select * from person;
~~~







查看源码定位启动失败：

~~~c++
启动失败 
cd /root/data/observer/log/
grep ERROR observer.log //请看第一个错误。
  
DEF_INT(__min_full_resource_pool_memory, OB_CLUSTER_PARAMETER, "5368709120", "[268435456,)",
    "the min memory value which is specified for a full resource pool.",
        
DEF_CAP(stack_size, OB_CLUSTER_PARAMETER, "1M", "[512K, 20M]",
    "the size of routine execution stack"
    "Range: [512K, 20M]",
    ObParameterAttr(Section::OBSERVER, Source::DEFAULT, EditLevel::STATIC_EFFECTIVE));
    
~~~



~~~
obd cluster display obtest
Get local repositories and plugins ok
Connect to observer ok
Wait for observer init ok
+---------------------------------------------+
|                   observer                  |
+-----------+---------+------+-------+--------+
| ip        | version | port | zone  | status |
+-----------+---------+------+-------+--------+
| 127.0.0.1 | 3.1.0   | 2881 | zone1 | active |
+-----------+---------+------+-------+--------+




# 什么是 OceanBase Client
https://github.com/oceanbase/obclient/blob/master/README.md
~~~

```

```



Bug:

- https://github.com/oceanbase/oceanbase/issues/107
- https://maimai.cn/web/gossip_detail?gid=29085179&egid=05322459ce5811eb9159246e96b48088
- https://www.oceanbase.com/community/answer/detail?id=292
- https://github.com/oceanbase/obdeploy/issues/4
- https://www.oceanbase.com/community/answer/detail?id=292





## 总结：OceanBase启动过程源码分析：检查配置文件。





















## 具体过程：



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





[2021-06-16 03:32:49.038316] ERROR stop (ob_ddl_task_executor.cpp:176) [257][0][Y0-0000000000000000] [lt=8] invalid tg id BACKTRACE:0x90a3c0e 0x900348b 0x24c2cfb 0x24c03b5 0x625a2f6 0x5ef31c0 0x65e2a4f 0x87001af 0x8701abd 0x24c1415 0x7f20177b9493 0x24c00ee

[2021-06-16 03:32:49.038383] ERROR wait (ob_ddl_task_executor.cpp:181) [257][0][Y0-0000000000000000] [lt=62] invalid tg id BACKTRACE:0x90a3c0e 0x900348b 0x24c2cfb 0x24c03b5 0x625a4e6 0x5ef31c8 0x65e2a4f 0x87001af 0x8701abd 0x24c1415 0x7f20177b9493 0x24c00ee



https://github.com/oceanbase/obdeploy



### 方案1： 使用 RPM 包（Centos 7 及以上）安装。

```
sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://mirrors.aliyun.com/oceanbase/OceanBase.repo
sudo yum install -y ob-deploy
source /etc/profile.d/obd.sh
```

https://github.com/oceanbase/obdeploy/blob/master/example/mini-local-example.yaml

obd cluster destroy obtest
obd cluster deploy obtest -c /root/oceanbase/mini-local-example.yaml 

obd cluster start obtest -v

[2021-06-16 16:24:36.963284] WARN [SHARE] add_extra_config (ob_common_config.cpp:100) [9380][0][Y0-0000000000000000] [lt=25] Invalid config, value out of range(name="memory_limit", value="2G", ret=-4147)

[2021-06-16 16:32:16.971564] ERROR [SERVER] init_config (ob_server.cpp:832) [11202][0][Y0-0000000000000000] [lt=770] invalid config from cmdline options(opts_.optstr_="__min_full_resource_pool_memory=268435456,memory_limit=2G,system_memory=1G,stack_size=512K,cpu_count=16,cache_wash_threshold=20M,workers_per_cpu_quota=10,schema_history_expire_time=1d,net_thread_count=4,sys_bkgd_migration_retry_num=3,minor_freeze_times=10,enable_separate_sys_clog=0,enable_merge_by_turn=False,datafile_disk_percentage=20,enable_syslog_recycle=True,max_syslog_file_count=4", ret=-4147) BACKTRACE:0x90a3c0e 0x900348b 0x24d44b1 0x251e04b 0x87055be 0x8701023 0x24c1415 0x7f69aafd46a3 0x24c00ee



- open files number must not be less than 655350 (Current value: 100001)



~~~
问题：
[WARN] (127.0.0.1) fs.aio-max-nr must not be less than 1048576 (Current value: 65536)
[WARN] (127.0.0.1) open files number must not be less than 655350 (Current value: 1024)
[WARN] (127.0.0.1) not enough memory

办法：
vi /etc/security/limits.conf
*	 soft	nofile		655360
*	 hard	nofile		655360
*  soft nproc    655360
*  hard nproc    655360

vi /etc/sysctl.conf
fs.file-max = 655360
fs.aio-max-nr=1048576

sysctl -p//生效
ulimit -a //新窗口查看结果
~~~



obd cluster edit-config obtest
obd cluster redeploy obtest



```
cat /proc/cpuinfo| grep "cpu cores"| uniq
```

### 阅读：OceanBase 数据库概览 需要 x天

就看这个文档：https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/cluster-management



```javascript
yum install gdb -y
```











