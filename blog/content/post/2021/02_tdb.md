---
title: "TDengine"
date: 2021-08-30
description: "TDengine"
draft: false
tags: ["book"]
---



##  

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

### 二、这个技术的优势和劣势分别是什么 

### 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

### 四、技术的组成部分和关键点。

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比



https://www.taosdata.com/cn/documentation/

https://hub.fastgit.org/taosdata/TDengine

https://www.taosdata.com/cn/documentation/



# issue

- https://github.com/taosdata/TDengine/issues/7408
- https://hub.fastgit.org/taosdata/TDengine/issues/7503



### 第一天



群公告
#新手任务 #灭虫计划 🎉📢最轻松的新手Contributor任务，来啦！

我们正在推出持续的社群活动——灭虫计划，让大家可以零门槛成为TDengine的Contributor。具体规则：
1. TDengine团队以issue的形式（标签为good first issue）发布任务
2. 感兴趣的同学在issue下回复pick up challenge
3. 在收到assigned it to you的回复后，视为认领成功，就可以开始干活啦～
4. 做完任务后记得将Pull Request提交至develop分支哦～

提交的Pull Request被合并后，即可成为TDengine的Contributor，届时我们会发放带有专属编号的证书以及小礼品～





本期准备了5个关于文档的“虫”，如下：

https://github.com/taosdata/TDengine/issues/7650
https://github.com/taosdata/TDengine/issues/7629
https://github.com/taosdata/TDengine/issues/7628
https://github.com/taosdata/TDengine/issues/7495
https://github.com/taosdata/TDengine/issues/765



https://hub.fastgit.org/

1. 环境 vpn（开发环境）

~~~shell
git clone https://github.com/watchpoints/TDengine
cd TDengine
git submodule update --init --recursive
mkdir debug && cd debug 
//. jemalloc 传说中最强最前沿的内存分配模型. jemalloc(facebook)
cmake .. -DJEMALLOC_ENABLED=true
cmake --build .
sudo make install

TDengine is installed successfully!

To configure TDengine : edit /etc/taos/taos.cfg
To start TDengine     : sudo systemctl start taosd
To access TDengine    : use taos in shell

TDengine is installed successfully!

./build/bin/taosd -c test/cfg
taos

create database db;
use db;
create table t (ts timestamp, a int);
insert into t values ('2019-07-15 00:00:00', 1);
insert into t values ('2019-07-15 01:00:00', 2);
select * from t;
drop database db;

~~~



















