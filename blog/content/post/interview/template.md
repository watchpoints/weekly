---
title: 面试必备之深入理解thread local
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
对标：https://mp.weixin.qq.com/s/qIYXDoIqILMJlRewHDkeaA
https://mp.weixin.qq.com/s/qIYXDoIqILMJlRewHDkeaA


点击<font color="#245bdb">蓝色</font>关注。

><font color="#c0504d">文末有福利，先到先得</font>



阅读本文你将获得以下收益：


知识地图：深入理解计算机系统---文件子系统

本文专注大厂面试拆解 第xxx 片文字，希望对你有帮助


一篇文章 
就是一个ppt，
一个短视频，
一个直播答疑 
一个付费课程咨询 
一个产品设计
一次有价值的沟通





## 一、面试回顾

时间：2025年 4月28 
岗位：数据库开发工程师
公司：vmare/阿里云/kangao数据库
形式：线下
面试：自己感觉回答很好，但是估计过不了

一面

基础面试：
1. 谈谈你对自旋锁理解
2. 进程通信那个方式
3. 线程局部存储
4. read,write io过程。

项目面试：
1. 干了这么多年， 你角色是什么，一个开发吗？
2. 假如入你独立开发c特性，能不能做？
3. 熟悉英语吗？开源社区 参与中文的还是英文的，英文资料行不行
4. 你学校主任是谁，
5. 在之前公司干什么事情？说的项目，结果 判断不是核心人员。（太武断了）
## 二、谈谈你对ThreadLocal理解

### 2.1 青铜被虐（工作0-5年）：

思考：
1. 听说过没接触过，不知道 怎么实现，然后陷入慌乱，之前线程，进程。
2. 根本想不起来基础知识   局部变量、全局变量、堆、堆栈、静态变量区别 和这个有关系

>划重点：
>1. c++ 不会凭空造一个新概念，都是基于原有基础上发展的
>2. c++ 特性都是依赖编译器，gcc，甚至操作系统。


### 2.2 王者归来（5-10年）

#### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题







####  二、 这个技术适用的场景。任何技术都有其适用的场景




#### 三、技术的组成部分和关键点




### 五、对比java怎么实现的？



## 最动人的作品，为自己而写，刚刚好打动别人

  

我在寻找一位积极上进的小伙伴，

一起参与**神奇早起 30 天改变人生计划**，做一个伟大产品取悦自己，不妨试试

  

 1️⃣关注公众号:后端开发成长指南(回复"面经"获取）获取过去我**全部面试录音**和面试复盘

![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)

2️⃣ **感兴趣的读者可以通过公众号获取老王的联系方式。**

  

 加入我的技术交流群Offer 来碗里 (回复“面经”获取），一起抱团取暖

![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)

本群目标是：
#### 抬头看天：走暗路、耕瘦田、进窄门、见微光

- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。


#### 低头走路：一次专注做好一个小事
- 不扫一屋 何以扫天下。
- 让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。



#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，别人投入时间完成任务，别人采取措施解决问题了，不要不贪别人功劳，
- 但是不要给自己这样假设：别人完成就等着自己完成了，大家一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现， 
- 别人不这么假设，至少kpi上不会写成自己的，至少晋升不是你，裁员淘汰是，你的整个公司ceo，整个部门总裁，整个领导不帮助一下的，他们不这么想 ，你什么没做，战略是别人10年一点带你研究的多难，项目拆分别人10年完成多少问题，项目实现10年安排组织一点点完成多少bug，多少代码，是不要给自己这样假设：你等了看了观察10年什么做 ，0 贡献，
-  但是不要给自己这样假设，别人全部市场，别人全部市场，别人占据全部客户，一切重要无比，你太差，太才，思考不行，沟通不行，认知不行，去tmd，给别人丢脸。这个方面我无法控制，在这方面经历任何问题应该的。
- 我控制 的事情是 我必须亲自了解行业遇到难题，了解有什么需求，行业解决方案，我可以从三万英尺看问题，像周围人学习，像免费公开英文资料学习，从模仿开始。然后免费公开。我要通过技术拿到百万年薪P7职务，我必须糊涂混沌中走出来
-  目标：拿百万年 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 现状：缺乏实战，渴望提升动手能力公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 成为优秀完成任务，成为团队、公司都认可的核心骨干。优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量

##### 低头走路：
 - 不扫一屋，何以扫天下
 - 让自己早睡早起，锻炼身体，保持个人卫生，多喝水，表达清楚，把基本的事情做好










