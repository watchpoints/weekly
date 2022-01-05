
https://open.oceanbase.com/answer/detail?id=13700695
## 期望目标

- 练习目的

本次练习目的是通过 OceanBase Docker 容器，快速的体验 OceanBase 的 自动化部署过程，以及了解 OceanBase 集群安装成功后的目录特点和使用方法。

- 练习条件

有笔记本或服务器，内存至少12G 。
操作系统不限，能安装 Docker 环境即可。
练习内容
请记录并分享下列内容：


- （必选）下载Docker 镜像：https://hub.docker.com/repository/docker/obpilot/oceanbase-ce 。

- （必选）使用 OBD 命令完成后续的 OceanBase 集群部署。
- （必选）创建一个业务租户、一个业务数据库，以及一些表等。


## 一、检查系统环境
- 操作系统版本  CentOS7
~~~
 cat /etc/redhat-release
 CentOS Linux release 7.7.1908 (Core)
~~~
- 查看版本内核

 系统内核需要 3.10 （包括）以上
~~~
cat /proc/version
Linux version 3.10.0-693.el7.x86_64 (builder@kbuilder.dev.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-16) (GCC) ) #1 SMP Tue Aug 22 21:09:27 UTC 2017

该系统使用的是Linux 3.10 内核的64为操作系统。

GCC 为GUN 编译器集合，采用4.8版本。

uname -r
3.10.0-693.el7.x86_64

安装Docker要求Centos内核版本高于3.10


~~~

- 内存

 有笔记本或服务器，内存至少12G 。

## 二、命令安装 Docker 镜像



### 2.1 根据系统配置,选择同步安装方式，我这里是centos
直接选择
https://docs.docker.com/engine/install/centos/

说明：

- window 安装方式。因为我系统只有8G不适合
https://open.oceanbase.com/answer/detail?id=508

~~~
sudo yum install -y yum-utils
sudo yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo

sudo yum install docker-ce docker-ce-cli containerd.io

~~~

> 执行结果

~~~
Error: Package: docker-ce-rootless-extras-20.10.11-3.el7.x86_64 (docker-ce-stable)
           Requires: fuse-overlayfs >= 0.7
Error: Package: docker-ce-rootless-extras-20.10.11-3.el7.x86_64 (docker-ce-stable)
           Requires: slirp4netns >= 0.4
           Available: slirp4netns-0.3.0-1.el7.x86_64 (gomedc-extras)
               slirp4netns = 0.3.0-1.el7
~~~

原因分析


- 排查内核版本底因素,

Docker 要求 CentOS 系统的内核版本 3.10 

我的Linux version 3.10.0-693.el7.x86_64 符合条件，符合条件
我期望最简单方式 命令安装。不是采取package方式


- 从官方社区选择答案 

https://github.com/docker/docker-install/issues/204
~~~
/etc/yum.repos.d/docker-ce.repo

[centos-extras]
name=Centos extras - $basearch
baseurl=http://mirror.centos.org/centos/7/extras/x86_64
enabled=1
gpgcheck=1
gpgkey=http://centos.org/keys/RPM-GPG-KEY-CentOS-7
yum -y install slirp4netns fuse-overlayfs container-selinux
~~~



###  2.2 服务启动


- 启动关闭docker  

sudo systemctl start docker

或者
service docker start | stop
 
- 查看版本 

sudo docker version

~~~
sudo docker version
Client: Docker Engine - Community
 Version:           20.10.11
 API version:       1.41
 Go version:        go1.16.9
 Git commit:        dea9396
 Built:             Thu Nov 18 00:38:53 2021
 OS/Arch:           linux/amd64
 Context:           default
 Experimental:      true

~~~

- 日志信息
docker logs
- 故障检查
service docker status


### 三. Docker方式部署 OceanBase 

- 直接执行命令 

~~~
https://open.oceanbase.com/articles/8600142

docker search oceanbase

docker pull oceanbase/obce-mini

docker run -p 2881:2881 --name obce-mini -d -e OB_HOME_PATH="/app/data/docker" -e OB_TENANT_NAME="test" oceanbase/obce-mini

查看容器启动日志
docker logs obce-mini | tail -1

Start observer ok
observer program health check ok
Connect to observer ok
Initialize cluster
Cluster bootstrap ok
Wait for observer init ok

分析上面日志可以看出几点信息：

会安装两个软件包：oceanbase-ce-lib 和 oceanbase-ce-3.1.0 。
先初始化集群目录。
然后初始化集群（bootstrap）。
再初始化业务租户（tenant）。

进入容器

docker exec -it obce-mini bash


查看 OceanBase 工作目录结构
yum -y install tree
tree /root/ob

mysql -uroot -h127.1 -P2881


~~~

- 参数说明

镜像地址

 镜像的源码地址在 Github 上：
 https://github.com/oceanbase/oceanbase/tree/master/tools/docker/mini 

 启动相关参数：
 
 OB_HOME_PATH 部署路径

 OB_TENANT_NAME 默认租户名称

 ### 查看默认启动 obd 

~~~
 obd cluster list
+--------------------------------------------------------+
|                      Cluster List                      |
+---------+----------------------------+-----------------+
| Name    | Configuration Path         | Status (Cached) |
+---------+----------------------------+-----------------+
| mini-ce | /root/.obd/cluster/mini-ce | running         |
+---------+----------------------------+-----------------+

##obd cluster start

obclient -uroot -h127.1 -P2881(没有启动代理)

docker ps -a
docker stop obce-mini
docker start obce-mini
进入容器
docker exec -it obce-mini bash
~~~

### 相关问题

- 问题1  Docker容器启动报WARNING: IPv4 forwarding is disabled. Networking will not work
~~~
vi /etc/sysctl.conf
net.ipv4.ip_forward=1
systemctl restart network
sysctl net.ipv4.ip_forward
~~~



### 四、 obd 部署集群 

>机器配置不高，这里部署一个单机

1. 在容器内部使用obd停止mini-ce集群

obd cluster stop mini-ce

>stop           stop a cluster had started

2. 创建
https://gitee.com/oceanbase/obdeploy/blob/master/example/local-example.yaml

cd /app/data
touch mini-local-example.yaml
//
obd cluster deploy temp -c ./mini-local-example.yaml

>deploy         use current deploy config or an deploy yaml file to deploy a cluster

~~~

[root@e75dcac1b8ab data]# obd cluster deploy temp -c ./mini-local-example.yaml

oceanbase-ce-3.1.0 already installed.
+-----------------------------------------------------------------------------+
|                                   Packages                                  |
+--------------+---------+---------+------------------------------------------+
| Repository   | Version | Release | Md5                                      |
+--------------+---------+---------+------------------------------------------+
| oceanbase-ce | 3.1.0   | 2.el7   | afd11d52f83eef4b456d77969fde620c4bfba85e |
+--------------+---------+---------+------------------------------------------+

127.0.0.1 initializes cluster work home
temp deployed
~~~

3. 观察结果

obd cluster start temp

- 错误1 docker 下根本没有 netstat

yum install net-tools

- 错误2  [ERROR] 127.0.0.1:2881 port is already used

~~~

obd cluster list
+--------------------------------------------------------+
|                      Cluster List                      |
+---------+----------------------------+-----------------+
| Name    | Configuration Path         | Status (Cached) |
+---------+----------------------------+-----------------+
| temp    | /root/.obd/cluster/temp    | deployed        |
| mini-ce | /root/.obd/cluster/mini-ce | running         |
+---------+----------------------------+-----------------+

~~~

obd cluster start temp

~~~
Start observer ok
observer program health check ok
Connect to observer ok
Initialize cluster
Cluster bootstrap ok
Wait for observer init ok
+---------------------------------------------+
|                   observer                  |
+-----------+---------+------+-------+--------+
| ip        | version | port | zone  | status |
+-----------+---------+------+-------+--------+
| 127.0.0.1 | 3.1.0   | 2881 | zone1 | active |
+-----------+---------+------+-------+--------+

~~~
4. 销毁：或者停止 stop

obd cluster stop temp
obd cluster destroy temp


### 五、创建业务租户、数据库、表

#### 5.1 基本语法

>思考 用户和租户关系
>每个租户对应一定的资源

obclient -u[用户名]@[租户名]#[集群名称] -P[端口号] -h[ip地址] -p[密码] -D[数据库名] -c

- MySQL 租户的管理员用户名默认是root
-  默认租户 sys
- 数据库名 oceanbase

obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase


### 5.2 创建租户 

> 计算集群剩余可用资源的目的是为了避免创建业务租户时不会碰到资源不足的报错。

资源不够了

~~~
MySQL [oceanbase]> select a.zone,concat(a.svr_ip,':',a.svr_port) observer, cpu_total, cpu_assigned, (cpu_total-cpu_assigned) cpu_free, mem_total/1024/1024/1024 mem_total_gb, mem_assigned/1024/1024/1024 mem_assign_gb, (mem_total-mem_assigned)/1024/1024/1024 mem_free_gb  from __all_virtual_server_stat a join __all_server b on (a.svr_ip=b.svr_ip and a.svr_port=b.svr_port) order by a.zone, a.svr_ip;
+-------+----------------+-----------+--------------+----------+----------------+----------------+----------------+
| zone  | observer       | cpu_total | cpu_assigned | cpu_free | mem_total_gb   | mem_assign_gb  | mem_free_gb    |
+-------+----------------+-----------+--------------+----------+----------------+----------------+----------------+
| zone1 | 127.0.0.1:2882 |        14 |         11.5 |      2.5 | 4.000000000000 | 3.500000000000 | 0.500000000000 |
+-------+----------------+-----------+--------------+----------+----------------+----------------+----------------+
~~~

> 遇到第一个问题： 资源不足如何处理？这里假设机器足够分配。
~~~
# 使用 edit-config 命令进入编辑模式，修改集群配置

obd cluster edit-config mini-ce
memory_limit: 16G
cpu_count: 32
# 保存并退出后，obd 会告知您如何使得此次改动生效
# 此配置项仅需要 reload 即可生效
obd cluster reload mini-ce
~~~

查看资源:cpu 没有成功，但是内存可以了。

~~~
MySQL [oceanbase]> select a.zone,concat(a.svr_ip,':',a.svr_port) observer, cpu_total, cpu_assigned, (cpu_total-cpu_assigned) cpu_free, mem_total/1024/1024/1024 mem_total_gb, mem_assigned/1024/1024/1024 mem_assign_gb, (mem_total-mem_assigned)/1024/1024/1024 mem_free_gb  from __all_virtual_server_stat a join __all_server b on (a.svr_ip=b.svr_ip and a.svr_port=b.svr_port) order by a.zone, a.svr_ip;
+-------+----------------+-----------+--------------+----------+-----------------+----------------+----------------+
| zone  | observer       | cpu_total | cpu_assigned | cpu_free | mem_total_gb    | mem_assign_gb  | mem_free_gb    |
+-------+----------------+-----------+--------------+----------+-----------------+----------------+----------------+
| zone1 | 127.0.0.1:2882 |        14 |         11.5 |      2.5 | 12.000000000000 | 3.500000000000 | 8.500000000000 |
+-------+----------------+-----------+--------------+----------+-----------------+----------------+------------
~~~

#### 5.2 创建资源池

思路：资源单元和 资源池关系？

>创建资源单元规格（RESOURCE UNIT)

小贴士：

- 创建资源单元规格，并不会立即分配资源，为了测试通过创建最低
- 最大最小设置一样。

~~~
create resource unit unit_demo max_cpu=1, min_cpu=1, max_memory='1G', min_memory='1G', max_iops=10000, min_iops=1000, max_session_num=1000000, max_disk_size='1G';


select * from __all_unit_config \G

*************************** 3. row ***************************
     gmt_create: 2021-12-29 16:00:42.486033
   gmt_modified: 2021-12-29 16:00:42.486033
 unit_config_id: 1002
           name: unit_demo
        max_cpu: 1
        min_cpu: 1
     max_memory: 1073741824
     min_memory: 1073741824
       max_iops: 10000
       min_iops: 1000
  max_disk_size: 1073741824
max_session_num: 1000000
3 rows in set (0.001 sec)

~~~

- 查看资源分配情况:unit_demo 还没有被被使用

~~~

MySQL [oceanbase]> select t1.name resource_pool_name, t2.`name` unit_config_name, t2.max_cpu, t2.min_cpu, t2.max_memory/1024/1024/1024 max_mem_gb, t2.min_memory/1024/1024/1024 min_mem_gb, t3.unit_id, t3.zone, concat(t3.svr_ip,':',t3.`svr_port`) observer,t4.tenant_id, t4.tenant_name
    -> from __all_resource_pool t1 join __all_unit_config t2 on (t1.unit_config_id=t2.unit_config_id)
    ->     join __all_unit t3 on (t1.`resource_pool_id` = t3.`resource_pool_id`)
    ->     left join __all_tenant t4 on (t1.tenant_id=t4.tenant_id)
    -> order by t1.`resource_pool_id`, t2.`unit_config_id`, t3.unit_id
    -> ;
+--------------------+------------------+---------+---------+----------------+----------------+---------+-------+----------------+-----------+-------------+
| resource_pool_name | unit_config_name | max_cpu | min_cpu | max_mem_gb     | min_mem_gb     | unit_id | zone  | observer       | tenant_id | tenant_name |
+--------------------+------------------+---------+---------+----------------+----------------+---------+-------+----------------+-----------+-------------+
| sys_pool           | sys_unit_config  |       5 |     2.5 | 1.199999999255 | 1.000000000000 |       1 | zone1 | 127.0.0.1:2882 |         1 | sys         |
| test               | test             |       9 |       9 | 2.500000000000 | 2.500000000000 |    1001 | zone1 | 127.0.0.1:2882 |      1001 | test        |
+--------------------+------------------+---------+---------+----------------+----------------+---------+-------+----------------+-----------+-------------+

~~~

>创建资源池

小贴士

- unit_num 表示多少份。更说明这是基本单元
- 参数 zone_list 表示资源池的 Zone 列表


create resource pool pool_troy unit='unit_demo', unit_num=1, zone_list=('zone1');

>创建租户

~~~
CREATE TENANT IF NOT EXISTS test_troy
    -> charset='utf8mb4',
    -> replica_num=3,
    -> zone_list=('zone1'),
    -> primary_zone='zone1',
    -> resource_pool_list=('pool_troy');
Query OK, 0 rows affected (11.333 sec)
~~
小贴士：
- replica_num：租户在创建时候指定副本数量
- zone_list 副本分布
- primary_zone



https://open.oceanbase.com/docs/community/oceanbase-database/V3.1.0/create-a-tenant-1


### 参考
- https://www.cnblogs.com/pursuing-dreams/p/15137318.html
- 参考 实战教程第三章3.2：如何创建和连接 MySQL 租户
- 实战教程第三章3.1：查看 OceanBase 集群资源的使用情况



