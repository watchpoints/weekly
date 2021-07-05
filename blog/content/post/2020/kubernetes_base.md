---
title: "从零开始k8s"
date: 2021-06-20
description: "Kubernetes中文文档"
draft: false
tags: ["中间件"]
---



##  

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

### 二、这个技术的优势和劣势分别是什么 

### 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景



### 四、技术的组成部分和关键点。

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比

https://www.kubernetes.org.cn/k8s

http://docs.kubernetes.org.cn/



## 第一天



##  # todo-01 用其他人身份购买3年云主机 不超过500元

 





### 01-Mac中搭建Kubernetes（方法1 个人笔记）

- 我们推荐基于Docker的本地方案。

- Preferrences->Kubernetes中勾选Enable Kubernetes



### 02 - centos 环境搭建（方法2 云主机）



~~~shell
来源：https://docs.docker.com/engine/install/centos/
# docker 安装
yum install -y yum-utils 
yum-config-manager \
    --add-repo \
    http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo ##设置Docker仓库（这里使用阿里云地址）

yum makecache  ## 更新yum软件包索引
yum -y install docker-ce docker-ce-cli containerd.io ##安装最新版本的Docker
systemctl start docker ## 启动docker

docker -v
Docker version 20.10.7, build f0df350
docker info

# 使用kubeadm快速搭建单机kubernetes集群
https://www.jianshu.com/p/70efa1b853f5
## 设置yum源
curl -o CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo

curl -o docker-ce.repo https://download.docker.com/linux/centos/docker-ce.repo
## 配置kubernetes的yum源
cat <<EOF > /etc/yum.repos.d/kubernetes.repo
[kubernetes]
name=Kubernetes
baseurl=http://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=0
repo_gpgcheck=0
gpgkey=http://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg
        http://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF

systemctl stop firewalld
systemctl disable firewalld
setenforce 0

## 执行下列命令刷新yum源缓存
yum clean all
yum makecache 
yum repolist

## CentOS kubernetes单机部署

yum install -y kubeadm

一共会安装kubelet、kubeadm、kubectl、kubernetes-cni这四个程序。
kubeadm：k8集群的一键部署工具，通过把k8的各类核心组件和插件以pod的方式部署来简化安装过程

kubelet：运行在每个节点上的node agent，k8集群通过kubelet真正的去操作每个节点上的容器，由于需要直接操作宿主机的各类资源，所以没有放在pod里面，还是通过服务的形式装在系统里面

kubectl：kubernetes的命令行工具，通过连接api-server完成对于k8的各类操作

kubernetes-cni：k8的虚拟网络设备，通过在宿主机上虚拟一个cni0网桥，来完成pod之间的网络通讯，作用和docker0类似。
kubeadm version


~~~







### 02 -[Kubernetes设计架构](https://www.kubernetes.org.cn/kubernetes设计架构)

- ### Kubernetes设计理念与分布式系统（学习分布式系统经典入门案例）























## 塔山

- 一键部署利器 Kubeadm 搭建 Kubernetes

  https://www.jianshu.com/p/7a3ccb2b7856

  https://www.jianshu.com/p/70efa1b853f5

  https://blog.csdn.net/heian_99/article/details/103933928

  

- https://blog.csdn.net/weixin_43423864/article/details/109481260 【耗时 15分钟 ，完成】
- https://www.jianshu.com/p/70efa1b853f5

- http://www.fwhyy.com/2019/05/building-kubernetes-in-mac/
- 