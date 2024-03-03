---
title: "Rust For Linux02"
date: 2023-12-06
description: ""
draft: false
tags: ["Rust"]
---





本文主要描述 第六章：文件系统

### 参考资料

- https://github.com/LearningOS/2023a-rcore-watchpoints
- https://rcore-os.cn/rCore-Tutorial-Book-v3/chapter6/0intro.html#id2

- https://rcore-os.cn/rCore-Tutorial-Book-v3/chapter6/index.html
- [ rcore-tutorial-book-v3.pdf](https://cloud.tsinghua.edu.cn/d/51b02806997d459783e0/files/?p=%2F相关资料%2Frcore-tutorial-book-v3.pdf)
- https://github.com/learningos
- https://os.phil-opp.com/



## 

![img](https://rcore-os.cn/rCore-Tutorial-Book-v3/_images/fsos-fsdisk.png)

![easy-fs 的磁盘布局如下图所示：](https://rcore-os.cn/rCore-Tutorial-Book-v3/_images/%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F%E5%B8%83%E5%B1%80.png)

## 本章代码导读

本章涉及的代码量相对较多，且与进程执行相关的管理还有直接的关系。其实我们是参考经典的UNIX基于索引结构的文件系统，设计了一个简化的有一级目录并支持 `open，read， write， close` ，即创建/打开/读写/关闭文件一系列操作的文件系统。这里简要介绍一下在内核中添加文件系统的大致开发过程。





### ch6 源码分析

- Build & Run 构建并运行

~~~
git checkout ch6
cd os
make run
~~~

















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







