---
title: ContextOS 模块开发与架构解析笔记
date: 2025-04-07
description: c++
draft: false
tags:
  - 小而美
  - ContextOS
categories:
---

本文目标

✅ 阅读 ContextOS 云操作系统 提供云研发小程序

✅ 熟悉框架：后端冰山架( Golang)，前端火山架(JS+CSS)使用方法

✅ 探索如何模块化方式 从0到1快速开发一个小程序

由于时间有限，如有误，恳请大家不吝指教，万分感激

## 1. 参考相关资源

- 云研发小程序 代码地址  https://git.shylinux.com/web/operation
- 云研发小程序 演示地址 ：https://web.shylinux.com/s/20240903-operation/c/web.team.production.portal
- ContextOS平台说明  https://web.shylinux.com  https://shylinux.com/
-  框架相关文档：https://web.shylinux.com/c/portal/development/#preface/flow.shy
- 相关视频：
- 1. https://meeting.tencent.com/cw/NLkkokOnf0
- 2. ContextOS 安装流程与组建集群

**云轩领航** 2025年01月21日 21:50 广东

，时长

视频号

## 2. 新概念对原有事物封装 ，需要理解扮演生态位（简单了解 3分钟）

David Wheeler（剑桥大学计算机科学教授）说过一句话 

> All problems in computer science can be solved by another level of indirection

计算机科学中的所有问题都可以通过增加一个间接层来解决。

操作系统有一个特点分层设计，通过上层 掉用 来隐藏底层复杂实现。


为了一个直观理解 ，这解释下ContextOS 官方文档：

- ContextOS 只有 20M 大小，集成了编辑器、编译器、前端构架、后端构架、代码库、数据库等工具。

- ContextOS 后端只需要一个进程，前端只需要一个网页，即可使用所有功能，不依赖任何外部代码与工具。


它是建立在浏览器上的一个操作系统，它扮演生态角色 我们后面慢慢体会 。

这里对比其产品扮演角色。

 案例1：Tidb公司推出了在浏览器上运行数据库，
 
 实现原理：
 为什么 浏览器能运行跑数据库呢？Go 语言支持了 WebAssembly（Wasm），
 TiDB 编译成 Wasm，在浏览器里直接运行生成的 Wasm 文件，
 这就使得在浏览器里运行一个数据库成为了现实。
 
 案例2:运行在浏览器运行Linux作系统  
 - 体验浏览器上操作系统 https://bellard.org/jslinux/
 实现原理：
 - CPU仿真器使用的是QEMU
 - 磁盘用的是Ram Disk
 - Linux  使用了2.6.20内核
 - 在Home目录下有一个hello.c的程序，你可以使用[TinyCC](编
案例3: [Puter 基于 Web 的桌面操作系统](https://www.oschina.net/p/puter "Puter 基于 Web 的桌面操作系统")

  开源互联网操作系统：您的所有文件、应用程序和游戏都集中在一个地方，可随时随地访问。
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407141333.png)
Puter 是基于 Web 的桌面操作系统，运行于浏览器中，具有丰富的功能、速度极快且可高度扩展。它可用于构建远程桌面环境或用作云存储服务、远程服务器、Web 托管平台等的界面。

Puter 选择采用原生 JavaScript 和 jQuery 编写，而没有使用前端三大框架（React、Vue、Angular），作者解释这是出于性能方面的考虑 —— 希望避免复杂的抽象并尽可能保持对整个技术栈的控制。
- 体验地址 https://puter.com/

 
划重点：

- 每个系统或者应用都有它自己定位，并且都是建立上层之上设计 其他问题先不管。

	6月1号 ContextOS 实现产品化，基于应用平台，
	开发各种场景化的小程序。
	云社区，对标58同城、美团等
	云教育，对标作业帮等
	云办公，对标钉钉、飞书等
	云研发，对标微软、谷歌等

其他功能 在 

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407163224.png)



## 3. 本地环境安装（6分钟，照做就行）


支持平台：下面是我的个人环境搭建
- mac
```shell
//安装ContextOS，每个项目都是独立的ContextOS

cd /Users/wangchuanyi/code //我的工作目录

git clone https://2025.shylinux.com/x/20250205-web 
//任何项目项目都是独立的ContextOS。
cd 20250205-web
sudo 
source etc/miss.sh  //
./bin/ice.bin    //我使用个人笔记本 不需要常驻执行，也不需要安装集群。
open http://localhost:9020/?debug=true



```


**云主机Centos平台 2c2g跨节点访问**
``` shell
//Centos平台 2c2g跨节点访问

git clone https://shylinux.com/x/ContextOS   //root//root/ContextOS
cd ContextOS && source etc/miss.sh  

./bin/ice.bin 

// 远程登陆的，需要授权
cd /root/ContextOS
./bin/ice.bin admin space login ce352f62e2473ca1bbb561accc878ce3
登录地址：
http://101.43.226.54:9020/

open http://localhost:9020

小提示：这个ip 是云主机内部的，需要改为对外访问的

防火墙端口 [9020](http://10.0.8.8:9020)开放




```


从商店下载 云研发代码

- 20240903-operation 点击安装


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407145019.png)


- 从我的应用查看下载后项目
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407144821.png)


运行后的结果 

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407172427.png)


##  4. 了解后端 冰山架 volcano API


#### 4.1 从熟悉APi 开始

回想一下过去用什么方法学习的
- 假如是java开发人员，首先参考java sdk https://docs.oracle.com/javase/8/docs/api/
- 假如是c++开发人员，首先学习c++标注看开始的 参考的https://en.cppreference.com/w/ 或者https://cplusplus.com/
- 假如是Rust开发人员，参考go sdk   https://doc.rust-lang.org/std/index.html
- 假如golang开发人员 go sdk  参考 https://go.dev/doc/
- 假如 Linux服务器后端开发人员，阅读 https://linux-kernel-labs.github.io/refs/heads/master/labs/kernel_api.html#linux-kernel-api
- https://man7.org/linux/man-pages/man2/read.2.html 
- 然后升级到框架 web 开发java  spring python _Flask_ 


 同理：ContextOS
-   https://web.shylinux.com/c/portal/development/#frame/icebergs.shy
- 源码位置：usr/icebergs




```go


package main

import (
	"fmt" 
	_ "shylinux.com/x/operation/src/dashboard"
	_ "shylinux.com/x/operation/src/development"
	_ "shylinux.com/x/operation/src/operation"
	_ "shylinux.com/x/operation/src/production"
	_ "shylinux.com/x/operation/src/storage"
	"shylinux.com/x/ice"
)

func main() {
	fmt.Println(ice.Run())  
	//负责解析参数、初始化框架、启动服务
	//启动后编译这里看不到界面
}
// golang init函数：初始化顺序：全局变量初始化->init()->main()
//这个也是入口
func init() {
	ice.Info.CodeMain = "src/operation/portal.go"
	ice.Info.NodeMain = "web.team.operation.portal"
}

```



#### 疑问：main 和 init  哪个是入口？

目的：模块化方式加载到操作系统上，

 > 类比 liunx 内核模块加载，运行时加重设计？这个道理估计体会很长时间。

- ice.Run=src/operation/portal.go  【一个模块理解一个单独的服务。】
- 如果没有init 会有什么后果？【这个不不清楚，为此需要分析】✅
- ice.Info.CodeMain 是 ice.info 结构体中的一个字段，用于指定项目的主要代码文件路径 
- 1. ice.Info.CodeMain 是 ice.info 结构体中的一个字段，用于指定项目的主要代码文件路径。

1. 当 ice.Run() 函数被调用时，框架会：

- 初始化框架环境和配置

- 根据 ice.Info.CodeMain 指定的路径加载主代码文件

- 根据 ice.Info.NodeMain 找到对应的入口模块

- 启动应用服务
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407180840.png)




## day1

http://101.43.226.54:9020/?debug=true#project:studio:dream:tabview






## 会议预约

https://web.shylinux.com/s/20240903-operation/c/web.team.production.portal?debug=true#7d3880d3f70acec5337d5f832ef71103:web.team.production.date:



![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250513193201.png)







<iframe src="//player.bilibili.com/player.html?isOutside=true&aid=947706170&bvid=BV1TW4y1372y&cid=1001200489&p=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"></iframe>

<iframe src="//player.bilibili.com/player.html?isOutside=true&aid=947706170&bvid=BV1TW4y1372y&cid=1001200489&p=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"></iframe>






