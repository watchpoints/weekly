---
title: "ARTS_2020_weekly_19"
date: 2020-06-03 
description: "GTD践行第19周"
draft: false
tags: ["GTD"]
categories: ["ARTS"]
---



# GTD践行第19周（2020第19周） 



# 本周视图 




![作息不好](https://imgkr.cn-bj.ufileos.com/b4ba2c55-083c-4cb0-af29-9bf427cdddce.jpg)







# 一. **ARTS的初衷**



![image.png](https://i.loli.net/2020/01/20/Wes4hOCzgJjIvTa.png) 





| 周一 | 周二          | 周三 | 周四       | 周五 | 周六           | 周日      |
| ---- | ------------- | ---- | ---------- | ---- | -------------- | --------- |
|      | **Algorithm** |      | **Review** |      | **Review/Tip** | **Share** |



### 1. Algorithm

> **主要是为了编程训练和学习。**每周至少做一个 leetcode 的算法题（先从Easy开始，然后再Medium，最后才Hard）。进行编程训练，如果不训练你看再多的算法书，你依然不会做算法题，看完书后，你需要训练。。

输出：字符串题目

https://wangcy6.github.io/post/leetcode/leetcode_string/



### 2. Review 

> 主要是为了学习英文，如果你的英文不行，你基本上无缘技术高手。所以，需要你阅读并点评至少一篇英文技术文章，我个人最喜欢去的地方是[http://Medium.com](https://link.zhihu.com/?target=http%3A//Medium.com)

>累计阅读英文文章：2 篇, 输出理解0 篇

- 深入理解 malloc Understanding glibc malloc
https://sploitfun.wordpress.com/2015/02/10/understanding-glibc-malloc/ 
翻译：https://hanfeng.ink/post/understand_glibc_malloc/
翻译：http://eternalsakura13.com/2018/02/27/heap1/


- 设计数据密集型应用 - 中文翻译
  https://github.com/Vonng/ddia
  https://asktug.com/t/topic/33907


 - [ ]  阅读valgrind技术手册,累计耗时4小时。



1. 自己过去3个星期搜索搜索中，Valgrind 没有找到有用的信息，都是copy ，没有自己一点理解，因此你必须看官方文档来获取信息。
    很多实用技巧，从官方文档获取，不是各种网上 ！！！！！！！！！！！！！！！！！！
   https://www.valgrind.org/docs/manual/manual-core.html#manual-core.whatdoes
2. 相关设计原理 https://www.valgrind.org/docs/pubs.html

https://www.valgrind.org/docs/muehlenfeld2006.pdf
Fault Detection in Multi-Threaded C++ Server Applications
多线程C ++服务器应用程序中的故障检测

https://www.slideserve.com/niles/valgrind-a-framework-for-heavyweight-dynamic-binary-instrumentation


- 输出：

即使监控多线程的程序，valgrind本身执行依然是一个线程？
~~~
https://stackoverflow.com/questions/29365611/does-running-valgrind-slow-down-my-application
This is both because Valgrind is serialising the threads, and because the code runs so much slower than normal.

(gdb) info thread  
* 1 process 13943  vgModuleLocal_do_syscall_for_client_WRK () at m_syswrap/syscall-amd64-linux.S:147

~~~

### 3. Tip

> 主要是为了总结和归纳你在是常工作中所遇到的知识点。学习至少一个技术技巧。
>
> 你在工作中遇到的问题，踩过的坑，学习的点滴知识。




本周分享文章：

- [理解Redis 6 的多线程](https://www.toutiao.com/i6831404730573390348/)


本周阅读
1. https://stackoverflow.com/questions/733360/why-do-we-not-have-a-virtual-constructor-in-c
- To construct an object, a constructor needs the exact type of the object it is to create






### 4.  Share

>主要是为了建立你的影响力，能够输出价值观。分享一篇有观点和思考的技术文章。


#####  输出理解：



 累计阅读文章：5篇，输出理解2篇

##### 阅读文章：
- tcp
https://baozh.github.io/2015-12/stl-vector-problem/ Linux 多线程服务端编程 - 使用muduo网络库7.4
input out buffer。【每次都有新收获muduo库】

https://memetao.github.io/TcpBuffer/

https://dulishu.top/libevent-evbuffer// [buffer ]

- 性能优化
https://www.infoq.cn/article/bSNpBm7h4GgGj9uTHM3d

- rpc 
https://github.com/apache/incubator-brpc/blob/master/docs/cn/bthread.md


协程的这个特点使其适合写运行时间确定的IO服务器，典型如http server，在一些精心调试的场景中，可以达到非常高的吞吐。
但百度内大部分在线服务的运行时间并不确定，且很多检索由几十人合作完成，一个缓慢的函数会卡住所有的协程。

