---
title: "TiKV Contributors day3："
date: 2024-07-31
description: "Tidb"
draft: false
tags: ["Tidb"] 
---





 成为tikv贡献者第三天：存储引擎



本文是TiKV 源码阅读第三篇，

目标：snapshot进行进一步理解，一边理解rust语法，一边抽象模块之间关系。



**阅读完你将收益融如下**

1. 容易题不丢分，先吃小甜点+1分，增加学习动力 掌握一个语法知识
    std::move()实现原理 
  - 为什么支持传入的是左值还是右值这2个参数是&&原因吗
  -  为什么返回的都是右值引用，是static_cast功劳吗？
  - forward怎么做到区分类型的，他做不到 还是依赖remove_reference吗？
  -  了解rust 用引用 生命周期用法
    不管什么raft，什么rockdb，本来不明白，完全当作黑盒，  完全是程序员思维。



## **回顾：**

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



## rust语法知识

**SnapContext<'_>  ，必须看懂这个语法, 看到它其他完全没心情看了，我是偏离主轨道，研究这个语法了 **



1. **回顾看看c++怎么处理的：**

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
//返回局部变量的引 运行时候出错
~~~



2. **rust怎么处理这个情况** 

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
   
    } //s 被销毁 
    
    println!("{}", result); // borrow result 依然指向 s
}

fn create_string_reference<'a>(s: &'a str) -> &'a str {
    s //无分号 return s;
}

~~~



明白了， 

- 在c++中  ref 和 local_string 之间范围关系 在编译时候没有约束
- 在rust 中 result 作用范围 和 s作用范围不一致，编译报错
- 引用的生命周期至少应该与其所指向的值的生命周期相同。



3. **通过std::move 理解， T,T&&  template<T&&>**



~~~c++
代码位置
https://en.cppreference.com/w/cpp/types/remove_reference
https://en.cppreference.com/w/cpp/utility/move
https://en.cppreference.com/w/cpp/utility/forward

/**
 *  @brief  Convert a value（left or right） to an rvalue.
 *  @param  __t  A thing of arbitrary type.
 *  @return The parameter cast to an rvalue-reference to allow moving it.
*/
template<typename _Tp>
  constexpr typename std::remove_reference<_Tp>::type&&
  move(_Tp&& __t) noexcept  ---（1）  模板中的&&不代表右值引用，而是万能引用，其既能接收左值又能接收右值
  {
      return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);  -----（2）
  } 
  
 C++11 typename remove_reference＜T＞::type（去除传入类型引用符号）-说人话
  -啥意思 就是获取类T，
  remove_reference 定义三次 c++语法应该报错了，注意是模板 在编译时候选择其中一个匹配
      
 对于模板参数T, T&, T&&. 其type类型都为T,     
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

- 模板实例化简单举例 ，

  int &&move(int)

  int &&move(int &a)

  int &&move(int && a)

   C++11 typename remove_reference＜T＞::type（去除传入类型引用符号）-说人话

   啥意思 就是获取类T，int 

  

- 别被static_cast误导了，不是static_cast保证了返回的是右值。

  

4. **std::forward**

~~~c++

这些函数模板 forward 是 C++ 中的完美转发函数，
通常与模板类模板参数一起使用，以实现转发机制。完美转发允许将函数的参数以原始的值类别（lvalue 或 rvalue）传递给另一个函数
不是人话
 
简化理解：为什么直接用不行
template<class T>
void wrapper(T&& arg)
{
    // arg is always lvalue
    // 直接使用：arg 无论传递什么在使用过程中当作右值使用，丢失类型信息，
    foo(std::forward<T>(arg)); 
    //std::forward<T>(arg  Forward as lvalue or as rvalue, depending on T
}

简化理解：forward怎么做到区分类型的，他做不到 还是依赖remove_reference
   
template< class T >
T&& forward( typename std::remove_reference<T>::type& t ) ;
如果 T 是一个引用类型（例如 int&），std::remove_reference<T>::type 将变为 int。----这个地方就是lvalue
    
T&& forward( std::remove_reference_t<T>& t ) 
 这个函数模板同样接受一个左值引用 t，并且通过别名模板移除引用，然后以 T&&（右值引用）的形式转发
 
T&& forward( std::remove_reference_t<T>&& t ) 
通用引用（universal reference）
如果 T 是一个左值引用类型（如 int&），T&& 将折叠为 T&；
  
如果 T 是一个右值引用类型（如 int&&），T&& 将折叠为 T
~~~



明白了，

类型折叠 简单理解成 语文没有逗号中的断句，商家最终解释权

从样板到最后实例化中间还有过程 
这个编译器发挥作用了





## rust项目结构

~~~
/tikv-master/src/storage
tree -d     
├── kv
├── mvcc
│   └── reader
│       └── scanner
├── raw
└── txn
    ├── actions
    └── commands

~~~



mod.rs 文件作用

- tikv-master\src\storage\mod.rs

- `mod.rs` 作用定义`storage`模块的公共接口和私有实现。

  1. **公共接口**：使用`pub`关键字来声明**子模块**中哪些项是公共的，可以被其他模块访 

     pub mod kv;      storage/kv 声明目录下模块被其他模块访

     pub mod mvcc; 声明内部子模块是公共的，可以被其他模块

     pub mod raw; 声明内部子模块是公共的，可以被其他模块

     pub mod txn; 声明内部子模块是公共的，可以被其他模块

     

     类似 c++中 头文件 提供public 函数

     

     use crate::storage::mvcc::MvccReader;

     

     use：类似 c++中 头文件 using namespace std;

     crate：**路径指定**：`crate::storage::mvcc`是`MvccReader`所在的路径

     

     

  2. 定义类

      pub struct Storage<E: Engine, L: LockManager

  

  

  tikv-master\src\storage\kv\mod.rs 作用 

  ~~~rust
  kv % tree 
  .
  ├── mod.rs
  └── test_engine_builder.rs
  
  
  pub use tikv_kv::*; //引用 tikv_kv模块全部内容 
  
  
  [package]
  name = "tikv_kv"
  version = "0.1.0"
  authors = ["The TiKV Authors"]
  description = "The key-value abstraction directly used by TiKV"
  edition = "2018"
  publish = false
  
  ~~~

  > 为 什么：storage\kv 没有 任何代码，

  pub use tikv_kv::*

  

  类似在C++中，头文件通常用于声明类的接口，包括虚函数，具体实现可能调用其他动态库

  ~~~
  class MyClass {
  public:
      MyClass();  // 构造函数
      virtual ~MyClass();  // 虚析构函数
      virtual void myFunction();  // 虚函数
  }; 
  ~~~

  

  

  **tikv_kv是什么**

  ~~~rust
  tikv_kv % tree     
  .
  ├── Cargo.toml
  └── src
      ├── btree_engine.rs
      ├── cursor.rs
      ├── lib.rs
      ├── raftstore_impls.rs
      ├── rocksdb_engine.rs
      └── stats.rs
  
  ~~~

  

  TiKV 支持了三种不同的 KV 存储引擎，

  - 单机 RocksDB 引擎、 rocksdb_engine.rs RocksDB 是一个单机的 Key-Value Map。
  - 内存 B 树引擎  btree_engine.rs
  - RaftKV 引擎

  

  

  ## 小结
  
  抛开一切概念 ，抛开 一切流程图，反正我是看不懂
  
  终于看到   rocksdb_engine 后面怎么看不清楚，有大神告诉一下吗/
  
  ~~~
  tikv_kv % tree     
  .
  ├── Cargo.toml
  └── src
      ├── btree_engine.rs
      ├── cursor.rs
      ├── lib.rs
      ├── raftstore_impls.rs
      ├── rocksdb_engine.rs
      └── stats.rs
  
  
  ~~~
  
  



## 参考

- TiKV 源码解析系列文章（十九）read index 和 local read 情景分析

- TiKV 源码解析系列文章（十）Snapshot 的发送和接收

- PointGet的一生

- Rust 参考手册 中文版

- TiKV RocksDB读写原理整理

- TinyKV Course: RaftStore执行流程

- TiDB 技术内幕 – 说存储

- TiKV 源码解析系列 - Raft 的优化

  
  
  
  
  
  
  
  
  
