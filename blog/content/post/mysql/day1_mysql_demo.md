---
title: "mysql内核学习过程"
date: 2022-02-27
draft: false
tags: ["mysql"]
---





# day1

### 一、问题

- msyql 忘记root密码怎么办？



## 二、过程

- mysql8.0安装

~~~mysql
#8.0 在centos6.8上安装不上 需要
A Quick Guide to Using the MySQL Yum Repository
wget https://dev.mysql.com/get/mysql80-community-release-el8-3.noarch.rpm
rpm -ivh mysql80-community-release-el8-3.noarch.rpm 

cat /etc/redhat-release
CentOS release 6.8 (Final)
#7.9 安装不删
rpm -e --nodeps  mysql80-community-release-el8-3
wget https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm
rpm -ivh mysql57-community-release-el7-9.noarch.rpm

https://blog.csdn.net/wohiusdashi/article/details/89358071
 Requires: libstdc++.so.6(GLIBCXX_3.4.15)(64bit)
 
 
# Centos6.8通过yum安装mysql5.7 

netstat -lntup |  grep mysql
wget https://dev.mysql.com/get/mysql57-community-release-el6-9.noarch.rpm
rpm -Uvh mysql57-community-release-el6-9.noarch.rpm 


# Server version: 5.6.51


容器方式安装mysql：
CentOS6.8安装Docker

yum install -y epel-release
EPEL的全称叫 Extra Packages for Enterprise Linux 。EPEL是由 Fedora 社区打造，为 RHEL 及衍生发行版如 CentOS、Scientific Linux 等提供高质量软件包的项目。装上了 EPEL之后，就相当于添加了一个第三方源。


yum install docker-io
解决CentOS6.8上安装docker失败：No package docker-io available

https://www.cnblogs.com/ybyn/p/13698058.html


1. docker pull mysql 默认拉取最新版本的镜像
2. docker run -itd --name mysql8.0 -p 3306:3306  -e MYSQL_ROOT_PASSWORD=123456  mysql
3 docker exec -it 7e1c7fc8da97 /bin/bash

docker logs 7e1c7fc8da97
FATAL: kernel too old 
~~~

- 基本操作

~~~mysql

mysql -h 127.0.0.1 -u root -p -P 3306

service docker start


 
systemctl start mysqld

# 查找以前是否装有MySQL
rpm -qi mysql-server
Name        : mysql-server
Version     : 8.0.26
# 启动：
service mysqld start
# 默认空密码
mysql -uroot 


~~~



## 三 结论

- centos6.8版本无法安装mysql8.0，容器的方式也不行。

