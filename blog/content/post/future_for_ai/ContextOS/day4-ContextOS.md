---
title: Trae从青铜到王者
date: 2025-04-12
description: c++
draft: false
tags:
  - 小而美
categories:
  - do book
---


小王同学开发日记第四天

## 我的目标 成为   ContextOS布道师


✅ 任务1： 阅读 ContextOS 云操作系统 提供云研发小程序

✅ 任务2：熟悉框架：后端冰山架( Golang)，前端火山架(JS+CSS)使用方法

✅ 任务3：探索如何模块化方式 从0到1快速开发一个小程序

由于时间有限，如有误，恳请大家不吝指教，万分感激


## 一、从PM角度分析xx设计，思维转变过来

# 二、从程序员角度了解代码实现

## 2.1 我的开发环境

### window 笔记本 
### window 启动wsl2当做服务器（理想状态 需要运桌面，云服务器）

		+-----------------------+
		| Windows 11 操作系统   |
		+-----------------------+
		          |
		          v
		+-----------------------+
		|       Hyper-V         |  <- 提供虚拟化支持
		+-----------------------+
		          |
		          v
		+-----------------------+
		|        WSL 2          |  <- 运行Linux内核
		+-----------------------+
		          |
		          v
		+-----------------------+
		|   Docker Desktop      |  <- 管理容器
		+-----------------------+
		          |
		          v
		+-----------------------+
		| Docker 容器（Linux）  |  <- 运行应用程序
		+-----------------------+

    启动：ubuntu22.04

### 启动ContextOS（任何一个小程序都是一个ContextOS）


- 启动
``` shell
git clone https://shylinux.com/x/ContextOS

cd /home/money/ContextOS
source etc/miss.sh //golang依赖下载
./bin/ice.bin //启动

访问：
http://192.168.45.52:9020/?debug=true

```

### 参考：官方**产品迭代**小程序

 官方地址：https://web.shylinux.com/s/20240903-operation
![产品迭代小程序来管理不同产品](https://s2.loli.net/2025/07/25/ALbzyPGQFMrIc7a.png)


在**产品迭代** 小程序 
不同公司 根据不同产品线 创建不同管理产品的，虽然是同一个小程序，不同产品线相互隔离。
上图 创建了云社区，云监控，云教育 不同产品。都是产品迭代小程序下创建的。

#### 下载 **产品迭代** 小程序 源码

方式1：源码直接下载 git clone https://shylinux.com/x/operation
方式2：从商店下载

![image.png](https://s2.loli.net/2025/07/25/zFfYrEp3d7hVysx.png)

