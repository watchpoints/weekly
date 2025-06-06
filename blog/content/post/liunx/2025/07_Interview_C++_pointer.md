---
title: 面试题——谈谈指针和引用的区别
date: 2025-03-31
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---

# 一、 大纲  






### 什么场景必须用指针,不能使用引用。

#### 1. 大局观：重要的数据结构 必须用指针表示
-  STL 提供容器vector map ,set 
- STL提供智能指针，自定义一些类
- 基本操 创建，插入，删除时候 **经常发生变化** 因此必须用指针
- **画外音**：平衡二叉搜索树：红黑树,AVL 树，哈希表：数组+链表 就是下一个问题


### 2. 大局观：用指针/智能指针 表示对资源“所有权“，引用只是借用。

- 提示：
	c/c++ 特点 就是不从语法层面发明一些新概念，通过最佳实践 自我约定**的概念
	在 C++ 中**，所有权是主要是通过**原始指针**或**智能指针**来表达。
	**在 Rust 中**，所有权是**语言级别内建规则**

- 指针有所有权（ownership）管理，**引用是别名而非资源管理者**
  通过c语言提供malloc free 只操作，指针，不操作引用。
- Smart Pointers 自定义的类。
- Item 18: Use std::**unique_ptr** for exclusive-ownership resource management. 
   禁止拷贝和复制，**同一时间只能有一个** `unique_ptr` 指向资源。
- Item 19: Use std::shared_ptr for shared-ownership resource managemen
- tem 20: Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.

- 函数：std::move()
错误理解：根据英文字母move理解，实现**移动语义**，这个❌理解。 引用不管理资源。
正确理解：**本身只做类型转换 ，返回右值引用**，**自己的类中实现移动构等自定义函数等，才能完成资源所有权转移

>  引用只拥有指针部分功能，上面场景不能代替指针，**涉及动态分配、容器、多态、共享、生命周期管理时**用指针（最好是智能指针）
### 3.  

### 参考书籍
- 侯捷老师的《STL原码剖析》
- 
    
你是10年经验c++架构师 在虚函数 场景必须用指针不能引用


你是10年经验c++架构师，什么场景下必须用引用 不能用指针
区别意思是什么场景必须用指针不能引用，什么场景下必须用引用 不能用指针

潜台词
1. 实际开发项目中怎么使用的怎么回答，不要背诵 不要背诵。
2. 问的c++，但是对C语言特别熟悉，其他语言golang，rust 也一定了解。


提示：### std::move()


**为了减少临时变量的copy**


## 什么场景下必须用引用 不能用指针



Item 3: Never treat arrays polymorphically.
Item 28: Understand smart pointers

- **所有权（Ownership）** 就是你拥有这本书，可以随便读、改、烧掉、卖掉。
    
- **借用（Borrowing）** 就是你把这本书借给朋友




读者提问：

为什么 我刚回答引用是对象别名，不能为空 ....
还没回答完毕，面试官 直接打断说，还有吗？
我继续回答 指针 跳转 。。。
面试官 就到这里吧，等通知。

老王回答：
这个和工作汇报领导一个道理，时间就是金钱，



## 二. 整理素材 


- 侯捷老师的《STL原码剖析》 vector 

高性能库设计，如 `std::vector::emplace_back` 能根据传入的是左值还是右值，分别调用拷贝构造或移动构造​[](https://www.cnblogs.com/ishen/p/13771991.html?utm_source=chatgpt.com)
- **左值引用** `T&`：只能绑左值，`const T&` 可绑右值。
    
- **右值引用** `T&&`：只能绑右值，用于移动语义。
    
- **万能引用** `T&&`（出现在模板或 `auto` 推导中）：能绑左值也能绑右值，用于完美转发
https://en.wikipedia.org/wiki/Reference_%28C%2B%2B%29?utm_source=chatgpt.com#Rvalue_reference

move semantics, or perfect forwardin
https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers


https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers


**C++ 智能指针遇到的坑**，- 无脑只使用 shared_ptr
在10万次对象创建测试中，unique_ptr返回与裸指针直接 new 的性能差异小于1%
## 三 内容 


# 一、 最佳实践

### 什么场景必须用指针,不能使用引用。

#### 1. 大局观：重要的数据结构 必须用指针表示

-  STL 提供容器vector map ,set，智能指针，自定义类。

- 基本操 创建，插入，删除时候 **经常发生变化** 因此必须用指针

- **画外音**：平衡二叉搜索树：红黑树,AVL 树，哈希表：数组+链表 就是下一个问题


### 2. 大局观：用指针/智能指针 表示对资源“所有权“，引用只是借用。

- 提示：

    c/c++ 特点 就是不从语法层面发明一些新概念，通过最佳实践 自我约定**的概念

    在 C++ 中**，所有权是主要是通过**原始指针**或**智能指针**来表达。

    **在 Rust 中**，所有权是**语言级别内建规则**

- 指针有所有权（ownership）管理，**引用是别名而非资源管理者**,free 函数传递指针，不传递引用。

- 引用只具备指针部分工能，nullptr,ptr++，ptr**, sizeof 虚函数  都是小区别

- **画外音** ：右值,右值引用, move(),移动语义，类的移动赋值函数, 智能指针理解


 
# 二、 最佳实实践

### 什么场景必须用引用，不能使用指针


<font color="#245bdb">大局观：引用分为三个类型，左值引用，右值引用，万能引用</font>

- 左值引用：只借用，不负声明周期管理，在类的拷贝构造函数，复制操作函数使用。指向存在对象（左值）
- 右值引用：指向 右值（临时值，代表马上销毁），std:move 函数能把一个左值变成右值。配合当前自定义类或者使用容器 <font color="#245bdb">重载移动构造函数，移动复制运算符</font>
   例如：unique_ptr **不能进行复制操作只能进行移动操作** 。

 - 返回值是unique_ptr：C++17和之后的版本中，由于强制开启了返回值优化，所以 move 操作也不需要了，进行了RVO优化
- 万能引用：右值引用 在参数传递过程中 变为左值。配合std:forwd 使用，根据不同参数调用不同重载函数。


描述 push_back(move(t)) 过程

1. ​**​模板推导​**​：`std::move`将`t`转为右值引用；
2. ​**​函数重载​**​：容器选择右值引用版本的`push_back`；
3. ​**​资源转移​**​：调用元素类型的<font color="#ff0000">移动构造函</font>数完成高效资源转移(类实现移动构造函)



画外音：C++ 智能指针<font color="#00b050">避坑指南</font>

push_back(std::move(head)); push_back 怎么知道 head类型是什么，实现 移动语义操作

push_back(std::move(head)); push_back 怎么知道 head类型是什么，实
## 链接我 


坚持思考，方向比努力更重要。

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


## 参考
- https://cloud.tencent.com/developer/article/1784441 【】