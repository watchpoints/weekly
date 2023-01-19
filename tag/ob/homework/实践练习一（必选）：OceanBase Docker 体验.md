
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


### 五、创建业务租户、数据库、表
