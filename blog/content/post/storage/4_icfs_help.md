---
title: "日拱一卒：操作手册"
date: 2023-08-023
description: "you can"
draft: false
tags: ["CEPH"]
categories: ["CEPH"]
---





## 系统罐装

### 1. Linux 如何查看网络带宽是千兆还是万兆

~~~
ethtool ens160
Speed: 10000Mb/s
~~~

### 2 .CentOS 8 网络管理：使用 nmcli 和 nmtui 进行高效配置

### 3, ssh 设置远程登录

/etc/ssh/sshd_config





# cifs



[mds减少mds 进程个数]
 systemctl stop icfs-mds@N

 再 删除 

/etc/systemd/system/icfs-mds.target.wants/icfs-mds@N.service ;

 systemctl daemon-reload  

 rm -rf /var/lib/icfs/mds/icfs-N





### 3  mds一致处于 reply状态怎么处理。 

reply --osd 读取日志信息 --

icfs ods  tree

高可用配置。





