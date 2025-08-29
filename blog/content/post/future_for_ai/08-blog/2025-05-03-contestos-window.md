---
title: ContextOS 模块开发与架构解析笔记
date: 2025-05-03
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




## 环境说明

 ###  window 本地 负责代码阅读

- trea工具 ssh不支持，因此改为window阅读代码

```
云研发小程序 代码地址  https://git.shylinux.com/web/operation

云研发小程序 演示地址 ：https://web.shylinux.com/s/20240903-operation/c/web.team.production.portal

https://web.shylinux.com/
```
### 云主机centos 2c2g环境代码运行


```shell
// 远程登陆的，需要授权
cd /root/ContextOS
./bin/ice.bin admin space login d72259f59b5fc390a71cb4be4aa953bb

登录地址：
http://101.43.226.54:9020/
```

### mac环境

```
/Users/wangchuanyi/code/20250205-web

./bin/ice.bin

http://localhost:9020?debug=true

/Users/wangchuanyi/code/20250205-web/usr/local/work/20240903-operation

```

启动命令：

./bin/ice.bin forever start port 9030 dev https://localhost:9020 nodename demo-test

exit


## 代码阅读

### 第一天

#### ✅ 1 了解云研发项目文档 main.sy

##### 1.1 类比方式解释 什么是 main.sy文档

- 类比:
Jupyter Notebook是指一种基于网页的交互式计算环境
它允许用户在浏览器中编写和运行代码，同时支持多种编程语言，如Python、R和Julia等。
它不仅能够执行代码，还能展示代码的运行结果，包括文本、图表和可视化内容，非常适合数据科学、机器学习和数据分析等领域的开发工作
- 类比: 画布 提供很多功能

- 个人理解：
在sy文档不仅可以写文字，图片，视频 还可以运行代码，但是加载程序中，一般不要很大


##### 1.2 使用方法

1. 这个不是Markdown语法 ，但是基本概念类似https://markdown.com.cn/intro.html
2. shy 文件的解析不是来自 Go 标准库，而是由 ice 框架实现的自定义解析器，web.wiki.word 是 Contexts 用来管理笔记与文档的工具，Contexts 的文档以 .shy 作为文件格式的后缀
3. 官方文档说明：https://shylinux.com/c/portal/started/#studio/note.shy

   用法：command "参数" [args] `多行内容`

- 标题:  文本类   title "云研发 
- 列表: 数据类， refer "a b"
 - field web.code.mysql.client   【引入mysql 配置模块 遗留任务：没有找到对应的代码】
- field web.code.mysql.query args `mysql operation` 【参数指定了要连接的数据库名为 "operation"】 

####  遗留任务  field 对应那个代码？？



#### ✅ 了解云研发项目文档 main.go


扩展阅读 方便解释一些概念
-  Beego 是一个开源的 Golang Web 框架，以卓越的性能和遵循 MVC（模型-视图-控制器）设计模式为特点。它支持开发 Web 应用程序、API 和后端服务，并内置了路由、ORM、日志记录和 HTTP 处理工具。
- Gin 是一个用 Go (Golang) 编写的 HTTP Web 框架。它具有类似 Martini 的 API，具有更好的性能——快 40 倍。如果您需要出色的性能。

```go

func main() { print(ice.Run()) }
func init() {

	ice.Info.CodeMain = "src/production/portal.go"
	
	ice.Info.NodeMain = "web.team.production.portal"

}

```


小提示：
1. 在 main 直接运行 结果如下 
![这个不是小程序界面入口](https://s2.loli.net/2025/05/25/unTLYVtJ9IocHAw.png)
### 第二天

- [ ]  看 视频：ContextOS 模块开发与架构解析笔记  https://meeting.tencent.com/cw/NLkkokOnf0


冰山框架（ice)--- Volcanos 是一个前端框架
https://shylinux.com/c/portal/development/#frame/volcanos.shy

没看懂

## 第三天 云教育

表格类型：




