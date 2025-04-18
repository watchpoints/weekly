---
title: 深入浅出理解DeepSeek 3FS (3) 步步引导轻松理解内存管理，面试必看
date: 2025-03-31
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---
**unix socket**
每个程序员都应该知道的延迟数字 （2020 年版本）

> 第一次提出在  Google I/O 2008 - Building Scalable Web Apps with App Engine

伯克利大学有个动态网页，可以查看每年各个操作耗时的变化
https://colin-scott.github.io/personal_website/research/interactive_latency.html


 ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250409105219.png)

下面是我解读

 机械硬盘HDD
- 英文：Read 1 MB sequentially from disk 30,000,000 ns 
- 翻译：从HDD 顺序读取1M数据 要<font color="#ff0000">30ms</font>
- 直观理解：带宽 <font color="#ff0000">34.8 MB/s</font>
- 假如你性能优化超过34.8 MB/s 就是<font color="#92d050">虚假</font>

固态盘SSD
- 英文：Read 1,000,000 bytes(1MB) sequentially from SSD 49,000ns
- 翻译：从 SSD 顺序读取 1 MB, 0.049 ms
- 直观理解：从 SSD 顺序读取 1 MB 数据的速度约为 20.41 GB/s
- 实测结果：顺序读取速度可达 7 GB/s。
- 思考：集群3fs 每秒6.6 TiB/s 的聚合读怎么做的？Infiniband交换机支持这样带宽吗？



网络传输
- 网卡：千兆网卡（家用）传输速率是1Gbps， 和万兆网卡（企业）传输速率是1Gbps， InfiniBand网卡（高性能）  200Gbps
- Send 2K bytes over 1 Gbps network 20,000 ns.
-  通过千兆网卡发送2K字节数据代价是（0.02ms）
-  ❌假如你视频传输优化到毫米以下，这个骗子❌，rmtp 默认1-3秒
- 发送2K字节数据所需的每秒带宽是 819Mbps。
-  直观理解： 假设一次<font color="#ff0000">RPC请求</font>的平均大小为1KB（1024字节），QPS大约为<font color="#ff0000">10万请求/秒</font>
- 反问：MySQL 数据库单点能支撑 1000 QPS， Redis 单点能支撑 10万 ,QPS双11阿里云公布的2020年双11订单创建峰值是58.3万笔/秒 ，百万级应该是读操作。隐藏什么？

内存
- 英文 Read 1 MB sequentially from memory 250,000 ns
- 直观理解：内存顺序读取1MB数据的带宽是4GB/s（千兆网卡不行了）
- 在4GB/s的带宽下，如果每个RPC请求的平均大小为1KB，那么每秒可以处理 4百万个RPC请求（实际情况 不会把带宽打满）
- 画外音：普通qps 万/秒 优化很不错了！！！


跨国家
- 从美国加州从发一个Packet（网络IP层的Packet包）到荷兰再回到加州的往返时间为150000000ns（150ms）



### 小结

- 写的代价一般是读的40倍
- 全局共享数据是非常昂贵的，这一般是分布式系统的基本单元的瓶颈，它是性能杀手，它让事务变成串行和缓慢
- 为可扩展的写行为作架构
- 优化比较缓慢的写行为
- 优化的更加广泛些，就是尽可能让写行为并行化



https://cedricchee.com/blog/latency/

https://github.com/sirupsen/napkin-math
https://norvig.com/21-days.html#answers
https://zhuanlan.zhihu.com/p/296808047
https://gist.github.com/jboner/2841832

Slides for QCon 2019 Beijin
https://github.com/QConChina/QConBeijing2019

QCon上海2020全球开发者大会PPT合集
https://github.com/TiScrip/QCon2020-shanghai
https://qcon.infoq.cn/2018/beijing/schedule



## 链接我 


如果对上面提到c++学习路径 推荐书籍感兴趣 

关注公共号：后端开发成长指南  回复电子书 

如果更进一步交流 添加 微信：wang_cyi


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


我是小王同学，

希望帮你深入理解分布式存储系统3FS更进一步 ，
为了更容易理解设计背后原理，这里从一个真实面试场故事开始的。

### 阅读对象（也是我正在做事情）

#### **1. 目标：冲击大厂，拿百万年薪**

- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。



    
#### **2. 现状：缺乏实战，渴望提升动手能力**

- 公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
    
- 想通过阅读优秀的源码，提高代码能力，从“不会写”到“敢写”，提升C++编程自信。
    
- 需要掌握高效学习和实践的方法，弥补缺乏实战经验的短板。

####  3. 价值：成为优秀完成任务，成为团队、公司都认可的核心骨干。

**优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量**

 **错误示范**：
- 不少同学工作很忙，天天加班，做了很多公司的事情。
  但是 不是本团队事情，不是本部门事情，领导不认可，绩效不高
- 做低优先级的任务，无法利他，绩效不高
- 招进来最后变成了随时被裁掉的一些征兆
1.  刻意提高工作难度
2. 工作中不公平对待
3. 制造恶性竞争
4. 捧杀



