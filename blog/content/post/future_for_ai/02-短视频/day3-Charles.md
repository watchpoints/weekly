---
title: Charles入门教程
date: 2025-08-21
description: c++
draft: false
tags:
  - 小而美
categories:
  - do book
---

# 一、需求分析 


为了防止 自己沉迷手机 ，在手机看视频，哪怕正能量的视频，自己 专业技术上视频也不行

我使用手机看 视频 原则是 下载视频，在电脑，或者 手机离线观看

| 平台  | 内容         | 解决 办法     | 原理      |
| --- | ---------- | --------- | ------- |
| 有书  | 直播回放，有网址   | cat-catch |         |
| 有书  | 录制课程，无分享地址 | 手工录屏      | http 请求 |
| 视频号 | 直播         | 无法解决      |         |




#  二、解决办法

## 2.1 charles手机APP抓包

###  1. 准备

- 安装 JDK
- 下载 Charles 官方地址 
   The latest version of Charles is 5.0.2.
   https://www.charlesproxy.com/latest-release/download.do

- 关闭 VPN/代理

### 2. 设置 Charles抓包 http请求


安装好 Charles 后，
在菜单栏勾选『Proxy -> macOS Proxy』，
macOS 系统 HTTP/HTTPS 代理将会被自动设置为本地代理，默认端口 8888

或者

**Proxy --> Proxy Settings** 设置监听端口号为8888，勾选，Enable transparent HTTP proxying


### 3.设置 Charles抓包 https请求




#### macos

需要开启SSL代理。

首先安装证书，位置：
菜单栏->Help->SSL Proxying->Install Charles Root Certificate
会自动导入 Charles Proxy CA 证书并打开 Keychain Access，


双击新导入的证书弹出证书信息页面，将 Secure Sockets Layer(SSL) 设置为 Always Trust，

关闭页面后弹出密码提示，输入密码更新系统信任设置。


In Charles go to the Help menu and choose "SSL Proxying > Install Charles Root Certificate". Keychain Access will open. Find the "Charles Proxy..." entry, and double-click to get info on it. Expand the "Trust" section, and beside "When using this certificate" change it from "Use System Defaults" to "Always Trust". Then close the certificate info window, and you will be prompted for your Administrator password to update the system trust settings.

官方资料 ：https://www.charlesproxy.com/documentation/using-charles/ssl-certificates/
如果出现：安装证书提示不能修改“System Roots”钥匙串
![报错直接安装到​**​系统钥匙串**](https://s2.loli.net/2025/08/21/ABsNd3Xn8DgFqTm.png)

删除重新安装


- 直接请求[https://chls.pro/ssl](https://link.zhihu.com/?target=https%3A//chls.pro/ssl)进行下载
- Help->SSL Proxying->Save Charles Root Certificate

保存好证书，打开钥匙串访问，直接拖进去即可。之后找到刚安装的证书，设置为始终信任。

- 配置代理


在菜单栏选择 『Proxy -> SSL Proxy Settings...』，在 SSL Proxying 选项卡中可以添加需要抓包的域名端口。

这里添加为*:443，所有域名都进行抓取。

![](https://pic3.zhimg.com/v2-90d4611c78d47a92b9ed3a947ed8ec22_1440w.jpg)


### 4. app 设置

- 查看本地 ip：192.168.3.56 Mac 局域网地址可以在 Charles 中从菜单栏『Help -> Local IP Address』获取。
- 设置 iOS HTTP 代理，打开 iOS 设备对应 WiFi 设置，添加代理 IP 地址（Mac 的局域网地址）和端口号（8888）。


# 通用产品


## 参考
- https://zhuanlan.zhihu.com/p/657110843

## 最动人的作品，为自己而写，刚刚好打动别人



 1️⃣ 如果有更多疑问，联系小王，一起交流，进步

![个人联系方式](https://s2.loli.net/2025/08/13/veChAocQwJONWKE.png)

2️⃣  关注公众号:后端开发成长指南(回复"面经"获取）获取过去我**全部面试录音**和面试复盘。
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)


#### 抬头看天：走暗路、耕瘦田、进窄门、见微光
- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。

#### 低头走路：一次专注做好一个小事
- 不扫一屋 何以扫天下。
- 让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。