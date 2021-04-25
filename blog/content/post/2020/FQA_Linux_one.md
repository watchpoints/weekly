---
title: "FQA_Linux_one"
date: 2019-11-26
description: "一句话总结"
draft: false
categories: ["Linux"]
---

## 当你的才华撑不起你的野心时，就应该静下心来好好学习。

# 知识卡

![image.png](https://i.loli.net/2019/11/13/iBAcurKQygeSUTD.png)



##  问题1 epoll这样方式有什么缺点？

- 第一步： 同步与阻塞，异步和非阻塞的区别？
- 第二步：了解网络模型
- 检查：select/epoll 是阻塞（ Block ）还是非阻塞（ Unblock ），同步( (Sync) )还是异步 (Async) ?
- 第三步：epoll特点
- 第四步：为什么不用现有框架呢 ，自己写干什么



### 老王：阻塞和非阻塞区别是什么？

小王：阻塞和非阻塞 一般只是系统调用

区别是  资源不满足，read为例子

(1):阻塞调用的进程，放入文件资源阻塞队列，非阻塞则不会，离开返回

(2) 返回结果：阻塞返回时候，数据已经读取用户缓冲区，非阻塞 数据依然停留在内核缓冲区，需要下次读取。

 

![image.png](https://i.loli.net/2019/11/19/uOEQ73XjG1nFSmh.png)

![image.png](https://i.loli.net/2019/11/19/Iy86NpHQkvAimoc.png)

### 老王：同步和异步区别什么？



小王：

> 同步和异步 区别是请求进程不阻塞,同步可以理解为顺序按照IO步骤来执行，异步发起请求后，不需要等待数据从内核用户交互过程，继续处理下一个请求。等事件处理完成之后，异步方式通知。 
>
> 操作系统只有阻塞和非阻塞概念，同步和异步是调用进程概念。



![image.png](https://i.loli.net/2019/11/14/G1ytqCYJOuFXZ9S.png)

 ![img](https://user-gold-cdn.xitu.io/2018/9/5/165a723ae2b8fb2a?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)





## 问题2  write文件有那2个方式呀？

问题拆分

- 什么是文件系统
- 申请内存2个方式
- mmap是什么
- write过程



### 老王 写文件有那几个方式



小王： 看图



![image.png](https://i.loli.net/2019/11/26/rNjz6aeJqyPuodc.png)

### 老王：mmap优缺点



小王：

![image.png](https://i.loli.net/2019/11/26/tkaRUFsw9KS1lJQ.png)