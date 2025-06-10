---
title: 从青铜到王者系列——谈谈指针和引用的区别
date: 2025-03-31
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---

## 大局观

问：指针和引用有什么区别？从青铜到王者怎么回答

提示：
1. 从项目系统设计开发角度，怎么设计，怎么开发，怎么编译，测试的 就是从大局观角度回答
2. 项目中都不使用 语法就不回答,有自己的最佳实践
3. 大局观1：资源说有权上：引用不控制释放只使用指针，代码安全角度
4. 大局观2：重要数据结构 ，经常插入，删除，都使用指针，不使用引用。

如果您觉得阅读本文对您有帮助，
请点一下“**点赞，转发**” 按钮，
您的“**点赞，转发**” 将是我最大的写作动力！
	
### 青铜回答：
##### 小青的思考：
1. 根本不知道怎么回答？想到书本上内容引用是变量的别名，
2. 没有什么本质区别？这个相同点
##### 开始回答
1. 区别指针可以赋值null，引用不行。
2. 面试官 反问，还有吗？开始支支吾吾,就是这样根本，还怎么回答
#### 点评
1. 仅仅开发使用角度，没有涉及模块，动态库，编译 内容。

## 白银回答

##### 小白的思考：
1. 在参数传递中，指针和引用都感觉都可以？
2. const 修饰 这个感觉回答不好
3. c++ 类 涉及构造创建资源，析构释放资源，代码这样写的，怎么回答区别
##### 开始回答
1. 在参数传递中，推荐 const 引用 ，如果空指针必须二级指针
2. 通过构造函数和析构函数申请，释放地址
3. 面试官问 还有吗？支支吾吾回答不出来了
#### 点评
1. 通过类管理 指针申请，和释放 就是c++最重要特点，不需要垃圾回收，这个没有细说
2. const 为什么用没说出来，效率提示，提升多少。
3. 在参数传递和类设计 经常指针，只会用， 不知道为什么这么用
4. 指针拷贝，浅拷贝，导致 core问题没有说
5. 线程安全呢？

### 钻石回答
#### 小钻的思考
1. 我准备C++高级特性右值引用这个，不是指针呀？指针可以 区别在哪里想不清楚？
2. 并发变成和区别啥关系？为题目限制主了，无法发挥更多。
##### 开始回答
1. const  引用指向右值
2. 最后刚毕业场景，没有改变


## 王者回答


#### 什么场景必须用指针,不能使用引用。

#### 1. 大局观：重要的数据结构 必须用指针表示 创建，插入，删除时候 **经常发生变化** 因此必须用指针
-  STL 提供容器vector map ,set 
- **引用**：​由于引用不能被重新绑定，也无法进行原子操作，因此在无锁编程中通常不使用引用
- **画外音**：平衡二叉搜索树：红黑树,AVL 树，哈希表：数组+链表 就是下一个问题

### 2. 大局观：用指针/智能指针 表示对资源“所有权“，引用只是借用。

- 指针有所有权（ownership）管理，
- 引用是别名而非资源管理者，不负责释放。
- new 和delete 不用引用

>Declares a named variable as a reference, that is, an alias to an already-existing object or function.
>//引用不会分配自己的内存地址，它会直接绑定到原变量的地址上。


### 什么场景必须用引用，不能使用指针

>大局观是什么？
>引用支持，c++很多特性移动语义，完美转发，移动赋值函 ，左值，右值，万能引用
>**右值引用**是连接这两个截然不同的概念的胶合剂。
>它是使移动语义和完美转发变得可能的基础语言机制。
>
>c++ 不喜欢引入新名字，最本质是什么 是什么 右值生命周期理解上。
>是什么改变引起的。



|类别|示例|是否可修改|生命周期管理|
|---|---|---|---|
|纯右值|`5`, `a + b`|不可修改|表达式结束即销毁|
|将亡值|`std::move(obj)`|可通过右值引用修改|延长至引用作用域结束|
|类类型右值|`Widget().modify()`|可调用成员函数修改|同将亡值规则|

- 右值：const T&  被设计为永远不可修改 。
- 右值引用 ，由 `T&&` 标识。这指的是允许在初始化后进行修改的临时变量，目的是实现“移动语义”

- **完美转发**(`std::forward`)：结合模板参数推导与 `std::forward<T>`，函数模板可以“保留”实参的值类别（左值/右值）传递给下游函数，使目标函数接收到的实参与被传递给转发函数的实参保持一致，指针无法实现此特性
- 在模板元编程中，​**​引用​**​通过保留完整类型信息，为 Concepts 提供了精确的类型约束基础，而​**​指针​**​因隐式退化可能破坏类型完整性
-  Effective Modern C++ # [第5章 右值引用，移动语义，完美转发]
- `std::move`并不移动任何东西，完美转发也并不完美。移动操作并不永远比复制操作更廉价；即便如此，它也并不总是像你期望的那么廉价


>**画外音** ：右值,右值引用, move(),`std::forward，移动语义，类的移动赋值函数, 智能指针理解

描述 push_back(move(t)) 过程

1. ​**​模板推导​**​：`std::move`将`t`转为右值引用；
2. ​**​函数重载​**​：容器选择右值引用版本的`push_back`；
3. ​**​资源转移​**​：调用元素类型的<font color="#ff0000">移动构造函</font>数完成高效资源转移(类实现移动构造函)


## 动手实践

#### std::forward

-  std::forward 一般用在函数内部，实现完美转发，保留原始类型【保留区分类型】
- std::move 函数在参数传递过程总，`std::move`将它的实参转换为一个右值，这就是它的全部作用。 【去掉统一转化】所以记住`std::move`做什么和不做什么很重要。它只进行转换，不移动任何东西。
- std::move doesn’t move anything, for example, and perfect forward‐
- ing is imperfect. Move operations aren’t always cheaper than copying;
- 当编译器决定哪一个`std::string`的构造函数被调用时，考虑它的作用，将会有两种可能性

```c++
// utility
template <typename T>
// left 
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}
//https://en.cppreference.com/w/cpp/types/remove_reference
//right 
template <typename T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
    static_assert(!std::is_lvalue_reference_v<T>,
                  "bad forward call");
    return static_cast<T&&>(t);
}

提问1
# 为什么std::forward需要重载出& 和 && 两个版本？
# T&& 怎么区分左右值的
# 为什么 `std::move` 不“移动”成指针
# 指针能指向右值吗？
//右值引用（`int&&`）本质上是 ​**​对临时对象的引用​**​，而非传统指针。它允许直接操作右值
```


### std::move()

```c
  /**
   *  @brief  Convert a value to an rvalue.
   *  @param  __t  A thing of arbitrary type.
   *  @return The parameter cast to an rvalue-reference to allow moving it.
   *  @since C++11
  */
  template<typename _Tp> [[__nodiscard__,__gnu__::__always_inline__]]
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { 
	    return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); 
    }


```
## 链接我 


坚持思考，方向比努力更重要。

关注公共号：后端开发成长指南  回复电子书 

如果更进一步交流 添加 微信：wang_cyi


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)
### 参考
####  [1] C++ Programming Language
- https://bartoszmilewski.com/2008/12/01/c-atomics-and-memory-ordering/
##### 【2】 Rust 语言从入门到实战

##### 【3】 book # 深入探索C++对象模型
- 4.5 inline Functions”章节深入探讨了内联函数在C++底层对象模型中的实现机制、性能影响以及与编译器优化的交互
- 内联函数是C++中的一个特性
- **短函数​**​（<100 行）优化效果显著（如 `getter/setter` 提升 ​**​20-30%​**​）
- 高级优化级别（如 `-O3`）结合链接时优化（LTO）可提升内联效果，
#### 【4】 3fs c++20
- 在 C++20 引入了约束与概念，这一核心语言特性是所有使用模板的 C++ 开发者都期待的
- https://mq-bai.gitbook.io/modern-cpp-templates-cookbook/11-yue-shu-yu-gai-nian
- 使用模板做泛型编程，最大的问题就是缺少良好的接口
- 现代 C++20 实战高手课
- https://github.com/samblg/cpp20-plus-indepth/tree/main/05/01-BasicConcepts
- 可以使用 C++20 引入的 requires 表达式，并且支持约束的逻辑组合，这是对 C++20 之前 enable_if 和 type_traits 的进一步抽象。
- - 可能提升10-15%的缓存命中率
- 你是10年经验c++架构师，什么场景下必须用引用 不能用指针 ,c++新特性，结合3fs开源代码说明
- 揭秘C++引用的诞生：优雅编程的艺术
##### [7】 # Effective Modern C++
- https://cntransgroup.github.io/EffectiveModernCppChinese/5.RRefMovSemPerfForw/item24.html
- **Item 24: Distinguish universal references from rvalue references**
- T&&” is either rvalue reference or lvalue reference.
- If the initializer is an lvalue, the universal reference corresponds to an lvalue reference.
- Item 25: Use std::move on rvalue references,std::forward on universal references
- The purpose of
this Item is to keep your expectations grounded
 - C++11is willing to generate move operations for classes that lack them 【自定义类需要自己实现】
 - 对于没有明确支持移动操作的类型，并且不符合编译器默认生成的条件的类，没有理由期望C++11会比C++98进行任何性能上的提升。
 - If those types offer cheap move

operations, and if you’re using objects in contexts where those move operations will

be invoked, you can safely rely on move semantics to replace copy operations with

their less expensive move counterparts.
C++11 引入了右值引用（&&），使得右值的概念更加丰富，并区分了**纯右值（prvalue）**和**将亡值（xvalue）**。

- **纯右值（prvalue, Pure Rvalue）**
    
    ：
    

- 纯粹的临时值，如字面量（42）、表达式（5 + 3）或非引用的函数返回值。
    
- 示例：int&& r = 10;（10 是纯右值，可以绑定到右值引用）。
    

- **将亡值（xvalue, Expiring Value）**
    
    ：
    

- 即将销毁但仍可操作的对象，通常是通过 std::move 或某些函数返回值生成的


