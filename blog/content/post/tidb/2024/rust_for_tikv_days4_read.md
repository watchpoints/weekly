---
title: "T成为tikv贡献者第三天："
date: 2024-07-8
description: "Tidb"
draft: false
tags: ["Tidb"] 
---





 成为tikv贡献者第三天：读请求 全链路跟踪



本文是TiKV 源码阅读第三篇，

目标：snapshot进行进一步理解，一边理解rust语法，一边抽象模块之间关系。



写完后我收益一下

1. 不管什么raft，什么rockdb，但是至少 了解  rust 用引用 生命周期用法。

   先来个小甜点，完全是程序员思维。

   因为回顾 std::move() 无论你传入的是左值还是右值，返回的都是右值引用？

   

   



回顾：

 [成为tikv贡献者第二天：读请求 全链路跟踪](https://asktug.com/t/topic/1029864?_gl=1*1hzzwb7*_ga*MTA4NDM3MTU5MC4xNzAzMDM2OTcz*_ga_5FQSB5GH7F*MTcyMjMzNjMzMC4zMi4xLjE3MjIzMzYzMzguMC4wLjA.)

 介绍了PC,KvService，Storage  分层关系 就是就是函数调用关系

Storage  提供了 Snapshot 来提供查询功能

代码位置

- tikv-master\src\storage\mod.rs
- 接口

~~~rust
/// Get a snapshot of `engine`.
fn snapshot(
    engine: &mut E,
    ctx: SnapContext<'_>,
) -> impl std::future::Future<Output = Result<E::Snap>> {
    kv::snapshot(engine, ctx)
        .map_err(txn::Error::from)
        .map_err(Error::from)
}
~~~







**rust语法知识1   SnapContext<'_>  ，必须看懂这个语法, 看到它其他完全没心情看了，我是偏离主轨道 **



- 回顾看看c++怎么处理的：stl 模板 这里不讨论 ，主要'a，不知道为什么这么规定。

​    C++ 没有 Rust 那样的生命周期系统，因此程序员需要更加小心地管引用的生命周期

~~~c++
#include <iostream>
#include <string>
const std::string& get_local_reference() {
    std::string local_string = "Hello, World!";
    return local_string; // 错误：返回局部变量的引用（static除外）
}
int main() {
    const std::string& ref = get_local_reference();
    std::cout << ref << std::endl; // 未定义行为：可能打印出 "Hello, World!"，也可能打印出垃圾数据
    return 0;
}

~~~



- rust怎么处理这个情况 编译时候语法检报错。

~~~rust

fn main() {
   
    let s = String::from("Hello, world!"); // 创建一个字符串并赋予所有权
    let result = create_string_reference(&s); // 传入字符串的引用
    {
        let a = s;
        // println!("{}", s) 语法报错
       // 在rust中，所有权规则主要有以下三条:
       //每个值都有一个所有者
       //每个值只能有一个所有者 等于c++ move 来的变量不再有效
       // 当所有者不在程序运行范围时，该值将被删除
   
    } //s  到这里结束了
    println!("{}", result); // result 依然在使用 
    //borrow later used here
}

fn create_string_reference<'a>(s: &'a str) -> &'a str {
    s
}

~~~



明白了， 

- 在c++中  ref 和 local_string 之间范围关系 在编译时候没有约束
- 在rust 中 result 作用范围 和 s作用范围不一致，编译报错
- 引用的生命周期至少应该与其所指向的值的生命周期相同。

画外音：rust为了保证编译正确，结果代码可读性太差了，其实不用这些约束，还增加概念。



- c++类型推导  T,T&&  template<T&&>

~~~c++


/**
 *  @brief  Convert a value（left or right） to an rvalue.
 *  @param  __t  A thing of arbitrary type.
 *  @return The parameter cast to an rvalue-reference to allow moving it.
*/
template<typename _Tp>
  constexpr typename std::remove_reference<_Tp>::type&&
  move(_Tp&& __t) noexcept  ---（1）  模板中的&&不代表右值引用，而是万能引用，其既能接收左值又能接收右值
  { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); } -----（2）
  
 
 对于模板参数T, T&, T&&. 其type类型都为T, 而且T如果有const属性，则type也会保留     
/// remove_reference
template<typename _Tp>
  struct remove_reference
  { typedef _Tp   type; };

template<typename _Tp>
  struct remove_reference<_Tp&>
  { typedef _Tp   type; };

template<typename _Tp>
  struct remove_reference<_Tp&&>
  { typedef _Tp   type; };


~~~

1）  模板中的&&不代表右值引用，而是万能引用，其既能接收左值又能接收右值

2STL——萃取机制（Traits）

- .定义：traits中文意思是特性，它通过提取不同类的共性，使得可以统一处理。

- 技术实现：traits运用显式模板特殊化(模板偏特化，全特化)将代码中因为类型不同而发生变化的片段提取出来，
- 用统一的接口来包装，并通过traits模板类公开的接口间接访问相应的类。



明白了，

- stl中   value_type  不是变量，*类型*别名typedef/using 

- std::remove_reference<_Tp>::type 通过作用域 萃取 type

-  move(_Tp&& ) 输入

  





## 参考

- TiKV 源码解析系列文章（十九）read index 和 local read 情景分析

- TiKV 源码解析系列文章（十）Snapshot 的发送和接收

- PointGet的一生

  Rust语言圣经(Rust Course)
