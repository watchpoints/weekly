---
title: 深入浅出理解DeepSeek 3FS (3) 步步引导轻松理解内存管理，面试必看
date: 2025-03-31
description: 面试之美
draft: false
tags:
  - 面试之美
  - 架构之美
  - 源码之美
---

🌟 大家好，我是小王同学，

今天带你们一起探秘内存分配的奥秘！

本文主要描述了如何为一个类自定义new实现

1. 大胆猜测，小心求证过程中  
      
    C++的new实现其实是分层设计的：  
    ➤ 第一层：调用C++标准库中的operator new  
    ➤ 第二层：operator new内部调用C语言标准库中的malloc  
    ➤ 第三层：最终由操作系统提供接口完成实际内存分配  
    
    
2. 模块化替换  
    🔧 在实际开发中，灵活替换默认内存分配方案非常关键：  
    ➤ 链接阶段：C++标准库中的operator new被声明为弱符号，用户只要自定义同名的强符号，就能在链接时自动替换！  
    ➤ 运行阶段：通过设置LD_PRELOAD环境变量，可以让自定义的malloc函数以更高的优先级替换原有实现。  
    这种模块化替换机制不仅让开发者能随时调控内存分配策略，更带来无限可能！✨
    
3. 3FS 怎么实现的  
    🚀 3FS的实现方式相当直观：  
    ➤ 通过定义OVERRIDE_CXX_NEW_DELETE编译开关，默认情况下采用operator new调用malloc；  
    ➤ 如果开启开关，自定义分配器，
	- 可能自定义实现的，
	- 可能第三方库的Jemalloc，TCMalloc
    ➤ 项目中定义了一个统一的全局变量gAllocator，用于管理所有内存分配接口；  
    ➤ 对于第三方分配器，通过dlsym动态加载函数地址（如getMemoryAllocatorFunc = (GetMemoryAllocatorFunc)::dlsym(mallocLib, GET_MEMORY_ALLOCATOR_FUNC_NAME)），
    实现了灵活且扩展性极强的内存管理方案。  
    这种设计既清晰又充满智慧，让内存管理不再神秘，而是充满了DIY的乐趣！
    理解如何深刻 ，这也是为选择看3fs原因，其他项目可不会这么清晰

Happy Coding～💖

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401152333.png)

## 一 越老生常谈，越难超出期望，换个问法就不会了

### 故事的背景 

工作8年的小王 同学 为了准备c++面试，
很早 就从脉脉上搜索过类似题目，信心满满而去，
什么返回值区别，重载区别？还有函数用法区别？


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331214604.png)


结果吊打一顿回来，在中午吃饭的时候，被老王看到了。

哈哈哈，又拒绝一家公司是对吧？
来说说这次发生了什么，有什么意想不到事情。
### 当时对话是这样的


面试官：请说说 new 和malloc区别？

小王脑中2小人开始不假思索推理 开来？
- 操作系统内存管理这个我不熟悉呀？网上看过很多文章和书籍讲虚拟内存看到迷糊，怎么回答。
- new 有什么可回答的， 分配内存，调用构造函数，10年前我知道了

于是我回答了

new 是运算符，运算符就支持重载，因为operator new 。。。。（这个回答是错误的）

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401092720.png)

老王:


从你回答慢了 2-3秒 速度来看 ，
你思路产生混乱，被题目本身限制住了
更没有和日常开发工作有效结合起来。
- 操作系统层面事情，内存管理事情，**你不会，别人不会，不需要在这个方面思考**
- new 实现过程 ，但是还是停留在10年前，这期间没看到你进步
**你可能了解，别人也了解事情 你需要探索背后原理**

> 划重点：
你知道，别人也知道，你了解还是10年前认知。没长进呀。 
这个才是重点探索内容。


## 二、大胆猜测，小心求证


### 1. 问题能描述清楚吗？
- new和malloc 区别

- 一个类如何定制自己的new和delete，代替默认的 new 和delete？（这个才是真正马甲）

>小提示

- new 是怎么调用operator new，是全局的operator new函数，还是每个类的operator new函数  ？


### 2. 为了解决这个问题准备哪些事情(技术路线)

#####  step1  c++学习路线和资料（想要电子书关注 回复 电子书）

-  【Effective C++】条款49~52：定制new和delete 
-  More Effective C++  Item 8: Understand the different meanings of new
    and delete
#####  step2 参考源码

（1）C libstdc++**: https://gcc.gnu.org/libstdc++/
    
- **LLVM libc++**: https://libcxx.llvm.org/

在标准库实现中，你可以找到 `std::allocator`、`std::unique_ptr` 和 `std::shared_ptr` 的实现方式。

### step3 **开源项目**

一些高性能的 C++ 项目也有自定义 `new/delete` 的实现，你可以参考它们的代码：

- **Google TCMalloc**（高性能内存分配器）  
- oceanbase
- 3FS



开始 干活


演示代码：


```c++
https://github.com/watchpoints/master-cpp/blob/main/cpp/03_new.cpp

#include <iostream>
class Foo
{
public:
     int number;
     Foo(int n)
     {
       number = n;
     }
};
int main() {
    Foo* ptr = new Foo(10);
    return 0;
}

```


### 小王疑问1：new语法 :是c++语法的高级抽象，既然抽象根本不知道内部怎么实现的？

老王说：借助https://godbolt.org/z/YWcecfG7b 这个工具


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401000058.png)

看不懂汇编没关系，直接让大模型来解释

以下是添加了详细注释的汇编代码，解释每一条指令的作用：

```assembly
__cxx_global_var_init:
        push    rbp                  # 1. 保存当前栈帧指针
        mov     rbp, rsp              # 2. 设置新的栈帧
        sub     rsp, 32               # 3. 在栈上分配 32 字节的局部变量空间
        mov     edi, 4                # 4. 设置 operator new 的参数，分配 4 字节的空间
        call    operator new(unsigned long)@PLT  # 5. 调用 operator new 进行内存分配
        mov     rdi, rax              # 6. 将分配的地址存入 rdi（作为 Foo::Foo(int) 构造函数的 this 指针）
        mov     rax, rdi              # 7. 复制 rdi 到 rax
        mov     qword ptr [rbp - 24], rax  # 8. 将 this 指针存入栈中的局部变量（用于后续操作）
        mov     esi, 10               # 9. 设置 Foo::Foo(int) 构造函数的参数，传递值 10
        call    Foo::Foo(int) [base object constructor]  # 10. 调用 Foo 类的构造函数
        jmp     .LBB0_1               # 11. 跳转到 .LBB0_1 继续执行后续逻辑
```

 call 指令 就是调用函数
 
new 实现调用2个函数

1. 内存分配 调用全局的 operator new 函数为对象分配足够大小的内存。（4字节）
    
2. 调用 Foo构造对象 
    
3. 返回对象指针 构造完毕后返回分配并初始化后的对象指针。


>  划重点：
More Effective C++ Item 8: Understand the different meanings of new and delete.


This operator is built into the language and, like sizeof, you can’t change its meaning: it always does the same thing. 

What it does is twofold.
- First, it allocates enough memory to hold an object of the type requested.

- Second, it calls a constructor to initialize an object in the memory that was allocated. 

The new operator always does those two things; you can’t change its behavior in any way
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401002243.png)


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401002708.png)

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401002949.png)




### 小王疑问2：new 是c++语法  operator new是一个函数？new 内部实现自动operator new函数？他们直接关系是什么



老王：根据上面分析结果 不妨大胆猜测一下。



+----------------------+
| new 关键字 (C++)    |  [libstdc++.so]
| 调用 operator new   |
+----------------------+
        |
        v
+----------------------+
| std::operator new   |  [libstdc++.so]
| (分配内存, 调用 malloc) |
+----------------------+
        |
        v
+----------------------+
| malloc() (glibc)    |  [libc.so]
| 分配内存  根据大小判断     |
+----------------------+
        |
  +-----+------+
  |            |
  v            v
+----------------------+   +----------------------+
| brk() (堆扩展)       |   | mmap() (大块内存)   |
| size ≤ 128 KB        |   | size > 128 KB        |
| [syscall to kernel]  |   | [syscall to kernel]  |
+----------------------+   +----------------------+
        |                           |
        v                           v
+----------------------+   +----------------------+
| Linux 内核管理      |   | Linux 内核管理      |
| 分配物理内存         |   | 分配独立映射区域    |
+----------------------+   +----------------------+


 **各函数对应动态库**

| 函数                  | 作用                  | 所在动态库               |
| ------------------- | ------------------- | ------------------- |
| `new`               | 申请对象并调用构造函数         | `libstdc++.so`      |
| `std::operator new` | 内存分配函数（调用 `malloc`） | `libstdc++.so`      |
| `malloc`            | 申请动态内存              | `libc.so`           |
| `brk`               | 调整堆（小内存）            | `libc.so` (syscall) |
| `mmap`              | 申请大块内存（独立映射）        | `libc.so` (syscall) |
| `free`              | 释放内存                | `libc.so`           |




### 小王疑问3：我还是不太相信new语法，用到libstdc++，这个不是一体的吗？

请 回到 演示代码部分，编译代码 ldd 查看，new 确实调用 libstdc++.so.6


ldd a.out
        linux-vdso.so.1 (0x00007fff6f9c0000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f5117c52000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f5117a29000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f5117942000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f5117e8c000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f511792200


老王：格局没打开  看看这个

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094005.png)
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094054.png)
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094204.png)

### 小王疑问4：g++ 命令 编译时候为什么默认自动连接libstdc++库吗？



简单来说，GCC是编译器，它负责将C++源代码编译成可执行程序。
在这个过程中，GCC会链接到Libc和Libstdc++这两个库。
Libc为GCC提供了底层的C语言接口，
而Libstdc++则为GCC提供了C++标准库的实现

libc是Linux下原来的标准C库，也就是当初写hello world时包含的头文件#include < stdio.h> 定义的地方。

后来逐渐被glibc取代

那glibc都做了些什么呢？ **glibc是Linux系统中最底层的API，几乎其它任何的运行库都要依赖glibc**。 glibc最主要的功能就是对系统调用的封装，你想想看，你怎么能在C代码中直接用fopen函数就能打开文件？ 打开文件最终还是要触发系统中的sys_open系统调用，而这中间的处理过程都是glibc来完成的。

- glibc是运行时库（Runtime Library）
- **对于系统级别的事件，libstdc++首先是会与glibc交互，才能和内核通信**。相比glibc来说，libstdc++就显得没那么基础了。

| **特性**       | **glibc （libc）**          | **libc++**               | **libstdc++**            |
| ------------ | ------------------------- | ------------------------ | ------------------------ |
| **用途**       | 提供 C 语言标准库实现              | 提供 C++ 标准库实现             | 提供 C++ 标准库实现             |
| **支持平台**     | 主要用于 Linux 操作系统           | 主要与 Clang 编译器配合使用        | 主要与 GCC 编译器配合使用          |
| **内容**       | 包括所有 C 标准库函数，操作系统接口、线程支持等 | 提供 C++ 标准库的模板库（STL）、IO流等 | 提供 C++ 标准库的模板库（STL）、IO流等 |
| **库功能**      | 系统调用、内存管理、线程库、文件操作等       | C++ STL，算法，IO流等          | C++ STL，算法，IO流等          |
| **重入性与线程安全** | 提供线程安全的库支持，包含锁等线程控制功能     | 提供线程安全支持（但不一定是内建的）       | 提供线程安全支持，包含多线程功能         |



自然会：
在 C++ 开发中，大多数程序都会使用标准库。**`g++` 自动链接 `libstdc++`**，（你不说我就不知道的事情）
可以减少用户的额外操作，提高编译的便捷性，同时避免因忘记手动链接库而导致的错误。因此，这是一种用户友好的设计选择




- 动手实验：使用gcc编译cpp代码，gcc默认不使用c++标准库libstdc++

```cpp
 如果使用 `gcc` 编译 C++ 代码（不推荐），
就需要手动添加 `-lstdc++` 选项来链接标准库

gcc 03_new.cpp 
 
/usr/bin/ld: /tmp/ccZKC4VY.o: in function `main':
03_new.cpp:(.text+0x13): undefined reference to `operator new(unsigned long)'

gcc 03_new.cpp  -lstdc++(需要手工制定)


```

- 动手实验：使用g++命令编译cpp代码，默认链接c++标准库 

g++ -v test.cpp -o test
COLLECT_GCC=g++
...
 /usr/lib/gcc/x86_64-linux-gnu/9/collect2 ...
 ... -lstdc++ ...


### 小王疑问5: 我相信了 new 与 `std::operator new` 有关系， 但是他真实一个函数吗？

operator是C++的关键字，它和运算符一起使用，表示一个运算符函数。

再次表示 new 运算符是不可以重载的，网上说错误的。

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401092720.png)

代码：

https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/libsupc++/new_op.cc


```c++
// A freestanding C runtime may not provide "malloc" -- but there is no
// other reasonable way to implement "operator new".
// 一些独立的 C 运行时环境可能不提供 "malloc"，但没有其他合理的方法来实现 "operator new"。
// 因此，这里直接声明 `malloc` 函数，保证后续调用不会出错。
extern "C" void *malloc (std::size_t);

#endif

// `operator new` 的弱定义版本，可以被用户自定义版本覆盖。
// 当 new 关键字被调用时，会执行此函数以分配内存。
_GLIBCXX_WEAK_DEFINITION void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)
{
  void *p;

  // `malloc(0)` 在不同的 C 标准库实现中可能会有不确定的行为（返回 nullptr 或者有效指针）。
  // 为了避免这种不确定性，如果 `sz == 0`，则将其设为 1，确保 `malloc` 至少分配 1 字节。
  if (__builtin_expect (sz == 0, false))
    sz = 1;

  // 循环尝试调用 `malloc`，直到成功分配内存或者 `new_handler` 处理失败
  while ((p = malloc (sz)) == 0) // 如果 `malloc(sz)` 返回 `nullptr`，表示内存分配失败
    {
      // 获取当前的 new_handler（可以通过 `std::set_new_handler` 设定）
      new_handler handler = std::get_new_handler ();
      
      // 如果没有设置 `new_handler`，则直接抛出 `std::bad_alloc` 异常，终止程序。
      if (!handler)
        _GLIBCXX_THROW_OR_ABORT(bad_alloc());
      
      // 调用用户自定义的 `new_handler`，该处理程序可能会尝试释放一些内存，
      // 或者执行其他策略（如记录日志、延迟执行），以便下一次 `malloc` 可能成功。
      handler ();
    }

  // 成功分配内存，返回指针
  return p;
}

https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/libsupc++/new_op.cc
// ------------------ BEGIN COPY ------------------
// Implement all new and delete operators as weak definitions
// in this shared library, so that they can be overridden by programs
// that define non-weak copies of the functions.

static void* operator_new_impl(std::size_t size) {
  if (size == 0)
    size = 1;
  void* p;
  while ((p = std::malloc(size)) == nullptr) {
    // If malloc fails and there is a new_handler,
    // call it to try free up memory.
    std::new_handler nh = std::get_new_handler();
    if (nh)
      nh();
    else
      break;
  }
  return p;
}


```


>  划重点: **为什么 `new` 被视为高级抽象？,operator new 人为第水平管理内存方式

1. **内存管理的封装**：
    
    - 在 C 中，如果你想在堆上分配内存，你通常会使用 `malloc` 或 `calloc`，然后需要手动管理内存，包括初始化对象。
        
    - 在 C++ 中，`new` 运算符提供了一个更高级的抽象，**不仅仅是内存分配**，还自动调用对象的构造函数来初始化对象。
        
    - 这种封装隐藏了底层内存管理的细节，简化了内存分配的使用，使开发者不需要关注内存分配与对象初始化的分离。
        
2. **异常安全**：
    
    - 使用 `new` 时，如果内存分配失败，它会抛出一个 `std::bad_alloc` 异常，这使得开发者不需要显式地检查 `malloc` 返回的 `NULL` 值。这样的异常处理提供了一种更加安全和优雅的内存管理方式。
        
3. **对内存分配的透明管理**：
    
    - 虽然底层的 `operator new` 函数可以被重载，但 C++ 语言本身提供了透明的内存分配接口，允许开发者使用 `new` 运算符而不关心内存分配的细节。这种抽象化让开发者集中于高层次的程序逻辑，而不是低级的内存管理。

https://en.cppreference.com/w/cpp/memory
https://en.cppreference.com/w/cpp/memory/new

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250327203824.png)


| **特性**    | `malloc` **(函数)**  | `new` **(运算符)**            |
| --------- | ------------------ | -------------------------- |
| 是否是函数？    | ✅ 是普通函数            | 不是函数，是运算符                  |
| 是否调用构造函数？ | 不会调用               | ✅ 会调用构造函数                  |
| 返回类型      | `void*`，需要强制转换     | 直接返回正确的指针类型                |
| 是否可以重载？   | ❌ 不能重载             | ✅ `operator new` 可以被重载     |
| 是否支持类型安全？ | ❌ 需要手动转换类型         | ✅ 自动匹配类型                   |
| 释放方式      | `free(ptr);`       | `delete ptr;`              |
| 失败时返回     | `nullptr` / `NULL` | **抛出异常**（`std::bad_alloc`） |


### 小王疑问6：  call    operator new(unsigned long)@PLT   PLT 是什么？ operator new 函数 weak definitions 又是什么


老王： 

![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094558.png)


推荐看一本书 程序员的自我修养—链接、装载与库 补一补。
![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401105728.png)


最喜欢一句话
Any problem in computer science can be solved by anotherlayer of indirection.”
  
  这句话几乎概括了计算机系统软件体系结构的设计要点，整个体系结构从上到下都是按照严格的层次结构设计的
  ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401110103.png)

##### @PLT 含义是什么

  
PLT 是 ELF 文件中的一块代码区域，用于动态链接时的函数调用延迟绑定。

也就是说，当程序第一次调用一个外部函数（如来自共享库的函数）时，
实际调用的是 PLT 中的一个“跳板”入口，经过解析后再跳转到真正的函数地址；

而后续调用则直接通过更新过的 GOT 表项（全局偏移表）跳转到目标函数。




##### 动手验证：例子越简单越好

假设有下面的 C 程序

```
#include <stdio.h>

int main() {
    printf("Hello, PLT!\n"); //使用libc这个动态库
    return 0;
}
```


当你编译这个程序（使用动态链接库，例如 libc）后，编译器不会直接把 printf 的真实地址写进代码中，而是生成一条调用“printf@PLT”的指令。

符号（Symbol）这个概念随着汇编语言的普及迅速被使用，它用来表示一个地址，这个地址可能是一段子程序（后来发展成函数）的起始地址，也可以是一个变量的起始地址。


也就是说，程序在运行时会先跳转到 PLT 表中的一个“跳板”入口，而不是直接跳转到 libc 中的 printf 函数。


#动态链接中PLT与GOT工作流程

1. 第一次调用时（延迟绑定）： 当程序第一次调用 printf 时，实际执行的是 printf@PLT 中的代码。该代码的第一条指令通常是跳转到 printf 对应的 GOT 表项（Global Offset Table，全球偏移表）。由于这是第一次调用，这个 GOT 表项里并没有存储真实的 printf 地址，而是保存了一个指向特殊解析代码的地址（通常会指向 PLT 的第一项，也称为共公 PLT 表项）。
    
2. 解析函数地址： 随后，这个特殊的解析代码会调用动态链接器（如 _dl_runtime_resolve），根据传入的信息（例如函数的编号、模块信息等），找到真正的 printf 地址，并将这个地址写入到 GOT 表中对应的 printf 表项。
    
3. 后续调用直接跳转： 此后，再次调用 printf 时，程序跳转到 printf@PLT，这时通过 GOT 表查到的地址已经是解析后的 printf 真实地址，直接跳转过去执行，从而省去了再次解析的步骤


>大局观

首先, 我们要知道, GOT和PLT只是一种重定向的实现方式. 所以为了理解他们的作用, 就要先知道什么是重定向, 以及我们为什么需要重定向


比如某个公司开发完成了某个产品，它按照一定的规则制定好程序的接口，其他公司或开发者可以按照这种接口来编写符合要求的动态链接文件。

该产品程序可以动态地载入各种由第三方开发的模块，在程序运行时动态地链接，实现程序功能的扩展


## **弱符号的用途**

弱符号主要用于：

1. **库函数的默认实现**
    
    - `operator new` 就是一个典型案例，标准库提供 **默认的弱定义**，用户可以 **提供自己的强定义** 进行替换。
        
2. **可选的全局变量**
    
    - 在某些场景下，一个库可能会提供一个 **默认变量**，但允许应用程序提供自己的变量：
        


        

## 三、这才是刚刚开始，3FS还到呢，怎么为类定制的new操作



**动手验证**：

```c++
#include <iostream>
#include <cstdlib>

// 自定义 operator new（强定义）
void* operator new(std::size_t size) {
    std::cout << "Custom operator new called, size: " << size << std::endl;
    return malloc(size);
}

int main() {
    int* p = new int; // 这里会调用自定义的 operator new
    delete p;
    return 0;
}

 输出

Custom operator new called, size: 4

- `operator new` 在标准库中是弱符号（`__attribute__((weak))`）。
    
- 当程序员提供了自己的 `operator new`，它作为 **强符号** 替换了标准库的实现。
    
- 因此，`new` 关键字调用的是自定义的 `operator new`，而不是标准库的版本。
```


阅读代码：3FS\src\memory\common\OverrideCppNewDelete.h
```c++
// Override global new/delete with custom memory allocator.
// 用强符号代替标准库的弱符号
void *operator new(size_t size) { return hf3fs::memory::allocate(size); }

void operator delete(void *mem) noexcept { hf3fs::memory::deallocate(mem); }


inline void *allocate(size_t size) { return std::malloc(size); }
  
```



## 四、彩蛋呢，没有彩蛋就不算惊喜


###  小王疑问8：malloc 也能替换吗
上面三个章节 解释了 
int* ptr =new int(10) 这个高级抽象 

调用c++标准库 operaotr new函数-->c语言标准库 malloc --glibc 和os底层先不考虑。


``` shell
+----------------------+
| new 关键字 (C++)    |  [c++语法]
| 调用 operator new   |
+----------------------+
        |
        v
+----------------------+
| std::operator new   |  [libstdc++.so]
| (分配内存, 调用 malloc) |
+----------------------+
        |
        v
+----------------------+
| malloc() (glibc)    |  [glibc.so]
| 分配内存  根据大小判断     |
+----------------------+
        |
  +-----+------+
  |            |
  v            v
+----------------------+   +----------------------+
| brk() (堆扩展)       |   | mmap() (大块内存)   |
| size ≤ 128 KB        |   | size > 128 KB        |
| [syscall to kernel]  |   | [syscall to kernel]  |
+----------------------+   +----------------------+
        |                           |
        v                           v
+----------------------+   +----------------------+
| Linux 内核管理      |   | Linux 内核管理      |
| 分配物理内存         |   | 分配独立映射区域    |
```




C++ `new` 操作的完整调用路径




```c++
new 操作
 ├── operator new(size)       // 调用标准库中的 operator new
 │    ├── malloc(size)       // 调用 C 语言的 malloc
 │    │    ├── _int_malloc() // glibc 内部分配
 │    │    │    ├── sbrk()   // 小块分配（扩展 heap）
 │    │    │    ├── mmap()   // 大块分配（独立映射）
 │    │    └── 返回分配的内存地址
 │    └── 返回指针
 ├── 构造函数初始化对象
 └── 返回对象指针

```


代码位置：
https://github.com/lattera/glibc/blob/master/malloc/malloc.c



老王：

在使用GCC编译器时，如果不想工程使用系统的库函数，
例如在自己的工程中可以根据选项来控制是否使用系统中提供的malloc/free函数，可以有两种方法：

(1). 使用LD_PRELOAD环境变量：可以设置共享库的路径，并且该库将在任何其它库之前加载，即这个**动态库中符号优先级是最高的**。

(2). 使用GCC的–wrap选项：


下面用简单的语言解释一下 GCC 的 –wrap 选项是如何工作的，以及如何用它来替换（拦截）一个函数，比如 malloc：

---

### 1. 什么是 –wrap 选项？

- **基本概念：**  
    `--wrap=symbol` 是一个链接选项。它的作用是在链接阶段“拦截”对某个函数（symbol）的调用，让这些调用转而去调用另一个函数（包装函数）。

Use a wrapper function for symbol. Any undefined reference to symbol will be resolved to `__wrap_`symbol
    
- **如何实现：**  
    当你使用 `--wrap=malloc` 时，所有对 `malloc` 的调用都会被重定向到 `__wrap_malloc`。这就好像你给 `malloc` 装上了一个“包装纸”，让调用者实际使用的是包装后的版本。
    

---

### 2. 包装函数（wrapper function）的工作机制

- **实际操作：**
    
    - **重定向调用：**  
        当程序中调用 `malloc` 时，链接器把它转换成对 `__wrap_malloc` 的调用。
        
    - **调用原始函数：**  
        如果你在你的包装函数里需要调用真正的 `malloc`，你可以直接调用 `__real_malloc`。链接器会把 `__real_malloc` 转换回真正的 `malloc` 实现。
        
- **简单示例：**  
    假设我们想对 `malloc` 进行包装：
    
    ```c
 // wrap.c  
#include <stdio.h>  
#include <stdlib.h>  
void* __real_malloc(size_t size); // 只声明不定义__real_malloc  
void* __wrap_malloc(size_t size) // 定义__wrap_malloc  
{  
    printf("__wrap_malloc called, size:%zd\n", size); // log输出  
    return __real_malloc(size); // 通过__real_malloc调用真正的malloc  
}

下面是测试用例：

// test.c  
#include <stdio.h>  
#include <stdlib.h>  
int main()  
{  
    char* c = (char*)malloc(sizeof(char)); // 调用malloc  
    printf("c = %p\n", c);  
    free(c); // 调用free，防止内存泄漏  
    return 0;  
}
下面是编译链接过程：

gcc -c wrap.c test.c  
gcc -Wl,--wrap,malloc -o test wrap.o test.o // 链接参数-Wl,--wrap,malloc12

结果查看：  
./test
```
    


### 3. C++ 中的特殊注意

- **C++ 名字修饰问题：**  
    如果你用 C++ 来实现包装函数（比如 __wrap_malloc），一定要在函数声明前加上 `extern "C"`。  
    这样做可以防止 C++ 的名字修饰（name mangling），确保链接器能正确找到 `__wrap_malloc`。  
    示例：
    
    ```cpp
    extern "C" void* __wrap_malloc(size_t size) {
        // 包装逻辑
        return __real_malloc(size);
    }
    ```
    

---



## 3FS 是怎么做的

- 定义宏开关
CMakeLists.txt
option(OVERRIDE_CXX_NEW_DELETE "Override C++ new/delete operator" OFF)

- 宏开关开启不同特性（c语言没有c++函数重载这个特性）
  
```c
src\memory\common\OverrideCppNewDelete.h

#ifdef OVERRIDE_CXX_NEW_DELETE
void *allocate(size_t size)
#else
inline void *allocate(size_t size) { return std::malloc(size); }

```

- allocate 如何实现加载不同的内存分配器

```c 
gAllocator = getMemoryAllocatorFunc();


void *allocate(size_t size) {
//这里通过环境变量 MEMORY_ALLOCATOR_LIB_PATH 来指定要加载的内存分配器库。
//例如，如果你想使用jemalloc.tcmalloc：
//set MEMORY_ALLOCATOR_LIB_PATH=D:\path\to\jemalloc.dll
//set MEMORY_ALLOCATOR_LIB_PATH=D:\path\to\tcmalloc.dll
  if (!gAllocatorInited) {
    std::call_once(gInitOnce, loadMemoryAllocatorLib);
    gAllocatorInited = true;
  }
  //- 首次调用内存分配相关函数时触发加载


  void *mem;

  if (gAllocator == nullptr)
    mem = std::malloc(allocateSize);
    // 使用系统默认分配器
  else

    mem = gAllocator->allocate(allocateSize);
    // 使用自定义分配器


```

- 自定义分配器如何实现的

static MemoryAllocatorInterface *gAllocator = nullptr

```cpp
#define GET_MEMORY_ALLOCATOR_FUNC_NAME "getMemoryAllocator"

//这种设计允许在运行时动态切换不同的内存分配器实现，提供了很好的灵活性和可扩展性。
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
//这种设计允许在运行时动态切换不同的内存分配器实现，提供了很好的灵活性和可扩展性。
//这种设计允许在运行时动态切换不同的内存分配器实现，提供了很好的灵活性和可扩展性。
```

代码位置：3FS\src\memory\jemalloc

```c++
class JemallocMemoryAllocator : public MemoryAllocatorInterface
   void *allocate(size_t size) override { return je_malloc(size); }
   bool profiling(bool active, const char *prefix) override


extern "C" {

hf3fs::memory::MemoryAllocatorInterface *getMemoryAllocator() {

  static hf3fs::memory::JemallocMemoryAllocator jemalloc;

  return &jemalloc;
}


class CustomMemoryAllocator : public MemoryAllocatorInterface
void *allocate(size_t size) override {
        // 实现内存分配逻辑
        return malloc(size);
    }
    
// 导出获取分配器的函数
extern "C" MemoryAllocatorInterface* getMemoryAllocator() {
    static CustomMemoryAllocator allocator;
    return &allocator;
}


```



-  如何第三方库获取分配器的实例

 基本概念：https://www.cnblogs.com/Anker/p/3746802.html
```c++


static void loadMemoryAllocatorLib() {

  GetMemoryAllocatorFunc getMemoryAllocatorFunc = nullptr;
  const char *mallocLibPath =  std::getenv("MEMORY_ALLOCATOR_LIB_PATH");
  mallocLib = ::dlopen(mallocLibPath, RTLD_NOW | RTLD_GLOBAL);
  //dlsym 函数每次调用只能查找和返回一个符号（函数或变量）的地址。
  //# 获取函数的运行时地址
  getMemoryAllocatorFunc = (GetMemoryAllocatorFunc)::dlsym(mallocLib, GET_MEMORY_ALLOCATOR_FUNC_NAME);
  //GetMemoryAllocatorFunc
工作流程是：

1. 通过 dlsym 获取函数指针
2. 调用该函数指针获取分配器实例
3. 将实例保存到全局变量 gAllocator [疑问 这个我没看懂 怎么关联的]
4. 后续的内
  
```

要优缺点：

编译时链接 ：

- 优点：运行更快，不需要符号查找
- 缺点：可执行文件更大，更新库需要重新编译
运行时链接 ：

- 优点：可执行文件更小，可以动态切换不同的库实现
- 缺点：有运行时开销，需要处理加载失败的情况
在当前项目中，JemallocLib 本身是作为动态库编译的，但它与 jemalloc 是编译时链接的关系，而 GlobalMemoryAllocator 则是在运行时动态加载 JemallocLib。

生命周期 ：
- 加载：首次使用时
- 使用：程序运行期间
- 卸载：程序退出时通过 shutdown 函数清理

## 链接我 


如果对上面提到c++学习路径 推荐书籍感兴趣 

关注公共号：后端开发成长指南  回复电子书 

如果更进一步交流 添加 微信：wang_cyi


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


我是小王同学，

希望帮你深入理解分布式存储系统3FS更进一步 ，
为了更容易理解设计背后原理，这里从一个真实面试场故事开始的。

### 阅读对象（也是我正在做事情）

#### **1. 目标：冲击大厂，拿百万年薪**

- 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- ![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250401094551.png)


    
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