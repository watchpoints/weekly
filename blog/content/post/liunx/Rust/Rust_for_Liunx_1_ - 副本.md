---
title: "Rust For Linux"
date: 2023-11-06
description: ""
draft: false
tags: ["book"]
---









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





各位同学，今天(周二)晚上8点项目一上第一次课。介绍一下ArceOS Unikernel的相关概念、架构及关键组件构成。
大家如果有时间，可以先做如下准备：
1. Fork ArceOS的工程，clone到本地。工程链接如下
git@github.com:arceos-org/arceos.git

2. 把apps目录下的各个应用都运行一遍，具体办法见主页。
(display和C这两个子目录暂时不用管，不是我们学习的重点。)

3. 运行上述应用时，可以打开LOG=trace，跟踪一下每个应用具体整合了那些组件。

4. 项目一的实习基于Riscv64，所以大家在运行时，请加上ARCH=riscv64。
如果不指定，默认是x86_64，不同体系结构的运行机制会有一定差别，这样就会与我们后面的实验不符。

5. 指导老师的实验环境是WSL + Ubuntu22.04。大家可以通过这个准备验证一下自己的环境，我们正式的练习会在每次课后发



另，项目一学习资料汇总如下，请了解：
(1)  ArceOS组件化教程.ppt，见群内分享；
(2) 陈老师的一篇文章，包含了组件化OS-ArceOS的一些思想来源
https://github.com/chyyuu/thoughts/blob/main/tangram-oskits.md
(3)  Rcore tutorial的附录A中包含了一些很有价值的参考资料，Rust系统编程基础相关的，推荐学习参考
http://rcore-os.cn/rCore-Tutorial-Book-v3/appendix-a/index.html
(4) ArceOS代码库，手册还处于开发过程中：
https://github.com/arceos-org/arceos
(5)  Rcore OS的相关资料
https://github.com/orgs/rcore-os/discussions/categories/ideas
(6)  ArceOS开源项目的最新进展参考
https://github.com/orgs/rcore-os/discussions/categories/ideas





@所有人 下面是6个选题会的回放，感兴趣&没看过的同学可以看看，了解一下背景等。录制: 第三阶段项目方向选题会
日期: 2023-11-05 19:58:18
录制文件：https://meeting.tencent.com/v2/cloud-record/share?id=54cbe9a8-fa09-4ac4-82ce-5888129808e7&from=3

















