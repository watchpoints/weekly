---
title: "docker入门教程"
date: 2021-08-25
description: "docker入门教程"
draft: false
tags: ["code_reading"]
---




# docker 

# 第一天

## 基本操作

https://hub.fastgit.org/docker

https://github.com/docker



### 安装

~~~
[root@fhb ~]# cat /etc/redhat-release
CentOS release 6.8 (Final)
[root@fhb ~]# sudo rpm -iUvh http://dl.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm
[root@fhb ~]# sudo yum update -y

curl -sSL https://get.docker.com/ | sh
docker version
service docker start

yum install -y nginx
service nginx start
~~~



### docker 基本操作

- 启动

  ```shell
  systemctl restart docker
  ```

- 停止所有的容器

​    docker stop $(docker ps -aq)



- 删除所有的容器

 docker rm $(docker ps -aq)

- 删除所有的镜像

docker rmi $(docker images -q)

- 查看指定时间后的日志，只显示最后100行：

```shell
$ docker logs -f -t --since="2018-02-08" --tail=100 CONTAINER_ID
$ docker logs --since 30m CONTAINER_ID
docker logs -f --tail=100 f215dfab44fd
```





## 二、BOOK

###  2.1 第一本 https://yeasy.gitbook.io/docker_practice/ **Docker技术入门与实战（第3版）**

https://yeasy.gitbook.io/docker_practice/



### 2.1.1 底层实现



Docker 底层的核心技术包括 Linux 上的命名空间（Namespaces）、控制组（Control groups）、Union 文件系统（Union file systems）和容器格式（Container format）





## day1: OceanBase开发者手册

> 环境说明：因为公司没有合适机器，后面一切环境都是macos上进行的。

- macOS 下使用 Docker 搭建 ubuntu 环境



~~~shell
https://github.com/oceanbase/oceanbase/wiki/how_to_build

## 获取 ubuntu 镜像（推荐centos，我才用ubuntu就是因为上面集成）
docker pull ubuntu

##macOS 的终端中执行命令 
ssh -p 26122 root@localhos
~/.ssh/config
Host learn
HostName localhost
User     root
Port     26122

~~~



