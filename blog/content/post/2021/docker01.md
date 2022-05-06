---
title: "docker入门教程"
date: 2021-08-25
description: "docker入门教程"
draft: false
tags: ["code_reading"]
---





# 目标：  



> 容器实战高手课 在实战中深入理解容器技术的本质



- Linux 性能优化实战
- Linux 内核技术实战课
- 深入剖析 Kubernetes



当你不太理解为什么要学习 Kubernetes 项目的时候，

或者，你在学习 Kubernetes 项目感到困难的时候，

不妨想象一下 Kubernetes 就是未来的 Linux 操作系统。



>之前看到这个东西时候感觉很好，工作不用，然后慢慢学，结果自己忘记了。

# 摘要：

### 容器网络 (4讲)

15 | 容器网络：我修改了/proc/sys/net下的参数，为什么在容器中不起效

- 重要文档  !!! https://www.kernel.org/doc/Documentation/sysctl/net.txt

ls /proc/sys/net/core. busy_poll rmem_max somaxconn

![img](https://static001.geekbang.org/resource/image/ba/61/ba6c9ed0dcccc7f4f46bb19c69946e61.png)

Linux 系统中，常见的动态追踪方法包括 ftrace、perf、eBPF 以及 SystemTap 等。当你已经定位了某个内核函数，但不清楚它的实现原理时，就可以用 ftra[ce 来跟踪它的执行过程。至](https://time.geekbang.org/column/article/87342?utm_source=website&utm_medium=geektime&utm_campaign=322-presell&utm_content=PC0701&utm_term=pc_interstitial_398)



![img](https://static001.geekbang.org/resource/image/bc/6f/bc04236262f16e0b69842dafd503616f.jpg)

----------------------------------------

strace -T -tt -ff -p pid -o strace.out

https://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1/



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



## 2.2 第二本：Docker 源码分析

本文主要剖析 docker-ce 的源码。

docker-ce github:
[https://github.com/docker/docker-ce](https://links.jianshu.com/go?to=https%3A%2F%2Fgithub.com%2Fdocker%2Fdocker-ce)



## 2.3 第三本：容器实战高手课 在实战中深入理解容器技术的本质

1. 16 | 容器网络配置（1）：容器网络不通了要怎么调试?

-  Network Namespace 隔离了网络设备，IP 协议栈和路由表，以及防火墙规则



~~~
docker run -d --name if-test centos:8.1.1911 sleep 36000
docker exec -it if-test bash
exit

docker ps -a
docker stop 6e6decd1566a
docker rm 6e6decd1566a
docker run -d --name if-test --network none centos:8.1.1911 sleep 36000

docker exec -it if-test ip addr

Network Namespace 里就只有 loopback 一个网络设备，而没有了 eth0 网络设备了。

~~~

- 60秒思考：让容器 Network Namespace 中的数据包最终发送到物理网卡上，需要完成哪些步骤呢？

用 Docker 启动的容器缺省的网络接口用的也是这个 veth



你可以查看一下Docker 网络的文档或者Kubernetes 网络的文档，这些文档里面介绍了很多种容器网络配置的方式。







目标：  



> 容器实战高手课 在实战中深入理解容器技术的本质



摘要：

15 | 容器网络：我修改了/proc/sys/net下的参数，为什么在容器中不起效
