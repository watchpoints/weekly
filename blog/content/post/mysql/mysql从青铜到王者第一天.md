---
title: "mysql从青铜到王者第一天"
date: 2023-08-09
draft: false
tags: ["mysql"]
---





# day1

### 一、问题

- msyql 忘记root密码怎么办？



## 二、过程

- mysql8.0安装

~~~mysql

在mysql官网获取yum源安装包
http://dev.mysql.com/downloads/repo/yum/
复制下载链接地址为：
mysql80-community-release-el7-7.noarch.rpm


A Quick Guide to Using the MySQL Yum Repository
wget https://dev.mysql.com/get/mysql80-community-release-el7-7.noarch.rpm
rpm -ivh mysql80-community-release-el7-7.noarch.rpm
yum install mysql-community-server -y

 启动mysql
service mysqld start

# 关闭mysql
service mysqld stop

# 查看启动状态
service mysqld status

# 重启mysql
service mysqld restart
# 查看默认密码
sudo grep 'temporary password' /var/log/mysqld.log


因此，请遵守容器设计原则，一个容器里运行一个前台服务！
报错1:
docker centos 使用 systemctl Failed to get D-Bus connection: Operation not permitted

docker ps -a
3346da816048
docker start 3346da816048
ssh root@10.2.131.60 -p 22 （宿主机器IP）

docker exec -it 3346da816048   /usr/sbin/init 

   docker run --privileged  -ti -e "container=docker"  -v /sys/fs/cgroup:/sys/fs/cgroup  centos  /usr/sbin/init
   
创建容器：
docker-failed-to-get-d-bus-connection-operation-not-permitted
# docker run -d -name centos7 --privileged=true centos:7 /usr/sbin/init
进入容器：
# docker exec -it centos7 /bin/bash
这样可以使用systemctl启动服务了。


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



分享地址：

- http://localhost:1313/post/mysql/day1_mysql_demo/
- https://wangcy6.gitbook.io/do_book/2023
- https://wangcy6.github.io/post/mysql/day1_mysql_demo/

