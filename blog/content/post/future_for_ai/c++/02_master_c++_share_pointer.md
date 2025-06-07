---
title: c++代码编程规范
date: 2024-09-07
description: c++
draft: false
tags:
  - man
categories:
  - do book
---

成为面试官 第一练

未了节省读者时间一句话解释

shared_ptr 的引用计数本身是安全且无锁的，但对象的读写则不是，
因为 shared_ptr 有两个数据成员，读写操作不能原子

- 一个 shared_ptr 对象实体可被多个线程同时读取（文档例1）；

• 两个 shared_ptr 对象实体可以被两个线程同时写入（例2），“析构”算写操作；

• 如果要从多个线程读写同一个 shared_ptr 对象，那么需要加锁（例3~5）



废话不多说直接看源码，以下基于gcc13版本shared_ptr实现

>shared_ptr< T >继承自__shared_ptr<T, __default_lock_policy>，
__shared_ptr拥有实际指针element_type*
和成员引用计数__shared_count<__default_lock_policy>

UML结构图
![](https://blog.noct.site/images/shared_ptr.png)
代码位置：
https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/include/bits/shared_ptr.h#L174

 class shared_ptr : public __shared_ptr<_Tp>

https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/shared_ptr_base.h#L1430
~~~c++
// Forward declarations.
template<typename _Tp, _Lock_policy _Lp = __default_lock_policy>
class __shared_ptr;
  element_type*	   _M_ptr;           // Contained pointer.  拥有实际指针
 __shared_count<_Lp>  _M_refcount;    // Reference counter. 成员引用计数
~~~


https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/shared_ptr_base.h#L1137

 _Sp_counted_base<_Lp>*  _M_pi;


https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/shared_ptr_base.h#L2246



# 引用计数操作

增加引用计数，shared_ptr和__shared_ptr的复制构造都使用默认编译器构造，也就是使用__shared_count复制构造：


# [How do shared pointers work?](https://stackoverflow.com/questions/2802953/how-do-shared-pointers-work)

Best practice shared_ptr site:stackoverflow.com OR site:reddit.com  OR site: github

`std::shared_ptr` reference count is _atomic_. increasing or decreasing the reference count **requires _atomic_ increment or decrement**.




https://stackoverflow.com/questions/441814/fully-thread-safe-shared-ptr-implementation

https://blog.noct.site/2024/03/22/gcc13_shared_ptr/
https://www.cnblogs.com/chaohacker/p/14802112.html

## [为什么多线程读写 shared_ptr 要加锁？](https://www.cnblogs.com/Solstice/archive/2013/01/28/2879366.html "发布于 2013-01-28 05:17")
https://www.cnblogs.com/Solstice/archive/2013/01/28/2879366.html## [为什么多线程读写 shared_ptr 要加锁？](https://www.cnblogs.com/Solstice/archive/2013/01/28/2879366.html "发布于 2013-01-28 05:17")

陈硕（giantchen_AT_gmail_DOT_com）

2012-01-28

我在《Linux 多线程服务端编程：使用 muduo C++ 网络库》第 1.9 节“再论 shared_ptr 的线程安全”中写道：

（shared_ptr）的引用计数本身是安全且无锁的，但对象的读写则不是，因为 shared_ptr 有两个数据成员，读写操作不能原子化。根据文档（[http://www.boost.org/doc/libs/release/libs/smart_ptr/shared_ptr.htm#ThreadSafety](http://www.boost.org/doc/libs/release/libs/smart_ptr/shared_ptr.htm#ThreadSafety "http://www.boost.org/doc/libs/release/libs/smart_ptr/shared_ptr.htm#ThreadSafety")）， shared_ptr 的线程安全级别和内建类型、标准库容器、std::string 一样，即：

• 一个 shared_ptr 对象实体可被多个线程同时读取（文档例1）；

• 两个 shared_ptr 对象实体可以被两个线程同时写入（例2），“析构”算写操作；

• 如果要从多个线程读写同一个 shared_ptr 对象，那么需要加锁（例3~5）。

请注意，以上是 shared_ptr 对象本身的线程安全级别，不是它管理的对象的线程安全级别。

后文（p.18）则介绍如何高效地加锁解锁。本文则具体分析一下为什么“因为 shared_ptr 有两个数据成员，读写操作不能原子化”使得多线程读写同一个 shared_ptr 对象需要加锁。这个在我看来显
