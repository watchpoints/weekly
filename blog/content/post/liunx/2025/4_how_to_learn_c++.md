---
title: 当面试官问 C++ 20 新特性的时候，应该怎样回答？
date: 2025-04-06
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---



大家好，这是<font color="#00b050">大厂面试拆解</font>系列的第<font color="#953734">4</font>篇文章，
如果你面试 或者工作中遇到<font color="#245bdb">c++</font>相关，欢迎留言提问。

一天 小义同学和老王 在餐厅相遇

小义： 老王 ，我刚买的**杨枝甘露** 你尝尝味道如何，听说你一直研究c++
     <font color="#9bbb59">c++20</font>有什么新特性呀。为什么回答完毕每次面试官等着我继续回答，但是实在想不出来？

老王：我相信你说这些新特性，关键是 **为什么**要引入这些变化
      只有知道为了什么，用在什么地方，我们才能真正学到这个知识。

小义：公司已成立就使用c++开发项目了，这个项目运行10多年，我<font color="#c00000">每天忙碌业务</font>
      <font color="#e5b9b7">我也不知道</font>当初怎么设计的？
      
老王：不妨换个思路，想一下想

1. 你在项目开发中回遇到什么问题？
2. c++11之前有哪些方法来解决这些问题？
3. c++11之后有哪些方法解决这些问题？

<font color="#ff0000">如果 实在想不起来，c++之父 博客，视频，书籍 了解一下，</font>

小义：好的 ，<font color="#f79646">我马上去做</font> 

老王：慢慢的喝了一口, 有点甜，下次试试 **草莓冰淇淋**

>画外音：老王对验收标准 有了自己想法，不问就直接去做，鲁莽了。

c++使用场景就是大型项目，大型项目有什么特点

（1）代码多，自然编译慢
     ----c++必须在<font color="#d83931">编译期间 </font>开始进行优化的，怎么链接的 ，版本多少？

（2）用户多，数量大
    ---性能采用，函数调用采用同步，还是<font color="#c3d69b">异步（aio）</font>，采用进程，线程 还是协程，
     最后平均耗时多少，最大性能多少，
     至少大于 每个程序员都应该知道的数字 ，小于系统最大性能吧。不然着睁眼说瞎话。
     
 （3）模块多，开发人员水平参差不齐，采用易用代码，而不是难用的语法。
     ----采用通用的标准 无论什么，前面添加<font color="#31859b">模版</font>， 模版类，模版函数。看看怎么回答。
 
一个展现的面貌出现我们面前。
 c++语法是建立在编译器 标准库之上的 一个高级抽象。

![c++设计目的](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407224849.png)


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407224917.png)

![编译性能提高](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407225248.png)


<font color="#c00000">主要内容介绍完毕，下面是小义思考过程</font>

## 1. 项目中遇到问题

![实际问题](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407224803.png)

---------------------------------------------------


## 2. 开始 c++演化历史从 1979 年到 2020 年 

C++ 的 1979–2006
C++11：感觉像是门新语言
C++17：大海迷航
C++20：方向之争

| 年份          | 事件 / 标准版本             | 语言特性                                           | 库支持                                                                 |
| ----------- | --------------------- | ---------------------------------------------- | ------------------------------------------------------------------- |
| **1979**    | 开始开发 “C with Classes” | 类、构造函数/析构函数、public/private、简单继承、函数参数类型检查       | 任务（协程与仿真支持）、使用宏参数化的向量                                               |
| **1985**    | C++ 首次商业发布            | 虚函数、运算符重载、引用、const                             | 复数运算、流输入输出（stream I/O）                                              |
| **1989–91** | ANSI/ISO 标准化启动        | 抽象类、多重继承、异常、模板                                 | 输入输出流（iostreams），不包括任务库                                             |
| **1998**    | C++98，首个 ISO 标准       | 命名空间、命名类型转换、bool、dynamic_cast                  | STL（容器和算法）、string、bitset                                            |
| **2011**    | C++11                 | 内存模型、auto、范围 for 循环、constexpr、lambda、用户自定义字面量等 | 线程与锁、future、unique_ptr、shared_ptr、array、时间、随机数、无序容器等                |
| **2014**    | C++14                 | 泛型 lambda、constexpr 函数中的局部变量、数字分隔符等            | 用户自定义字面量等                                                           |
| **2017**    | C++17                 | 结构化绑定、变量模板、构造函数模板参数推导等                         | 文件系统、scoped_lock、shared_mutex、any、variant、optional、string_view、并行算法 |
| **2020**    | C++20                 | 概念（concepts）、模块、协程、三向比较、编译时计算支持等               | 概念、ranges、日期与时区、span、格式化、<font color="#c00000">改进的并发与并行支持</font>    |


C++ 作为一门博大精深的语言，其发展演化历程也堪称波澜壮阔。
由于教育的原 因，很多人对 C++ 还停留在 C++98 之前的版本。
殊不知 C++ 在经历从 2006 年之 后至今的 15 年“激流勇进式的发展”，
在很多人眼里已经近乎变为一个全新的语 言。
这 15 年间，国际 C++ 标准委员会发布的 4 个版本：C++11/14/17/20 也被统 称为“现代 C++”。

因为这一时期对 C++ 发展的里程碑作用，我将其称为“现代 C++ 的文艺复兴”。


设计c++目的是什么 

**作者回答这样的一个问题：
1. 如何直接操作硬件，
2. 同时又支持 高效、高级的抽象？


<font color="#92d050">> C++ 的核心精神“直接映射硬件”和“零开销抽象”正 是对这一“生态位”恰如其分的支撑。</font>


如图  c++满足用户能直接 表达需求。


![c++最终目标](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407214752.png)


![机器思维](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407214837.png)
![从机器思维变成抽象思维](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407214858.png)

![变化过程](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250407214931.png)

### 第一个十年

1981 年：const——支持接口和符号常量的不变性。 
• 1982 年：虚函数——提供运行期多态。 
• 1984 年：引用——支持运算符重载和简化参数传递。
• 1984 年：运算符和函数重载
• 1987 年：类型安全链接——消除许多来自不同翻译单元中不一致声明的 错误。
• 1987 年：抽象类——提供纯接口。
1980 年 模板——在经历了多年使用宏进行泛型编程的痛苦之后，更好地支持泛型 编程

### 第二个十年

该委员会终于发布了第一个标准：C++98
C++98 的主要语言特性是 
• 模板——、对泛型编程的编译期支持
• dynamic_cast 和 typeid——一种非常简单的运行期反射形式（“运行期 类型识别”，又名 RTTI）。 
• namespace——允许程序员在编写由几个独立部分组成的较大程序时避免 名称冲突。
• 条件语句内的声明——让写法更紧凑和限制变量作用域。
• 具名类型转换——（static_cast、reinterpret_cast 和const_cast）： 
• bool：一种被证明非常有用和流行的布尔类型；

### 第三个10年

2006年 c++进入低谷
- 大多数涉及 C++的专业软件杂志已经死亡，因为纸质出版物正在下降
- C++在教育中的作用正在急剧下降。C++不再是新的和有趣的语言，Java 作为一种更简单、更强大的语言直接向大学销售。计算机科学考试突然从 C++改为 Java。
- 大学一年级的学生编程。调查了大约 20 本最流行的 C++编程教科书，最后大声抱怨 如果这是 C++，我也不喜欢

原因很多 
- 不愿妥协 大多数委员会成员都有强烈的意见，但要在一个大型团体中达 成共识需要妥协
- 缺乏优先级：从技术的角度来看，所有问题都是平等的：不精确的规范就 是不精确规范
- 完美主义：一个标准预期会被几百万人用到，并且可以稳定数十年。人们 自然希望它是完美的
- 少数人的阻挠：共识流程可以防止某些类型的错误，尤其是防止多数人的 暴政。


回想起来，2006 年可能是 C++ 的最低谷，但重要的技术事件也恰好在此时发生 了：大约在 2005 年，历史上第一次单个处理器（单核）的性能停止提高，能效 （“每瓦特的性能”）成为一个关键指标（尤其是对于服务器集群和手持设备）。计 算经济学转而青睐更好的软件。

然后工业应用开花了
 - C++诞生的电信行业中，使用率很高
 - 它已经蔓延到游戏（例如，虚幻、PlayStation、Xbox 和道格拉斯亚当斯的《泰坦尼克号》）、金融（例如，Morgan Stanley 和 Renaissance Technologies）、微电子（例如，Intel 和 Mentor Graphics）、电影（例如，Pixar 和 Maya）、航空航天（例如，洛克希德-马丁公司和美国宇航局）
 - c++在科学和工程领域的广泛应用，比如高能物理（例如，CERN、SLAC、FermiLab）、生物学


### C++11：感觉像是门新语言

![C++11：感觉像是门新语言](https://s2.loli.net/2025/04/06/yri1sfuXCUhoAab.png)
### C++20：为方向而奋
![[Pasted image 20250406155749.png]]

![image.png](https://s2.loli.net/2025/04/06/FPlsk5OWMcNUnyo.png)
![image.png](https://s2.loli.net/2025/04/06/EvcW3g2a5J9DezQ.png)


三个十年（2010s - 2020s）

**关键词：现代化 + 并发支持 + 编译期能力**

### ✨ 设计目标：

- 适应现代软件的需求：**多核并发、泛型编程、元编程、模块化**；
    
- 简化语法，提升开发者生产力（如 `auto`, `range-for`, `lambda`）；
    
- 加强**编译期计算**能力，提高性能和安全性。
    

### 💡 核心特性：

- C++11：lambda、`auto`、智能指针、并发支持；
    
- C++14/17：泛型 lambda、结构化绑定、`optional/variant/any`；
    
- C++20：协程、概念（concepts）、模块、ranges、三向比较。
    
### 🎯 设计哲学：

> “**更少的代码，更高的表达力，不损失性能。**”  
> 现代 C++ 倾向将更多计算移至编译期，提升效率与可维护性



## 参考第一手资料

#### 1. 左耳朵耗子
1. https://coolshell.cn/haoel 很多面试官都是看上面题目来面试别人
#### 2. C++之父。Bjarne Stroustrup
- 作者：C++之父。Bjarne Stroustrup 现代C++的发展与演化
   标题：在拥挤和变化的世界中茁壮成长: C++ 2006-2020
    Thriving in a crowded and changing world: C++ 2006-2020  
    英文：https://www.stroustrup.com/hopl20main-p5-p-bfc9cd4--final.pdf
- C++ 之父最新蓝图：让代码更安全，还能兼容40年前的老程序https://mp.weixin.qq.com/s/3bB-7G4tGTvdahPfTV32UQ
- 博客与视频 https://www.stroustrup.com/videos.html 
- C++语言的设计和演化
-  ModernCpp-李建忠

1. ceph项目源码
-  [Ceph 性能瓶颈分析与优化 (混合盘篇)](https://my.oschina.net/u/4565392/blog/5130265)、
- https://blog.csdn.net/weixin_43778179
- https://blog.csdn.net/weixin_43778179
- 告别头文件，编译效率提升 42%！C++ Modules 实战
- https://bot-man-jl.github.io/articles/?post=2022/Cpp-Project-Compile-Optimization
在使用 clang 编译时，可以添加 [`-ftime-trace` 参数](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-ftime-trace)，记录 “在生成一个目标文件时 各个环节的耗时细节”，并将结果输出为 [`chrome://tracing`](https://www.chromium.org/developers/how-tos/trace-event-profiling-tool/) 支持的 **火焰图** _(flamegraph)_ `.json` 格式文件
## 为什么写这篇文章



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
- 整天做低优先级的任务，变成为部门裁员指标一部分


如果更进一步交流 添加 微信：wang_cyi
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)