---
title: "Tidb学习记录"
date: 2020-06-09
description: "Tidb"
draft: false
categories: ["FQA"]
---

# 更新日志

| 开始日期      | 更新日期    | 更新内容     |
| --------- | ------------ | -------- |
| 2020-06-09 | 2020-06-09 | 持续更新 |



### 资料
- 文档 https://github.com/pingcap/docs-cn
- FQA 关注个子模块 
     https://asktug.com/c/qa/TiDB
     https://asktug.com/c/qa/sql


### FQA 了解细节

- 架构是什么，我了解他有什么用？

假如你遇到下面问题 MySQL的一些集群方案和数据库中间件都在视图解决当表条目变大时，查询性能下降的问题，可以看看这个

整体:

![image.png](https://i.loli.net/2020/06/09/FHYSRo4i5kqtxOb.png)

![http://ljchen.net/2019/10/12/TiDB%E6%9E%B6%E6%9E%84%E5%8E%9F%E7%90%86/](https://i.loli.net/2020/06/09/WRxH2XB8G6oyZPd.png)

计算-->TiDB

![无状态的TiDB](https://i.loli.net/2020/06/09/7TyclYUAbieNfg4.png)
存储--->TiKV
![TiKV ](https://i.loli.net/2020/06/09/l1vtPCzoVdpgyrU.png)







