---
title: "源码解析：ceph数据读写"
date: 2024-03-28
description: "you can"
draft: false
tags: ["CEPH"]
categories: ["CEPH"]
---

## 1. 需求

## [【ceph】ceph-mds journal模块解读](https://www.cnblogs.com/bandaoyu/p/16752032.html "发布于 2022-10-04 01:21")

Journaler类负责将内存中已经序列化的数据写入到对应metadata pool中的obj中，同时提供读取该obj的接口

当mds处理客户端请求后，都分装成LogEvent事件，通过journal_and_reply函数中的submit_mdlog_entry,提交到pending_events集合中，再通过submit_thread线程，将LogEvent事件刷新到metadata pool中。
## 2. 架构设计

## 3. 代码实现


MDLog::_submit_thread()


## 4. 问题定位




### 5. 环境复现

-  vdbench 配置案例及参数说明
- 