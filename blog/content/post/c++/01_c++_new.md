---
title: "c++入门指南"
date: 2024-08-07
description: "do book"
draft: false
tags: ["man"]
categories: ["do book"]
---





## 内存管理



 使用运算符 new 进行动态内存分配



**函数支持重载，运算符同样也支持重载**

​     C++的提供了 重载运算符这一特性，  本质也是operators（）函数重载，当遇到该运算符时就调用函数一样。

 运算符重载的限制

  ![img](https://i-blog.csdnimg.cn/blog_migrate/2d9dc48706af499c293b68a5c79dd0ec.png)



![](https://i-blog.csdnimg.cn/blog_migrate/27306e4e14171e7ee0307589877451e0.png) 



小提示：Markdown左对，在原来基础上，后面一个空格就解决了 右对齐HTML css语法



**重载运算符new**

~~~c++

throwing (1)	void* operator new (std::size_t size); 
// throwing allocation ，On failure, it throws a bad_alloc exception,
nothrow (2)	    void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
//nothrow allocation   on failure it returns a null pointer  instead of throwing an exception
placement (3)	void* operator new (std::size_t size, void* ptr) noexcept;
//placement Simply returns ptr (no storage is allocated).
~~~



https://cplusplus.com/reference/new/operator%20new/



- https://csguide.cn/cpp/memory/malloc_free.html#malloc-%E5%92%8C-free
- https://www.bookstack.cn/read/wangdoc-clang-tutorial/docs-memory.md
- https://juejin.cn/post/7353112518146605108
- https://cplusplus.com/doc/tutorial/dynamic/
