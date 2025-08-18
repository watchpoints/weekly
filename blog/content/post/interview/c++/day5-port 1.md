---
title: 大厂面试题：如果判断一个服务是否可用
date: 2025-07-26
description: do book
draft: false
tags: 
categories:
  - do book
---
https://yuanbao.tencent.com/chat/naQivTmsDa/7cef0fae-3ef3-4507-93db-bf214bd32602


# 腾讯一面:生产环境如何使用gdb调试Release发行版本的程序？



大厂<font color="#245bdb">基础知识</font>面试拆解第10篇文章 

**历史文章：**

| 序号  | 知识地图   | 题目                                                                                 |
| --- | ------ | ---------------------------------------------------------------------------------- |
| 1   | 网络     | [判断一个端口是否可用](https://mp.weixin.qq.com/s/XNoNfOkbesF8vXSqHj_XcA)                    |
| 2   | 网络     | [腾讯一面：epoll不支持文件IO，kqueue怎么做到的](https://mp.weixin.qq.com/s/f5IuDt89mP81ssnRKFN_Rg) |
| 3   | 并发     | [Redis 单线程为什这么快](https://mp.weixin.qq.com/s/VLyQjVdJLmPmRkZS0VZ90g)                |
| 4   | 数据结构   | [什么是跳表（Skip List）](https://mp.weixin.qq.com/s/wXDT8BItZwMY0LVrHmphvQ)              |
| 5   | 分布式数据库 | [如何把一个文件系统目录树存储到数据库](https://mp.weixin.qq.com/s/_S8eJ0KxtT5P0JhvSUJNlA)            |
| 6   | 并发     | [从一道面试题谈linux下fork的运行机制](https://mp.weixin.qq.com/s/dsLFxyL-0IvxS0_oWy0upA)        |
| 7   | 并发     | [close-on-exec机制](https://mp.weixin.qq.com/s/B7BbwTdfbojOXaGZvdy79Q)               |
| 8   | 数据结构   | [LRU缓存](https://mp.weixin.qq.com/s/e1h5LURY5l7fNlGvcp925Q)                         |
| 9   | 分布式数据库 | [什是线性一致性](https://mp.weixin.qq.com/s/mKdaPMEQEI7UxCSnA20e3g)                       |



## 面试官：

生产环境如何使用gdb调试发行Release版本程序？

提示：
- <font color="#245bdb">先别急着直接回答问题,先提问在回答</font>

- 看看青铜(工作0-3年)到王者（工作5-10年)怎么提问的。


## 青铜（0-3年经验) 


![](https://s2.loli.net/2025/08/12/wCrJAoa38xVHTqP.png)

### 提问1：哪个服务core了
- 一台物理机部署很多服务，我如何区分Coredump属于哪个服务？

- 只有区分哪个服务才能执行 gdb ./my_program ./core.1234

- **`/proc/sys/kernel/core_pattern`**：指定核心转储文件的路径和命名规则
-  **刚罐装的操作系统**  配置 可能指定 核心转储文件的路径 但是 **命名规则** 没有
- core生成的目录：/var/log/下，名为core.xxxx文件 


### 提问1 回答：判断服务ceph-mds否debug版本


首先进入/var/log目录下通过file命令查看是哪个程序出的core

#### file <core文件名>

​示例输出​**​：

```
/var/core/my_program.core: 
ELF 64-bit LSB core file x86-64, 
version 1 (SYSV), SVR4-style, 
from '/usr/bin/osd-xxxx'
```

​**​来源程序​**​：`from '/usr/bin/osd-xxxxm'` 
显示生成 Core 文件的进程名称（即崩溃的程序名）。

- gdb /usr/bin/ceph-mds  core .370020 使用gdb进行调试
#### 提示： file usr/bin/ceph-mds


​**​关键字段​**​：`not stripped` 包含调试符号和调试信息。

**关键字段​**​：`stripped`表示符号表和调试信息已**被移除。**


**使用 `strip`剥离符号**

 剥离符号 strip hello 
 
 //去掉符号表、调试节区，只保留运行所需部分


 再次查看文件信息 file hello
 
`hello: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=..., stripped`


| **状态​**​             | ​**​用途​**​                      | ​**​ |
| -------------------- | ------------------------------- | ---- |
| ​**​stripped​**​     | 生产环境部署，减小文件体积（约 20%-50%），提升运行效率 |      |
| ​**​not stripped​**​ | 开发/测试环境调试，保留符号信息（如函数名、变量名）      |      |
|                      |                                 |      |


### 提问2 ：请管理员安装ceph-mds debug包，不然我无法定位

- 生产的 Release版本 开启 `-O2`/-O3 优化， 内联、循环展开、死代码消除、寄存器分配等全面启用
- 运行速度通常比 Debug 版本快 **3~50 倍**
- Release版本的程序 我根本无法定位core的原因？
- **请管理员给我安装 debug包**


#### 提问2回答：

- ceph 为例子 cmake工程 已经debug信息打包rpmp包
- 直接rpm -ivh 安装xxxx即可
- 例如 gdb 调试时候 自动加载同步下 usr/lib/debug/path/to/ceph-mds.debug

 也可以手工加载 


```c
# 启动 GDB 并加载符号
gdb myapp_release
(gdb) symbol-file myapp_release.debug  # 手动加载符号
(gdb) set debug-file-directory /path/to/symbols  # 设置符号搜索路径
```
## 黄金（3-5 年）

### 提问3：GDB调试代码 bt显示问号？？


我完全同意上面做法，

但是不能在生产环境执行，安装debug符号 ，
<font color="#245bdb">会导致整个服务响应 降低5-10倍</font>，

Ceph: A Journey to 1 TiB/s 

增加通过调整cmake 编译方式提升性能。

- 生产环境的core 迁移到 测试环境来定位

- 但是 为什么 gdb  查看全部 ？？？怎么办？
-  debug安装包 已经安装成功。


### 回答3

#### cpu类型

- 是不是cpu类型有关？debug版本对应cpu类型的

```shell

lscpu | grep 'Model name'
Model name:  Intel(R) Xeon(R) Platinum 8255C CPU @ 2.50GHz

Model name:  Hygon C86 7285

```

1. **海光（Hygon）CPU**

- 海光是中国的国产CPU品牌，基于AMD Zen架构设计（其实是AMD的授权和定制版本）。
 ![](https://s2.loli.net/2025/08/12/SBbA3tpfyKFomhJ.png)

 2. **Intel CPU**

![](https://s2.loli.net/2025/08/12/jrwORX9WaSzBlEV.png)


#### 栈破坏 汇编跟踪
- 不装逼 
- 面试时候不说因为我不懂。

### 提问4： gdb bt堆栈打印出来 ，还是找不要出原因怎么办？

- 查看寄存器状态 (gdb) info registers
- x/10x &bind_var
- watch *0x7ffc12345678
- 这命令全部了还是看不懂


### 回答4：

- 看代码，核心代码不懂，确实看不懂


## 王者(5-10)：

### 提问5：

- 我同意上面做法，没有gdb 调试mysql redis等案例
- gdb 汇编 没有这么多经验怎么处理？
- 我确实无法解决


### 回答5：从被动解决到主动预防
- 混沌工程（Chaos Engineering）
- 混沌工程的概念很早之前就有，但是直到 2012 年，
- 为了更好地推广混沌工程，Netflix 引入了一只猴子（Chaos Monkey），试想一只猴子在系统里面，平时是安安静静的，什么事情都不做，突然一天发疯开始在系统里到处捣乱，作为工程师，要干的一件事情就是逮住这只猴子，让它别捣乱，这大概就是混沌工程要表达的意思。
####  Chaos Mesh + SkyWalking，打造可观测的混沌工程


![](https://tidb-blog.oss-cn-beijing.aliyuncs.com/media/architecture-76301820de324f79d79db310b11b9246-1677983470329.png)

 通过Chaos-Mesh打造更稳定TiDB数据库高可用架构

Chaos Mesh 作为业内领先的混沌测试平台，具备以下核心优势：

- 核心能力稳固：Chaos Mesh 起源于 [TiDB](https://github.com/pingcap/tidb) 的核心测试平台，发布初期即继承了大量 TiDB 已有的测试经验。
    
- 被充分验证：Chaos Mesh 被众多公司以及组织所使用，例如腾讯和美团等；同时被用于众多知名分布式系统的测试体系中，例如 Apache APISIX 和 RabbitMQ 等。
    
- 系统易用性强：图形化操作和基于 Kubernetes 的使用方式，充分利用了自动化能力。
    
- 云原生：Chaos Mesh 原生支持 Kubernetes 环境，提供了强悍的自动化能力。
    
- 丰富的故障模拟场景：Chaos Mesh 几乎涵盖了分布式测试体系中基础故障模拟的绝大多数场景。
    
- 灵活的实验编排能力：用户可以通过平台设计自己的混沌实验场景，场景可包含多个混沌实验编排，以及应用状态检查等。
    
- 安全性高：Chaos Mesh 具有多层次安全控制设计，提供高安全性。
    
- 活跃的社区：Chaos Mesh 为全球知名开源混沌测试平台，CNCF 开源基金会孵化项目。
    
- 强大的扩展能力：Chaos Mesh 为故障测试类型扩展和功能扩展提供了充分的扩展能力。


## 最动人的作品，为自己而写，刚刚好打动别人



 1️⃣ 如果有更多疑问，联系小王，一起交流，进步

![个人联系方式](https://s2.loli.net/2025/08/13/veChAocQwJONWKE.png)

2️⃣  关注公众号:后端开发成长指南(回复"面经"获取）
    获取过去我**全部面试录音**和面试复盘。
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)


#### 抬头看天：走暗路、耕瘦田、进窄门、见微光
- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。

#### 低头走路：一次专注做好一个小事
- 不扫一屋 何以扫天下。
- 让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。
