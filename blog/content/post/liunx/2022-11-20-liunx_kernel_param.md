---
title: Linux内核参数配置入门
date: 2022-11-20
description: ""
draft: false
tags:
  - book
r:
---


Q3 如何判断一个进程启动时间，什么时候启动的？




Q2: 一个资源，被多线程操作，需要加锁吗?


  只要涉及写的，就需要加锁处理


Q1：nohup和&运行的后台命令，但是查找不到对应进程名字 该怎么处理

```css
nohup test.sh > test.log 2>&1 & 



```



## 如何学习

> 在本文中，我将试着去回答liunx 内核参数：
> Q1 (Why): 为什么需要协程？
> 我们会一起回顾协程出现的历史背景，当时要解决什么问题；同时，现在是什么场景，需要使用协程来进行处理？为什么进程或者线程不能够很好地满足当下的使用场景？
> Q2 (What): 到底什么是协程？
> 我们一直在谈论协程。由于协程中包含有「程」这个字眼，因此经常被拿来与进程线程进行对比，称协程为「用户态线程」；但又有人从协程实现的角度，说「协程是一种泛化的函数」。这就把我们给绕晕了。我们不禁要问，到底什么是协程？在本次分享中，我会试着进行回答。
> Q3 (How): 怎么实现协程 (库)？
> 在回答了协程是什么之后，第三个问题就紧随而来，我们可以自己去实现一个简单的协程或者协程库吗？如果可以实现，那我们应该怎么实现呢？
> Q4 (Usage): 使用协程时需要注意什么？
> 通过实际编码实现一个简单的协程库后，我们再来看 libco 的实现，就会清晰明了。我们会在第四部分介绍使用协程时需要注意的一些问题。
> 这就是我本次分享想要达成的目标 —— 回答这四个问题。

~~~
 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题 

 二、这个技术的优势和劣势分别是什么 

三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比
~~~





# # /etc/sysctl.conf





~~~
/etc/sysctl.conf
net.ipv4.tcp_keepalive_time = 60 
net.ipv4.tcp_keepalive_probes=3 
net.ipv4.tcp_keepalive_intvl=60

sysctl -p 

sysctl -a | grep keepalive
net.ipv4.tcp_keepalive_intvl = 10
net.ipv4.tcp_keepalive_probes = 75
net.ipv4.tcp_keepalive_time = 60
[root@server01 ~]#

KeepAlive默认情况下是关闭的，可以被上层应用开启和关闭
tcp_keepalive_time: KeepAlive的空闲时长，或者说每次正常发送心跳的周期，默认值为7200s（2小时）
tcp_keepalive_intvl: KeepAlive探测包的发送间隔，默认值为75s
tcp_keepalive_probes: 在tcp_keepalive_time之后，没有接收到对方确认，继续发送保活探测包次数，默认值为9（次）

~~~



如何排查Linux slab_unreclaimable内存占用高的原因？

https://help.aliyun.com/document_detail/316787.html



- vm.vfs_cache_pressure
  系统在进行内存回收时，会先回收page cache, inode cache, dentry cache和swap cache。

  vfs_cache_pressure越大，每次回收时，inode cache和dentry cache所占比例越大。

  vfs_cache_pressure默认是100，值越大inode cache和dentry cache的回收速度会越快，越小则回收越慢，为0的时候完全不回收(OOM!)。



# # /etc/security/limits.conf

## 

### Q1 (Why): 使用场景

### Q2 (What):: 原理

### Q3 (How): 怎么实

~~~
* hard memlock unlimited
* soft memlock unlimited
~~~







### perf工具使用



perf record   -g -p 30584
perf report -n --stdio

























