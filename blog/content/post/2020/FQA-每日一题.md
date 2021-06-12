---
title: "每日一题"
date: 2020-05-29
description: "每日一题"
draft: false
categories: ["FQA"]
---

# 更新日志

| 日期      | 更新内容     | 备注     |
| --------- | ------------ | -------- |
| 2020-5-1:2021-5-27 | FQ1 | 持续更新 |

>如何驱动学习？

>考试是对实践经验其中一个片段，进行复盘总结，如同黑客帝国中镜像学习一样反复练习 ，以始为终，去发现 
>
>在大棒加胡萝卜奖励驱动下，自己看不到的的错误。





# 每日一题



## 20201-5-27



## 面试官提问：

- 如何减少编译依赖？
- 在c++中利用类的前置声明，能减少编译依赖？但是如何检查 类的定义完整性，

后记：

 小白：在哪里卡住了，思考很长时间，知道别人不耐烦【他们不会对你提示的，根本不害怕担心他们嘲笑，这都不会，我认真分析我哪里不对，我哪里不对】

## 假如在使用过程中，根本无法访问这个类指针的成员和变量，只能指针本身

## 这个知识点根本无法应用呢？陷入死胡同



### 我的思考

1. 感觉这个问题无法解决，利用指针 而不是具体类，减少编译依赖，就是这样设计的 因此陷入死胡同。这根本无法检查，我知道sizeof 4

  我使用前置声明，无法访问具体的类和产生，这样做有什么意义，无法是哦你。这样设计，有啥用呢？你这个知识属于废弃知识。根本无法用。

陷阱：你思考一定没问题，你相信你自己，而是对预先声明基本用法和开发过程应用没动手操作过，让你陷入知识片面理解陷阱

这个用法没有问题，是你理解不到位。

- 尽可能地避免使用前置声明。使用 #include 包含需要的头文件即可。 谷歌代码规范也不提倡使用。
- 前置声明隐藏了依赖关系，头文件改动时，用户的代码会跳过必要的重新编译过程【雷】

1. PIMPL 问什么提倡

请仔细阅读： 。 PIMPL(Pointer to Implementation)

pImpl方法是微软的Herb Sutter提出来的，该方法是为了尽量减小接口和实现之间的耦合，

以避免接口改动对程序重新编译等带来的影响。

简单来说，如果你的大型程序因为复杂的头文件包含关系，使得你对某头文件的某小改动可能引起的巨大编译时间成本望而生畏，

那么你需要用pImpl方法来改善这种处境。

类CSample仅向外界暴露了最小的接口，真正的实现由其内部类CImpl来完成。

这样无论CImpl如何修改，外界对此一无所知，从而保持接口的不变性。

这种方式的另外一个好处是减少了头文件之间的依赖关系。

分类：数据与算法分离，接口与实现分离

# 编译防火墙——C++的Pimpl惯用法解析

https://leehao.blog.csdn.net/article/details/47610309

在类中使用 Pimpl 惯用法，具有如下优点：

- 降低耦合
- 信息隐藏
- 降低编译依赖，提高编译速度
- 接口与实现分离

1. 如果你动手写过代码，你发现 forward declaration根本无法使用，根本无法访问对应成员和函数。这有什么用呢？

遇到谷歌编程规范：https://blog.csdn.net/Hhrock/article/details/82386014

https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/headers/#forward-declarations

明白优缺点

- 尽可能地避免使用前置声明。使用 #include 包含需要的头文件即可。



1. 总结 c++中 编程规范前置声明 不提倡使用，但是 PIMPL(Pointer to Implementation)为什么提倡使用呢？这有什么区别

https://blog.csdn.net/caoshangpa/article/details/78590826

- classA的任何改动，对于classB来说都没有影响，除非classB修改了对classA调用的代码。在这里，指针起到了解耦合的作用。

- PIMPL Of course it is used. I use it in my project, in almost every class.

1. 使用范围是 自己类进一步拆分，抽象部分和实现部分。 c++中 编程规范前置声明 不提倡使用

[https://www.gamedev.net/tutorials/_/technical/general-programming/the-c-pimpl-r1794/](https://www.gamedev.net/tutorials/_/technical/general-programming/the-c-pimpl-r1794)

[https://www.gamedev.net/tutorials/_/technical/general-programming/the-c-pimpl-r1794/](https://www.gamedev.net/tutorials/_/technical/general-programming/the-c-pimpl-r1794)

https://leehao.blog.csdn.net/article/details/47610309

https://github.com/haozlee/pimpl/blob/master/public.h

https://stackoverflow.com/questions/8972588/is-the-pimpl-idiom-really-used-in-practice





### 我的回答：

- 针对类接口不变（这是电信行业头特点，对我api前期定下来，很少发生变化），类实现发生变化。一个类结构发生变化，引用相关头文件发生改变
- 变化成员封装成一个指针，这个指针做具体类的成员。然后使用中间层对我提供接口。 桥接模式。
- 这里b c d可以不使用指针

   c++中 编程规范前置声明 不提倡使用，但是 PIMPL(Pointer to Implementation)提倡使用呢？

~~~
//Pimpl是对自己类进一步拆分，只要保证自己模块没有问题，就解决MyClassImp依赖问题

//其他的类不需要关系，有没有发生变动
~~~

![pimpl 大小不变](https://img-blog.csdnimg.cn/2021010716135345.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpaGFvMjE=,size_16,color_FFFFFF,t_70#pic_center)

![](https://img-blog.csdn.net/20171121113924445)

这里是个例子 证明自己写过

https://www.gamedev.net/tutorials/_/technical/general-programming/the-c-pimpl-r1794/

 

- 在同一个文件里，单文件编译时候，获取完整定义

   class MyClass class MyClassImp;

### 面试官反问：

- 前置声明不足时什么，如何检查【使用类目的，就是使用成员和函数，在定义时候前置声明无法使用，这样编译不过的】
- pimpl不足时什么？如何预防
- 你了解设计模式吗？有哪些。
- https://zhuanlan.zhihu.com/p/106779428

Herb Sutter（C++标准委员会成员）写了一些相关的博客，对其博客做一个翻译记录： 

- [GotW #7a Solution: Minimizing Compile-Time Dependencies, Part 1](https://link.zhihu.com/?target=https%3A//herbsutter.com/2013/08/19/gotw-7a-solution-minimizing-compile-time-dependencies-part-1/)
- [GotW #7b Solution: Minimizing Compile-Time Dependencies, Part 2](https://link.zhihu.com/?target=https%3A//herbsutter.com/2013/12/31/gotw-7b-solution-minimizing-compile-time-dependencies-part-2/)
- [GotW #100: Compilation Firewalls](https://link.zhihu.com/?target=https%3A//herbsutter.com/gotw/_100/) 【本篇】

*compile-time dependencie*









### 5 
1000桶水，其中一桶有毒，猪喝毒水后会在15分钟内死去，
想用一个小时找到这桶毒水，至少需要几头猪？
https://www.zhihu.com/question/60227816/answer/1274071217

### 字节跳动面试官：[请你实现一个大文件上传和断点续传](https://juejin.im/post/5dff8a26e51d4558105420ed)
### 今天每日一题：Redis支持的数据类型？（快手）

###  假如Redis里面有1亿个key，其中有10w个key是以某个固定的已知的前缀开头的，如何将它们全部找出来？

使用keys指令可以扫出指定模式的key列表。
对方接着追问：如果这个redis正在给线上的业务提供服务，那使用keys指令会有什么问题？
这个时候你要回答redis关键的一个特性：
redis的单线程的。keys指令会导致线程阻塞一段时间，线上服务会停顿，直到指令执行完毕，服务才能恢复。
这个时候可以使用scan指令，scan指令可以无阻塞的提取出指定模式的key列表，但是会有一定的重复概率，在客户端做一次去重就可以了，
但是整体所花费的时间会比直接用keys指令长。

http://doc.redisfans.com/key/scan.html [游标]

### 第二题：主从数据库不一致如何解决?（群员面试题）

- 我的回答：

这个看似容易 不好回答。
~~~
全部一致（2pc）

不确信，主节点是否还能恢复,故障模型，采用失败，踢出集群。
主要解决问题是主从切换期间，新来数据该怎么办？不然引起全量同步。
需要主节点保存新增数据，等挂断主重启时候加载数据，然后同步。
因为全部一致，此时可能阻塞业务。

大部分一致（raft）

部分节点故障不影响业务正常执行，如何解决不一致情况
1 选择一个好的领导，这个很重要。
2. 日志匹配方式 ，修复从节点数据。没有特意去维护保存历史数据。

最终一致（redis）：
是无法保证数据一致性，有可能丢失数据。如果这样想，肯定不对，
别人问的是 不一直情况下解决参考redis方式 。
1 同步 
  全量同步，从节点获取历史RDB文件+BGSAVE期间产生新命令的缓冲数据 保持一致。
  说明：
       output buffer是Redis为client分配的缓冲区，
       若为某个客户端分配的output buffer超过了预留大小，Redis可能会根据配置策略关闭与该端的连接。

2  只要主做业务，从出现不一致情况。需要命令传播
   多个从节点共享【主节维护的大小固定复制挤压缓冲区] 解决断线问题。



~~~

- 参考答案：
- https://www.zhihu.com/question/357207584/answer/913693915

### 第一题 redis怎么保证高可用，高可用模式有那些?对比下优缺点?
FQA1：

我的回答：

redis高可用基础还是数据分片方式，如果没有槽点，任何高可用方案都毫无价值。
大数据拆分小数据。意思是说集群规模不能超过16384（不可以无限扩张）,
redis高可用依然采用主从模式，但是在主备切换等方面舍去中心代理，
依然想其他节点询问。数据同步方面：可以采用同步或者异步方式。redis采用gossip

raft在这方面做得步骤更加清晰。缺点也是，单个集群规模不能太大。类似2阶段提交
同步一个记录时间复杂度是o(n)


参考答案
~~~
参考答案如下：
Redis 高可用架构如下：
1.Redis Sentinel 集群 + 内网 DNS + 自定义脚本。
2.Redis Sentinel 集群 + VIP + 自定义脚本。
3.封装客户端直连 Redis Sentinel 端口。
4.JedisSentinelPool，适合 Java。
5.PHP 基于 phpredis 自行封装。
6.Redis Sentinel 集群 + Keepalived/Haproxy。
7.Redis M/S + Keepalived。
8.Redis Cluster。
9.Twemproxy。
10.Codis。


1.Redis Sentinel 集群 + 内网 DNS + 自定义脚本。
优点：
秒级切换；
脚本自定义，架构可控；
对应用透明。
缺点：
维护成本略高；
依赖 DNS，存在解析延时；
Sentinel 模式存在短时间的服务不可用。

2.Redis Sentinel 集群 + VIP + 自定义脚本。
优点：
秒级切换；
脚本自定义，架构可控；
对应用透明。
缺点：
维护成本略高；
Sentinel 模式存在短时间的服务不可用。

3.封装客户端直连 Redis Sentinel 端口。
优点：
服务探测故障及时；
DBA 维护成本低。
缺点：
依赖客户端支持 Sentinel；
Sentinel 服务器需要开放访问权限；
对应用有侵入性。

4.JedisSentinelPool，适合 Java。

5.PHP 基于 phpredis 自行封装。

6.Redis Sentinel 集群 + Keepalived/Haproxy。
优点：
秒级切换；
对应用透明。
缺点：
维护成本高；
存在脑裂；
Sentinel 模式存在短时间的服务不可用。

7.Redis M/S + Keepalived。
优点：
秒级切换；
对应用透明；
部署简单，维护成本低。
缺点：
需要脚本实现切换功能；
存在脑裂。

8.Redis Cluster。
优点：
组件 all-in-box，部署简单，节约机器资源；
性能比 proxy 模式好；
自动故障转移、Slot 迁移中数据可用；
官方原生集群方案，更新与支持有保障。
缺点：
架构比较新，最佳实践较少；
多键操作支持有限（驱动可以曲线救国）；
为了性能提升，客户端需要缓存路由表信息；
节点发现、reshard 操作不够自动化。

9.Twemproxy。
优点：
开发简单，对应用几乎透明；
历史悠久，方案成熟。
缺点：
代理影响性能；
LVS 和 Twemproxy 会有节点性能瓶颈；
Redis 扩容非常麻烦；
Twitter 内部已放弃使用该方案，新使用的架构未开源。

10.Codis。
优点：
开发简单，对应用几乎透明；
性能比 Twemproxy 好；
有图形化界面，扩容容易，运维方便。
缺点：
代理依旧影响性能；
组件过多，需要很多机器资源；
修改了 Redis 代码，导致和官方无法同步，新特性跟进缓慢；
开发团队准备主推基于 Redis 改造的 reborndb。
~~~









