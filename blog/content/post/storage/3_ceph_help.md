---
title: 《大话 Ceph》 之Ceph 运维手册"
date: 2023-06-009
description: "you can"
draft: false
tags: ["CEPH"]
categories: ["CEPH"]
---



http://localhost:1313/post/storage/2_ceph_mon/





## 服务平滑重启



任何一个成熟的项目，必须要提供出接口，就像[探针](https://so.csdn.net/so/search?q=探针&spm=1001.2101.3001.7020)一样，可以让我们探查进程内部的运行情况，进程不能是一个黑盒子。对于ceph而言，Admin Socket 提供了该功能。

Admin Socket 不仅可以查看当前的配置，进程运行的状态，而且，还可以修改配置，获取log等。



有时候需要更改服务的配置，但不想重启服务，或者是临时修改，此时我们就可以通过admin sockets直接与守护进程交互。如查看和修改守护进程的配置参数。

守护进程的socket文件一般是/var/run/ceph/$cluster-$type.$id.asok

基于admin sockets的操作：

- 方式一：tell子命令

  ```
  Usage:
  ceph tell <name (type.id)> help
  ```

  

- 方式二：daemon子命令
  ceph daemon $type.$id command

  
  
- 方式三：通过socket文件
  ceph --admin-daemon /var/run/ceph/$cluster-$type.$id.asok command

  

  常用command如下

  ```bash
  
  ```
  
  ### **3.1 tell子命令**
  
  命令使用格式如下，在管理节点执行即可
  
  ```bash
  ceph tell {daemon-type}.{daemon id or *} injectargs --{name}={value} [--{name}={value}]
  ```

  - daemon-type：为要操作的对象类型如osd、mon等。
  - daemon id：该对象的名称，osd通常为0、1等，mon为ceph -s显示的名称，这里可以输入*表示全部。
  - injectargs：表示参数注入，后面必须跟一个参数，也可以跟多个。

  例如
  
  ```bash
  # 在管理节点运行
  ceph tell mon.mon01 injectargs --mon_allow_pool_delete=true
  ceph tell mon.* injectargs --mon_allow_pool_delete=true
  ```

  mon_allow_pool_delete此选项的值默认为false，表示不允许删除pool，只有此选项打开后方可删除，记得改回去！！！ 这里使用mon.ceph-monitor-1表示只对ceph-monitor-1设置，可以使用*

  ### **3.2 daemon子命令**
  
  命令格式如下，需要登录到守护进程所在的那台主机上执行
  
  ```bash
  ceph daemon {daemon-type}.{id} config set {name}={value} 
  ```

  例。

  ```bash
  ssh root@mon01
  ceph daemon mon.mon01 config set mon_allow_pool_delete false 
  ```
  
  ### **3.3 socket文件**
  
  ```bash
  # 1、查看帮助
  ceph --admin-daemon /var/run/ceph/ceph-mds.mon01.asok help
  
  # 2、查看配置项
  ceph --admin-daemon /var/run/ceph/ceph-mds.mon01.asok config get mon_allow_pool_delete
  
  # 3、设置
  ceph --admin-daemon /var/run/ceph/ceph-mds.mon01.asok config set mon_allow_pool_delete true
  ```
  
  如果超过半数的monitor节点挂掉，此时通过网络访问ceph的所有操作都会被阻塞，但monitor的本地socket还是可以通信的。
  
  ```bash
  ceph --admin-daemon /var/run/ceph/ceph-mon.mon03.asok quorum_status
  ```
  
  

  ## #  维护集群常用命令

  ### mon相关
  
  1、查看mon状态

  ```bash
  ceph mon stat
  ```

  2、查看mon映射信息

  ```bash
  ceph mon dump
  ```
  
  3、检查Ceph monitor仲裁/选举状态

  ```bash
  ceph quorum_status --format json-pretty
  ```
  
  4、查看mon信息包括ip地址

  ```shellbash
  获得一个正在运行的 mon map，并保存在 1.txt 文件中 
  ceph mon getmap -o 1.txt
  monmaptool --print 1.txt
  ```

# wend 



- https://www.mianshigee.com/tutorial/ceph-handbook/troubleshooting-troubleshooting_mon.md

# https://zhuanlan.zhihu.com/p/386561535
