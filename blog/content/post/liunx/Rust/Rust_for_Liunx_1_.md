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







# 2023-01-06



录制: 第三阶段项目方向选题会
日期: 2023-11-05 19:58:18
录制文件：https://meeting.tencent.com/v2/cloud-record/share?id=54cbe9a8-fa09-4ac4-82ce-5888129808e7&from=3

https://shimo.im/docs/m8AZM4Z6X9tzG7kb


相关资料
训练营第三阶段项目实习选题会ppt介绍（这里的幻灯片中的链接无法访问。）
训练营第三阶段项目方向选题会“项目六：基于协程异步机制的操作系统/驱动”介绍（这里的幻灯片中的链接是可以访问的。）

选题协商
请各位同学在这里填写自己的选题意向，以便大家相互了解进展并进行可能的合作。
任务一要求所有同学参与，后面的六个任务可以选择自己有兴趣的部分参与，可以多人合作。

任务一：Embassy文档翻译
Embassy Documentation：Embassy is a project to make async/await a first-class option for embedded development.
具体任务描述：
看教学视频：并发与处理器、操作系统和编程语言
学习“协程的实现(200行代码讲透RUST FUTURES)”，写学习笔记；
填写自己想翻译的“Embassy Documentation”章节。
把翻译结果上传到公开仓库，并提交访问链接；

任务二：开发和移植跨OS的异步驱动
具体任务描述：
学习“Async Rust vs RTOS showdown（中文版本）”，写学习笔记；
在qemu模拟的RISC-V平台，尝试移植
任务三：异步机制的硬件支持 - 用户态中断的QEMU和FPGA实现和改进
任务四：共享调度器
任务五：reL4微内枋操作系统的异步改进
任务六：异步内核模块
任务七：异步函数调用栈跟踪分析





## 任务一：Embassy文档翻译

### 看教学视频：[并发与处理器、操作系统和编程语言](https://www.xuetangx.com/learn/THU0809100czxt/THU0809100czxt/14294493/video/25500376)





### 200行代码讲透RUST FUTURES



目的：这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题 

探索释Rust中的Futures， 为什么他们被设计成这样，以及他们如何工作。



阅读资料：

 Rust 中写的关于并发编程的第四文章

- https://stevenbai.top/rust/futures_explained_in_200_lines_of_rust/
- https://cfsamson.github.io/book-exploring-async-basics/
- https://cfsamson.gitbook.io/green-threads-explained-in-200-lines-of-rust/
- https://cfsamsonbooks.gitbook.io/epoll-kqueue-iocp-explained/



线程为不写并发使用的

语法：thread::spawn 与spawn闭包

参数传递方式：同C++11 Lambda的变量捕获



## 三 Rust中的Futures

### 概述

1. Rust中并发性的高级介绍
2. 了解 Rust 在使用异步代码时能提供什么，不能提供什么
3. 了解为什么我们需要 Rust 的运行时库
4. 理解“leaf-future”和“non-leaf-future”的区别
5. 了解如何处理 CPU 密集型任务







> 注解：
>
> 什么是`Future`? `Future`是一些将在未来完成的操作。 Rust中的异步实现基于轮询,每个异步任务分成三个阶
>
> 1. 轮询阶段 执行器(executor
> 2. 等待阶段. 事件源(通常称为reactor)注册等待一个事件发生
> 3. . 唤醒阶段

回顾：IO模型

- 面试经典题目：IO多路复用——深入浅出理解select、poll、epoll

- 面试经典题目：高级IO模型之kqueue和epoll

五种IO模型

《*UNIX网络编程*》(第1卷)(套接口API第3版)第1版和第2络专家W. Richard Stevens博士独自编写。

```text
[1]blockingIO - 阻塞IO
[2]nonblockingIO - 非阻塞IO
[3]signaldrivenIO - 信号驱动IO
[4]asynchronousIO - 异步IO
[5]IOmultiplexing - IO多路复用
```

多路服用 事件完成通知 事件就绪通知

面试经典题目：高级IO模型之kqueue和epol

kqueue 不仅能够处理文件描述符事件，还可以用于各种其他通知，例如文件修改监视、信号、异步 I/O 事件 (AIO)、子进程状态更改监视和支持纳秒级分辨率的计时器，此外 kqueue 提供了一种方式除了内核提供的事件之外，还可以使用用户定义的事

l

产品：redis libevent

- [根据事件类型分配（Dispatch）给某个进程 / 线程*](https://www.zhihu.com/question/26943938)

> 疑问“

- 与`leaf-future`相比，这些Future本身并不代表I/O资源。 当我们对这些Future进行轮询时, 有可能会运行一段时间或者因为等待相关资源而让度给调度器,然后等待相关资源ready的时候唤醒自己.





- Rust 和其他语言的区别在于，在选择运行时时，您必须进行主动选择。大多数情况下，在其他语言中，你只会使用提供给你的那一种。

异步运行时可以分为两部分: 1. 执行器(The Executor) 2. reactor (The Reactor)

当 Rusts Futures 被设计出来的时候，有一个愿望，那就是将通知`Future`它可以做更多工作的工作与`Future`实际做工作分开。



异步运行时可以分为两部分: 1. 执行器(The Executor) 2. reactor (The Reactor)

当

1. [async-std](https://github.com/async-rs/async-std)
2. [Tokio](https://github.com/tokio-rs/tokio)



这就是Rust标准库所做的。 正如你所看到的，不包括异步I/O的定义,这些异步任务是如何被创建的,如何运行的。



https://github.com/async-rs/async-std

https://github.com/tokio-rs/tokio





