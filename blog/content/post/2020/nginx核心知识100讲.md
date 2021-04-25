---
title: "nginx核心知识100讲"
date: 2020-04-29
description: "陶辉"
draft: false
tags: ["nginx"]
categories: ["中间件"]
---







## 下面重要概念记录

[nginx核心知识100讲课件@陶辉](http://www.taohui.pub/2018/12/27/nginx%e6%a0%b8%e5%bf%83%e7%9f%a5%e8%af%86100%e8%ae%b2%e8%af%be%e4%bb%b6/#comment-27516)

### FQA-1 如何不影响服务情况下升级

![image.png](https://i.loli.net/2020/04/29/69GrFClUmNZkpnw.png)



### FQA-2 为什么需要共享内存

![image.png](https://i.loli.net/2020/04/29/pubR5CXBImUZQaq.png)


### FQA-3 如何优化  
#### TCP消息的发送 10个步骤
![image.png](https://i.loli.net/2020/05/07/sz5Q9xabgncJr6K.png)
![image.png](https://i.loli.net/2020/05/07/nmTOUQWldefyktS.png)

> 了解下基础概念,然后看tcp/IP 详解，然后看看别人的分析。
- sk_stream_wait_memory — Wait for more memory for a socket
- TCP性能和发送接收窗口、Buffer的关系 不要设置socket的SO_SNDBUF和SO_RCVBUF
- MTU、MSS、tcp_write_queue发送队列、阻塞与非阻塞套接字、拥塞窗口、滑动窗口、Nagle算法
![image.png](https://i.loli.net/2020/05/07/VzZvpTULrgoyYFc.png)