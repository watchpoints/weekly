---
title: 从青铜到王者系列：3FS如何实现一个文件系统
date: 2025-03-23
description: 从青铜到王者
draft: false
tags:
  - code_reading
---

深入浅出理解DeepSeek 3FS (3) 说出来你可能不信：C++11发布13年了！


大家好，我是小王同学，

本文希望帮你深入理解分布式存储系统3FS更进一步


### 读者对象(可选)

#### **1. 目标：冲击大厂，拿百万年薪**

- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 通过目标驱动行动
    
#### **2. 现状：缺乏实战，渴望提升动手能力**

- 公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
    
- 想通过阅读优秀的源码，提高代码能力，从“不会写”到“敢写”，提升C++编程自信。
    
- 需要掌握高效学习和实践的方法，弥补缺乏实战经验的短板。

####  3. 价值：优秀完成任务，成为团队、公司都认可的核心人才
 **错误示范**：
- 不少同学工作很忙，天天加班，做了很多公司的事情。
  但是 不是本团队事情，不是本部门事情，领导不认可。
- 只埋头写代码，从不和团队沟通，导致没人知道你做了什么。
- 做低优先级的任务，无法利他，自然不会有人“推”你向上。

陷入沉迷权利大棒的公司/领导不屑争吵小手段（跟自媒体内容创作和流量平台一个道理）不停完成任务而非优秀的完成。
- 刻意提高工作难度
- 工作中不公平对待
- 突然换了一副嘴脸
- 拉帮结派
- 制造恶性竞争
- 刻意放大缺点
- 捧杀

**优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量**

> **“If you can’t measure it, you can’t manage it.”**

 **案例对比**（谁对团队贡献更大？）

- **A程序员**：一个月写不出一行代码，一个bug不解决，一个测试不完成。
    
- **B程序员**：每个月写1000行代码，彻底解决3个核心bug，完成20个测试。
    

答案不言而喻——优秀就是这么“俗气”。



# 讲一个故事


一天 老王 交给 小王同学一个任务
ceph 在硬盘读写太慢了，请调查协程实现方式，实现适合自己的业务的协程，
然后给出能提升多少，调查报告 

小王思路：
于是 开启漫长调研，自己实现不会呢，并且现在为什么磁盘慢，提升多少怎么验证
反复讨论三个月过去了。

- 我纠结为什么自己实现呢，自己实现不会呢
- 现在为什么磁盘慢

 看到 提问：
 ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250328162018.png)


目前适合用c++17 gcc版本7.4 编译，为什么不能最新版本，在企业这个忌讳。
不用最新版本 

因此 

>为什么deepseek的3fs主要用c++20 开发，采用C++20的现代特性特征，如何避坑的







## 拆分问题1： c++内存管理


代码地址：
- https://github.com/watchpoints/master-cpp
演示平台：https://gitpod.io/new


### 1. 问题是什么？

- 一个类如何定制自己的new和delete，代替默认的 new 和delete？

### 2. 为了解决这个问题准备哪些事情(技术路线)


#####  step1  c++学习路线和资料（想要电子书关注 回复 电子书）

-  【Effective C++】条款49~52：定制new和delete 
-  More Effective C++  Item 8: Understand the different meanings of new
    and delete
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327181737.png)

  https://github.com/steveLauwh/Effective-C-Plus-Plus/tree/master
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327182220.png)

- C++内存模型
-   https://github.com/isocpp/CppCoreGuidelines
- 经典文章

	C++三部曲之二：C++内存管理全景指南]
	C++三部曲之三：C++模版的本质
	C++的最后一道坎|百万年薪的程序员

#####  step2 参考源码

（1）**C++ 标准库实现**

如果你想研究 `new/delete` 的底层实现，可以查看 GCC 或 Clang 的 C++ 标准库源码：
- **GCC libstdc++**: https://gcc.gnu.org/libstdc++/
    
- **LLVM libc++**: https://libcxx.llvm.org/

在标准库实现中，你可以找到 `std::allocator`、`std::unique_ptr` 和 `std::shared_ptr` 的实现方式。

2）**开源项目**

一些高性能的 C++ 项目也有自定义 `new/delete` 的实现，你可以参考它们的代码：

- **Google TCMalloc**（高性能内存分配器）  
    [https://github.com/google/tcmalloc](https://github.com/google/tcmalloc)
- oceanbase

#####  step3 进一步探索


### 3  参考答案 （整理过程）

### GCC、Libc和Libstdc++之间有什么关系呢？
### **详细对比：`glibc` vs `libc++` vs `libstdc++`**

| **特性**       | **glibc （libc）**          | **libc++**               | **libstdc++**            |
| ------------ | ------------------------- | ------------------------ | ------------------------ |
| **用途**       | 提供 C 语言标准库实现              | 提供 C++ 标准库实现             | 提供 C++ 标准库实现             |
| **支持平台**     | 主要用于 Linux 操作系统           | 主要与 Clang 编译器配合使用        | 主要与 GCC 编译器配合使用          |
| **内容**       | 包括所有 C 标准库函数，操作系统接口、线程支持等 | 提供 C++ 标准库的模板库（STL）、IO流等 | 提供 C++ 标准库的模板库（STL）、IO流等 |
| **库功能**      | 系统调用、内存管理、线程库、文件操作等       | C++ STL，算法，IO流等          | C++ STL，算法，IO流等          |
| **重入性与线程安全** | 提供线程安全的库支持，包含锁等线程控制功能     | 提供线程安全支持（但不一定是内建的）       | 提供线程安全支持，包含多线程功能         |


GCC、Libc和Libstdc++之间有什么关系呢？
简单来说，GCC是编译器，它负责将C++源代码编译成可执行程序。在这个过程中，GCC会链接到Libc和Libstdc++这两个库。Libc为GCC提供了底层的C语言接口，而Libstdc++则为GCC提供了C++标准库的实现

### # GCC各版本对C++的支持情况


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327210032.png)


从c++11 诞生 2011年8月1到现在14年了。
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327213446.png)

#### libstdc++.so.6与C++标准的对应关系

libstdc++库实现了ISO C++标准。

这个标准库定义了C++程序中应该包含哪些组件，例如输入输出流、字符串处理、容器、算法和迭代器等。随着C++标准的演进，libstdc++也在不断更新



划重点： gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)  现在默认版本号11.4了



>划重点：

GCC，全称GNU Compiler Collection，是GNU项目下的一个开源编译器套件。它支持多种编程语言，包括C、C++、Objective-C、Fortran、Ada、Go等。GCC不仅仅是一个编译器，它还包含了其他与编译相关的工具，如链接器、汇编器等。



![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327194324.png)

| **特性**             | `new` 关键字   | `operator new` |
| ------------------ | ----------- | -------------- |
| **是否可以重载？**        | ❌ 不能        | ✅ 可以           |
| **作用**             | 分配内存并调用构造函数 | 只分配内存          |
| **是否可以直接调用？**      | ❌ 不能        | ✅ 直接调用         |
| **能否影响 `new` 行为？** | ❌ 不能        | ✅ 影响 `new`     |
|                    |             |                |
why:

`new` **并不是** `libstdc++` 标准库的函数，而是 C++ **语言级别的运算符**，它的实现依赖于标准库中的 `operator new` 函数。



### **为什么 `new` 不是 `libstdc++` 函数**


**`new`** 运算符可以看作是 C++ 语言中的一种 **高级抽象**，其提供了一种方便的方式来在堆上分配内存并构造对象，而不需要用户显式地调用底层的内存分配函数（例如 `malloc`）。


### **为什么 `new` 被视为高级抽象？**

1. **内存管理的封装**：
    
    - 在 C 中，如果你想在堆上分配内存，你通常会使用 `malloc` 或 `calloc`，然后需要手动管理内存，包括初始化对象。
        
    - 在 C++ 中，`new` 运算符提供了一个更高级的抽象，**不仅仅是内存分配**，还自动调用对象的构造函数来初始化对象。
        
    - 这种封装隐藏了底层内存管理的细节，简化了内存分配的使用，使开发者不需要关注内存分配与对象初始化的分离。
        
2. **异常安全**：
    
    - 使用 `new` 时，如果内存分配失败，它会抛出一个 `std::bad_alloc` 异常，这使得开发者不需要显式地检查 `malloc` 返回的 `NULL` 值。这样的异常处理提供了一种更加安全和优雅的内存管理方式。
        
3. **对内存分配的透明管理**：
    
    - 虽然底层的 `operator new` 函数可以被重载，但 C++ 语言本身提供了透明的内存分配接口，允许开发者使用 `new` 运算符而不关心内存分配的细节。这种抽象化让开发者集中于高层次的程序逻辑，而不是低级的内存管理。

https://en.cppreference.com/w/cpp/memory

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327203824.png)

代码：https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/libsupc++/new_op.cc

c++目的 

Direct mapping of language constructs to hardware facilities 
• Zero-overhead abstraction
• Make the language simpler! • 
Add these two essential features now!! •
Don’t break (any of) my code!!!

### **总结**

- **`new` 运算符的确是 C++ 中的一种高级抽象**，它封装了内存分配和对象构造的细节，提供了比 `malloc` 更高层次的内存管理方式。
    
- 它使得代码更简洁、更安全，同时隐藏了复杂的底层细节，减少了手动管理内存的复杂性。
    

这种抽象方式是 C++ 提供的一个重要特性，使得开发者可以更加专注于业务逻辑而不是底层的内存管理。


> 划重点：malloc 和 operator new 在内存管理上都是 称作 [Low level memory management](https://en.cppreference.com/w/cpp/memory/new "cpp/memory/new"),高级方式呢  
> 
> Smart pointers enable automatic, exception-safe, object lifetime management.

c++ 语言本身不提供 内存池管理方式，想其他语言，对应的库。
> 

**对比 `malloc` 和 `new`**

| **特性**    | `malloc` **(函数)**  | `new` **(运算符)**            |
| --------- | ------------------ | -------------------------- |
| 是否是函数？    | ✅ 是普通函数            | 不是函数，是运算符                  |
| 是否调用构造函数？ | 不会调用               | ✅ 会调用构造函数                  |
| 返回类型      | `void*`，需要强制转换     | 直接返回正确的指针类型                |
| 是否可以重载？   | ❌ 不能重载             | ✅ `operator new` 可以被重载     |
| 是否支持类型安全？ | ❌ 需要手动转换类型         | ✅ 自动匹配类型                   |
| 释放方式      | `free(ptr);`       | `delete ptr;`              |
| 失败时返回     | `nullptr` / `NULL` | **抛出异常**（`std::bad_alloc`） |
**总结**

✅ `new` 关键字是 **C++ 语法的一部分，不能被重载**。  
✅ `operator new` 是 **内存分配函数，它可以被重载**，并且 `new` 关键字会调用它。


### 小总结

new 是c++高级抽象，具体怎么实现的只能通过汇编来推断

string *ptr = new string("Memory Management");

1. 内存分配 调用全局的 operator new 函数为对象分配足够大小的内存（例如 sizeof(std::string) 字节）。
    
2. 构造对象 使用分配得到的内存地址调用 std::string 的构造函数，并传入常量字符串 "Memory Management" 作为参数。
    
3. 返回对象指针 构造完毕后返回分配并初始化后的对象指针

```

# 假设 sizeof(std::string) 为 32 字节（仅示例，实际值依实现而定）
mov    $32, %edi             # 将32字节作为参数放入 edi (operator new 的参数)
call   operator_new         # 调用全局 operator new 分配内存，返回指针在 %rax

mov    %rax, %rdi           # 将返回的内存指针作为第一个参数传递给构造函数
lea    .LC0(%rip), %rsi     # 将 "Memory Management" 字符串常量的地址加载到 rsi
						 
# 调用 std::string(const char*) 构造函数，名称会被编译器 mangling 处理，示例名称如下：
call   _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEC1EPKc

# 构造完成后，%rax 中保存着对象指针
针
```


大局观

首先, 我们要知道, GOT和PLT只是一种`重定向`的实现方式. 所以为了理解他们的作用,  
就要先知道什么是重定向, 以及我们为什么需要重定向
推荐阅读：
程序员的自我修养--链接、装载与库
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250328113045.png)


### 3. 2 libstdc++/llvm项目源码探索过程

glibc

```cpp

代码位置：
https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/libsupc++/new_op.cc



// 使用弱符号 (weak definition)，允许用户提供自定义的 operator new 覆盖该实现
_GLIBCXX_WEAK_DEFINITION void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)
{
  void *p;  // 用于存储 malloc 分配的内存指针

  /* 
     C++ 规范要求 operator new(0) 不能返回 nullptr，所以如果 sz == 0，强制分配至少 1 字节。
  */
  if (__builtin_expect(sz == 0, false)) // `__builtin_expect` 用于优化 CPU 预测分支，提高性能
    sz = 1;

  /* 
     进入循环尝试分配内存，直到 malloc 成功或 new_handler 处理失败。
     如果 malloc 返回 nullptr，意味着内存分配失败，尝试调用 new_handler 释放一些内存。
  */
  while ((p = malloc(sz)) == nullptr)
  {
      // 获取当前程序全局的 new_handler
      new_handler handler = std::get_new_handler();

      // 如果没有注册 new_handler，抛出 std::bad_alloc 异常或终止程序
      if (!handler)
        _GLIBCXX_THROW_OR_ABORT(bad_alloc());

      // 调用用户提供的 new_handler，尝试释放一些内存
      handler();
  }

  // 成功分配内存后，返回指针
  return p;
}



https://github.com/llvm/llvm-project/blob/main/libcxx/src/new.cpp

// ------------------ BEGIN COPY ------------------
// Implement all new and delete operators as weak definitions
// in this shared library, so that they can be overridden by programs
// that define non-weak copies of the functions.


https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/a00035_source.html#l00187


面试官:你知道C++函数重载吗?为什么C语言不支持函数重载

升级 C语言解决函数重载
```




### **什么是弱符号 (weak definition)?**

**弱符号 (weak symbol)** 是指：

- **链接器 (linker)** 允许多个相同名称的定义共存，但会优先选择**非弱符号 (strong definition)**。
    
- 如果没有找到**非弱符号**，则使用弱符号提供的默认实现。
    
- **适用于动态库 (shared library) 中的默认实现**，以允许应用程序提供自定义版本。
    

在 GCC 和 Clang 中，可以使用 `__attribute__((weak))` 标注弱符号：

void* operator new(std::size_t size) __attribute__((weak));
void operator delete(void* ptr) __attribute__((weak));

在C语言中，函数和初始化的全局变量（包括显示初始化为0）是强符号，未初始化的全局变量是弱符号。

对于它们，下列三条规则使用：

① 同名的强符号只能有一个，否则编译器报"重复定义"错误。

② 允许一个强符号和多个弱符号，但定义会选择强符号的。


③ 当有多个弱符号相同时，[链接器](https://zhida.zhihu.com/search?content_id=238104535&content_type=Article&match_order=1&q=%E9%93%BE%E6%8E%A5%E5%99%A8&zhida_source=entity)选择占用内存空间最大的那个
下面一段话摘自 wikipedia：

In computing, a weak symbol is a symbol definition in an object file or dynamic library that may be overridden by other symbol definitions. Its value will be zero if no definition is found by the loader.
###  3FS项目

3FS项目实现了一个灵活的自定义内存分配系统，通过重载全局的new/delete操作符，将内存分配重定向到自定义的分配器
 
 代码位置：

src\memory\common\GlobalMemoryAllocator.h
src\memory\common\OverrideCppNewDelete.h


```
// Override global new/delete with custom memory allocator.
void *operator new(size_t size) { return hf3fs::memory::allocate(size); }

inline void *allocate(size_t size) { return std::malloc(size); }

个文件通过条件编译宏 OVERRIDE_CXX_NEW_DELETE 控制是否启用自定义内存分配器。当启用时，全局的new和delete操作符会被重定向到 hf3fs::memory 命名空间中的 allocate 和 deallocate 函数。

```
```cpp

#define GET_MEMORY_ALLOCATOR_FUNC_NAME "getMemoryAllocator"

namespace hf3fs::memory {
class MemoryAllocatorInterface {

 public:

  virtual ~MemoryAllocatorInterface() = default;

  virtual void *allocate(size_t size) = 0;

  virtual void deallocate(void *mem) = 0;

  virtual void *memalign(size_t alignment, size_t size) = 0;

  virtual void logstatus(char *buf, size_t size) = 0;

  virtual bool profiling(bool active, const char *prefix) = 0;

};


using GetMemoryAllocatorFunc = MemoryAllocatorInterface *(*)();

}  // namespace hf3fs::memory
```

Let me explain this line of code:

```cpp
GetMemoryAllocatorFunc func = (GetMemoryAllocatorFunc)::dlsym(lib, "getMemoryAllocator");
```

This is a dynamic library function loading operation that consists of several parts:

1. **dlsym function**:
   - `::dlsym` is a POSIX function for loading symbols from dynamic libraries
   - First parameter `lib` is the handle to the loaded dynamic library
   - Second parameter `"getMemoryAllocator"` is the symbol name to look up

2. **Type casting**:
   - `(GetMemoryAllocatorFunc)` casts the void pointer returned by dlsym to our function pointer type
   - This cast is necessary because `dlsym` returns `void*`

3. **Function pointer assignment**:
   - The casted function pointer is assigned to `func`
   - After this, `func` can be called like a normal function to get the memory allocator instance

Example usage:
```cpp
// Load the library
void* lib = dlopen("memory_allocator.so", RTLD_NOW);
if (lib) {
    // Get the function pointer
    GetMemoryAllocatorFunc func = (GetMemoryAllocatorFunc)::dlsym(lib, "getMemoryAllocator");
    if (func) {
        // Call the function to get allocator instance
        MemoryAllocatorInterface* allocator = func();
    }
}
```

This mechanism allows 3FS to:
- Load custom memory allocators at runtime
- Switch between different allocator implementations without recompiling
- Maintain a clean separation between the core system and specific allocator implementations



让我通过一个具体的例子来说明这个函数指针类型的使用：

```cpp:d:\工作记录\code\money\3FS\src\memory\jemalloc\JemallocAllocator.h
#pragma once
#include "memory/common/MemoryAllocatorInterface.h"

namespace hf3fs::memory {

// 自定义内存分配器实现
class JemallocAllocator : public MemoryAllocatorInterface {
public:
    void *allocate(size_t size) override {
        return je_malloc(size);  // 使用jemalloc的分配函数
    }

    void deallocate(void *mem) override {
        je_free(mem);  // 使用jemalloc的释放函数
    }

    void *memalign(size_t alignment, size_t size) override {
        return je_aligned_alloc(alignment, size);
    }

    void logstatus(char *buf, size_t size) override {
        snprintf(buf, size, "Jemalloc allocator status");
    }

    bool profiling(bool active, const char *prefix) override {
        return active;
    }
};

// 导出函数实现
extern "C" {
    // 这个函数的类型就是 GetMemoryAllocatorFunc
    MemoryAllocatorInterface* getMemoryAllocator() {
        static JemallocAllocator allocator;
        return &allocator;
    }
}

} // namespace hf3fs::memory
```

使用示例：

```cpp:d:\工作记录\code\money\3FS\src\memory\jemalloc\example_usage.cpp
#include "JemallocAllocator.h"
#include <dlfcn.h>

void example() {
    // 1. 直接使用函数指针类型
    GetMemoryAllocatorFunc allocatorFunc;
    
    // 2. 从动态库加载函数
    void* lib = dlopen("libjemalloc_allocator.so", RTLD_NOW);
    if (lib) {
        // 将动态库中的符号转换为函数指针
        allocatorFunc = (GetMemoryAllocatorFunc)dlsym(lib, GET_MEMORY_ALLOCATOR_FUNC_NAME);
        
        if (allocatorFunc) {
            // 3. 调用函数获取分配器实例
            MemoryAllocatorInterface* allocator = allocatorFunc();
            
            // 4. 使用分配器
            void* memory = allocator->allocate(1024);  // 分配1024字节
            allocator->deallocate(memory);             // 释放内存
        }
        
        dlclose(lib);
    }
}
```

这个例子展示了：

1. **函数指针类型定义**：`GetMemoryAllocatorFunc` 定义了一个返回 `MemoryAllocatorInterface*` 的函数指针类型

2. **函数实现**：`getMemoryAllocator` 函数符合这个类型定义，它返回一个分配器实例

3. **动态加载**：展示了如何从动态库中加载并使用这个函数

4. **实际使用**：展示了如何通过函数指针获取和使用分配器实例

这种设计模式允许：
- 在运行时动态加载不同的内存分配器
- 保持接口统一性
- 实现插件式的内存分配器架构
- 方便进行内存分配器的切换和测试


对于operator new 来说，分为**全局重载**和**类重载**，全局重载是void* ::operator new(size_t size)，


# 拆分问题2： 磁盘读写

## 1. 问题是什么？如何 异步I/O提高性能

 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题 

 二、这个技术的优势和劣势分别是什么 

三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比


## 2. 为了解决这个问题准备哪些事情




### 文章

- [译] Linux 异步 I_O 框架 io_uring：基本原理、程序示例与性能压测（2020）

既然性能跟传统 AIO 差不多，那为什么还称 `io_uring` 为革命性技术呢？

1. 它首先和最大的贡献在于：**统一了 Linux 异步 I/O 框架**，
    
    - Linux AIO **只支持 direct I/O** 模式的**存储文件** （storage file），而且主要用在**数据库这一细分领域**；
    - `io_uring` 支持存储文件和网络文件（network sockets），也支持更多的异步系统调用 （`accept/openat/stat/...`），而非仅限于 `read/write` 系统调用。
- https://github.com/axboe/liburing/blob/liburing-2.0/examples/io_uring-test.c


### 项目：看别人怎么使用
- ceph bluestore  支持 io_uring  提升多少
https://github.com/ceph/ceph/pull/27392

``` shell

4k  +14%
8k  +42%
16k  +59%
32k  +89%
64k  +85%
128k  +102%

```

- **redis**：

以下是 redis 在 event poll 和 io_uring 下的 qps 对比：

1. 高负载情况下，io_uring 相比 event poll，吞吐提升 8%~11%。
2. 开启 sqpoll 时，吞吐提升 24%~32%。 这里读者可能会有个疑问，开启 sqpoll 额外使用了一个 CPU，性能为什么才提升 30% 左右？那是因为 redis 运行时同步读写就消耗了 70% 以上的 CPU，而 sq_thread 只能使用一个 CPU 的能力，把读写工作交给 sq_thread 之后，理论上 QPS 最多能提升 40% 左右（1/0.7 - 1 = 0.42），再加上 sq_thread 还需要处理中断以及本身的开销，因此只能有 30% 左右的提升。

- IO-uring speed the RocksDB & TiKV

## 3  结果是什么






# 拆分问题： 协程序

https://wx.zsxq.com/search/%E5%86%85%E5%AD%98%E6%B1%A0?groupId=51122582242854&searchUid=0.1545150984662378


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250328153322.png)


## 参考
- 《Thriving in a Crowded and Changing World: C++ 2006–2020》 C++ 之父的这篇论文，详细描写了 2006 - 2020 这些年 C++ 的变化过程，值得一看。
-  LD_PRELOAD的偷梁换柱之能
- https://gcc.gnu.org/onlinedocs/libstdc++/faq.html
- https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/files.html
- https://stackoverflow.com/questions/1885849/difference-between-new-operator-and-operator-new
- [深入了解GOT,PLT和动态链接](https://www.cnblogs.com/pannengzhi/p/2018-04-09-about-got-plt.html "发布于 2018-04-09 16:52")
- https://zhuanlan.zhihu.com/p/372151448
- 人人都能学的会C++协程原理剖析与自我实现
- 程序员的自我修养--链接、装载与库
- - 深入解析dlsym函数调用：探秘计算机系统的神奇之处！
- https://wx.zsxq.com/group/51122582242854/topic/418811812251528
- C++ 深入解析new关键字，::new、operator new函数，placement new表达式 
- https://blog.csdn.net/aishuirenjia/article/details/102979457 ✅
- [ Linux 异步 I_O 框架 io_uring：基本原理、程序示例与性能压测（2020）.md ✅
- 【NO.327】图解｜揭开协程的神秘面纱.md

虽然异步+回调在现实生活中看着也很简单，但是在程序设计上却很让人头痛，在某些场景下会让整个程序的可读性非常差，而且也不好写，相反同步IO虽然效率低，但是很好写，
[![img](https://camo.githubusercontent.com/1e84343c42aa960b3762da9101c8be462f2fbe20383bb5b13f05ff7d5bf63482/68747470733a2f2f706963322e7a68696d672e636f6d2f38302f76322d39613032613530313236373236336161353036623430396239653039646337355f373230772e77656270)](https://camo.githubusercontent.com/1e84343c42aa960b3762da9101c8be462f2fbe20383bb5b13f05ff7d5bf63482/68747470733a2f2f706963322e7a68696d672e636f6d2f38302f76322d39613032613530313236373236336161353036623430396239653039646337355f373230772e77656270)

还是以为异步图片下载为例，图片服务中台提供了异步接口，发起者请求之后立即返回，图片服务此时给了发起者一个唯一标识ID，等图片服务完成下载后把结果放到一个消息队列，此时需要发起者不断消费这个MQ才能拿到下载结果。

整个过程相比同步IO来说，**原来整体的逻辑被拆分为好几个部分，各个子部分有状态的迁移，对大部分程序员来说维护状态简直就是噩梦，日后必然是bug的高发地**。


## About

2023年最新整理 c++后端开发，1000篇优秀博文，含内存，网络，架构设计，高性能，数据结构，基础组件，中间件，分布式相关
https://github.com/0voice/cpp_backend_awsome_blog/tree/main