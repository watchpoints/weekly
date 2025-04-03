---
title: 成为ob贡献者(6):如何阅读Paxos代码 
date: 2024-10-04
description: 自我驱动学习，专业上只能靠自己
categories:
  - oceanbase
---
	  

为了整理思路，文章采用模拟2人对话方式。
本文主要内容

1. libc++ 是LLVM编译器套件的默认C++标准库。
2. libstdc++是GNU编译器套件（GCC）的默认C++标准库
3.  clang 环境变量设置

```shell
export PATH="/usr/local/opt/llvm/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lunwind" 
export CPPFLAGS="-I/usr/local/opt/llvm/include“
```




[compilation SIG] 编译和兼容



## 了解需求： 编译器 连接器 标准库区别

https://oceanbase.github.io/oceanbase/toolchain/

| 操作系统    | 开发语言  | 编译器       | 连接器               | c++标准库    | 集成工具                       |
| :------ | :---- | :-------- | ----------------- | --------- | -------------------------- |
| Linux   | c/c++ | **GCC**   | GNU Linker (ld)   | libstdc++ | gcc/g++                    |
| macOS   | c/c++ | **Clang** | LLVM Linker (lld) | libc++    |                            |
| Windows | c++   | MSVC      | link.exe          |           | Microsoft Visual C++/MinGW |
| 跨平台     | Go    | gc        |                   |           |                            |
| 跨平台     | rust  | rustc     |                   |           |                            |


 理清gcc、 g++,clang, libc,libc++,libstdc++的关系


提问：gcc是编译c语言代码，g++编译cpp代码，有什么区别 或者说c与cpp有什么区别？

- 差别 就是标准库，例如 stl 用到数据结构和算法

gcc hello.cpp  -lstdc++

 如果想使用 `gcc` 指令来编译执行 `C++` 程序，需要在使用 `gcc` 指令时，手动为其添加 `-lstdc++` 选项，表示 `gcc` 在编译 `C++` 程序时可以链接必要的 `C++` 标准库

 该程序中使用了标准库 `<iostream>` 和 `<string>` 提供的类对象，而 `gcc` 默认是无法找到它们的


提问：标准库：libc ，libc++， libstdc++区别？

1. 编译器支持：
 libstdc++是GNU编译器套件（GCC）的默认C++标准库实现，
 而libc++是LLVM编译器套件的默认C++标准库实现。
 因此，libstdc++主要用于GCC编译器，而libc++主要用于LLVM编译器（如Clang）。
- gcc:/usr/lib/gcc/x86_64-linux-gnu/11/libstdc++.so

~~~
ldd a.out 
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f62cd56f000) c++
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f62cd346000) c
~~~

        
1. 标准遵循：libstdc++实现了C++98、C++03、C++11、C++14和C++17标准的大部分功能，而libc++更加注重C++11、C++14和C++17标准的支持，并且在C++20标准中有更好的兼容性。
    libc++ is a new implementation of the C++ standard library, targeting C++11 and above.









# 开发过程

## 任务01: 从hello word 

mac 使用Clang配置VS Code C/C++环境

mac 版本：

~~~shell
sw_vers

ProductName: macOS

ProductVersion: 15.2

BuildVersion: 24C101

问题：
Warning: You are using macOS 12.  
We (and Apple) do not provide support for this old version.
但在今年9月份的一次更新中，Brew 放弃了对 Monterey 的支持
解决：
升级os版本 耗时一个晚上

~~~

installing LLVM on MacOS

~~~
xcode-select --install //大约15分钟
xcode-select -p

/Applications/Xcode.app/Contents/Developer

brew update
xcode-select --install //大约15分钟
brew install llvm //
~~~

查看帮助文档:brew info llvm

~~~
==> llvm: stable 19.1.6 (bottled), HEAD [keg-only]

Next-gen compiler infrastructure

https://llvm.org/

Installed

/usr/local/Cellar/llvm/19.1.6 (8,107 files, 2GB)

Poured from bottle using the formulae.brew.sh API on 2025-01-12 at 08:26:07

From: https://github.com/Homebrew/homebrew-core/blob/HEAD/Formula/l/llvm.rb

License: Apache-2.0 WITH LLVM-exception

==> Dependencies

Build: cmake, ninja, swig

Required: python@3.13, xz, z3, zstd

==> Options

--HEAD

Install HEAD version

==> Caveats

CLANG_CONFIG_FILE_SYSTEM_DIR: /usr/local/etc/clang

CLANG_CONFIG_FILE_USER_DIR: ~/.config/clang

  

LLD is now provided in a separate formula:

brew install lld


We plan to build LLVM 20 with `LLVM_ENABLE_EH=OFF`. Please see:

https://github.com/orgs/Homebrew/discussions/5654


Using `clang`, `clang++`, etc., requires a CLT installation at `/Library/Developer/CommandLineTools`.

If you don't want to install the CLT, you can write appropriate configuration files pointing to your

SDK at ~/.config/clang.

  

To use the bundled libunwind please use the following LDFLAGS:

LDFLAGS="-L/usr/local/opt/llvm/lib/unwind -lunwind"

  

To use the bundled libc++ please use the following LDFLAGS:

LDFLAGS="-L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lunwind"

  

NOTE: You probably want to use the libunwind and libc++ provided by macOS unless you know what you're doing.

  

llvm is keg-only, which means it was not symlinked into /usr/local,

because macOS already provides this software and installing another version in

parallel can cause all kinds of trouble.

  

If you need to have llvm first in your PATH, run:

echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc

  

For compilers to find llvm you may need to set:

export LDFLAGS="-L/usr/local/opt/llvm/lib"

export CPPFLAGS="-I/usr/local/opt/llvm/include"
~~~


编译环境配置：

~~~
echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.bashrc

//这将使你的终端会话能够找到 LLVM 的 `clang` 和 `clang++` 命令。

clang -v

Homebrew clang version 19.1.6

Target: x86_64-apple-darwin24.2.0

Thread model: posix

InstalledDir: /usr/local/Cellar/llvm/19.1.6/bin

Configuration file: /usr/local/etc/clang/x86_64-apple-darwin24.cfg

System configuration file directory: /usr/local/etc/clang


设置编译器标志
To use the bundled libc++ please use the following LDFLAGS:
echo 'export LDFLAGS="-L/usr/local/opt/llvm/lib - L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lunwind"' >> ~/.bashrc
echo 'export CPPFLAGS="-I/usr/local/opt/llvm/include"' >> ~/.bashrc
source ~/.bashrc

~~~

How to use LDFLAGS in makefile

Makefile选项CPPFLAGS ：
- 指定头文件（.h文件）的路径 
- CPPFLAGS 可以 用于 C 和 C++ 两者。
- gcc参数 -I/usr/include ，大写的I

Makefile选项LDFLAGS
- 指定库文件的位置
- gcc参数LDFLAGS=-L/usr/lib 大写L
- **LIBS**：告诉链接器要链接哪些库文件，如LIBS = -lpthread -liconv 小写l


##  gcc 编译 hello world代码过程


![gcc的编译过程](https://www.ink-hz.cn/2021/10/10/computer-system/gcc-compile/The-Compile-System.png)
说明：

GCC程序的编译过程大概分四个阶段

- 预处理（Pre-Processing）
   g++ -E hello.cpp -o  hello.i 
   说明：
   1. g++ 默认使用 libstdc++标准库
   2. 模板的处理不是在**预处理阶段**完成的
- 编译（Compiling）：
    1. g++ -S hello.i -o hello.s
    2. 说明：编译过程就是把预处理完的文件进行一系列词法分析，语法分析，语义分析及优化后生成相应的汇编代码文件
    
    
- 汇编（Assembling）
  1.g++ -c  hello.s  -o hello.o
  2. objdump  -h hello.o以获取二进制文件每个段的头部信息
  
- 链接（Linking）
1. 链接器ld将各个目标文件组装在一起，解决符号依赖，库依赖关系，并生成可执行文件
2. g++ -o hello hello.o
3. 连接过程 g++ -v -o hello hello.o 
  

~~~
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnx
Thread model: posix


~~~

默认情况下是使用动态链接
~~~
file hello
hello: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked
~~~



## clang 编译hellow word 过程


~~~
clang++ hello.cpp -o hello -L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lc++ -lunwind


export LDFLAGS="-L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lunwind"

clang++ hello.cpp -o hello


otool -L hello
/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 1800.105.0)

/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1351.0.0)
~~~

- **`lang++`**：使用 LLVM 的 C++ 编译器来编译 C++ 源代码。
- **`hello.cpp`**：你的 C++ 源文件。
- **`-o hello`**：指定输出文件名为 `hello`，即编译后生成的可执行文件名。
- **`-L/usr/local/opt/llvm/lib/c++`**：告诉编译器去 `/usr/local/opt/llvm/lib/c++` 目录查找 `libc++` 库，这是通过 Homebrew 安装的 LLVM 提供的库。
- **`-L/usr/local/opt/llvm/lib/unwind`**：告诉编译器去 `/usr/local/opt/llvm/lib/unwind` 目录查找 `libunwind` 库，这是 LLVM 提供的用于处理堆栈展开和异常的库。
- **`-lc++`**：指定使用 LLVM 提供的 `libc++` 库。
- **`-lunwind`**：指定链接 LLVM 提供的 `libunwind` 库

 ### 这样做的效果：

- 通过这个命令，`clang++` 将使用 Homebrew 安装的 LLVM 提供的 `libc++` 和 `libunwind`，而不是系统自带的版本。
- 这对于需要使用更现代的 LLVM 特性或者自定义版本的库时非常有用


小王提问：/usr/lib/libc++.1.dylib  是 macOS 系统自带的** C++ 标准库，不是通过 LLVM 安装的。这个编译不对


老王回答:~/.bash_profile环境变量重新设置一次



~~~
export LDFLAGS="-L/usr/local/opt/llvm/lib/c++ -L/usr/local/opt/llvm/lib/unwind -lunwind" 

export CPPFLAGS="-I/usr/local/opt/llvm/include“

clang++ hello.cpp -o hello

otool -L hello
hello:
/usr/local/opt/llvm/lib/c++/libc++.1.dylib (compatibility version 1.0.0, current version 1.0.0)

/usr/local/opt/llvm/lib/unwind/libunwind.1.dylib (compatibility version 1.0.0, current version 1.0.0)


~~~


完整过程：clang++ -v  hello.cpp -o hello

clang++ -v -E  hello.cpp -o hello.i
 clang++ -v -c  hello.s -o hello.o
clang++  -v  hello.o -o hello
从你提供的 `clang++ -v hello.cpp -o hello` 命令的输出中，我们可以看到 `clang++` 编译过程的详细信息，涉及几个关键步骤：

1. **编译器信息**：
    
    - **`clang version 19.1.6`**：表示你正在使用 Homebrew 安装的 `clang++` 版本 19.1.6。
    - **目标平台**：`x86_64-apple-darwin24.2.0`，表明编译器为 macOS 的 x86_64 架构生成代码。
    - **安装目录**：编译器位于 `/usr/local/Cellar/llvm/19.1.6/bin`。
2. **预处理**：
    
    - 编译器会处理源代码文件 `hello.cpp`，展开其中的宏和头文件。编译器在日志中显示了头文件搜索路径。例如，它会查找 `#include` 文件，在 `/usr/local/Cellar/llvm/19.1.6/bin/../include/c++/v1` 和 `/usr/local/Cellar/llvm/19.1.6/lib/clang/19/include` 等目录中查找标准库。
3. **编译**：
    
    - 在此阶段，`clang++` 将 C++ 源代码文件 `hello.cpp` 编译为目标文件（`.o` 文件）。它调用了 `clang-19`（`clang++` 的前端），并且使用了很多编译选项，如 `-target-sdk-version=15.2` 来指定 SDK 版本。
    - 日志中提到了编译器的一些标志，如 `-fcxx-exceptions`（启用 C++ 异常支持），`-fexceptions`（启用异常），`-mrelocation-model pic`（生成位置无关代码），等等。
4. **链接**：
    
    - 最后，编译器调用 `ld`（链接器）来链接生成目标文件（`.o`）和所需的标准库（如 `libc++`）。这会生成最终的可执行文件 `hello`。
    - 链接器调用日志显示了它如何将目标文件与标准库链接（包括 `libc++` 和 `libSystem`），并使用 `-syslibroot` 指定了 macOS 的系统库路径。




## 任务拆封02: POSIX AIO (Asynchronous I/O)** on macOS

我昨天又看了一下，大致有posix aio替换libaio，mac llvm原生编译依赖（clang 17或者19），mac llvm原生编译observer（这个有些基础了，但c++标准库要从libstdc++替换到libc++）


 I/O多路复用方案

- macOS  kqueue
- POSIX AIO
- Linux libAIO


![[Pasted image 20250112125303.png]]


小王提问：Difference between POSIX AIO and libaio on Linux?



POSIX AIO 本质上在是在用户级别上实现的，它在多个线程中执行正常的阻塞 I/O，不需要内核支持，因此给人一种 I/O 是异步的错觉。这样做特点是：
▶ 它适用于任何文件系统
▶ 它（基本上）可以在任何操作系统上运行
▶ 它适用于启用缓冲的文件（即不设置 O_DIRECT 标志）
而主要缺点是IO队列深度（即实际可以执行的未完成操作数）受所选线程数的限制

The POSIX AIO is a user-level implementation that performs normal blocking I/O in multiple threads, hence giving the illusion that the I/Os are asynchronous. The main reason to do this is that:
1. it works with any filesystem
2. it works (essentially) on any operating system (keep in mind that gnu's libc is portable)
3. it works on files with buffering enabled (i.e. no O_DIRECT flag set)    

**linux AIO — libaio 实现的异步 IO**

The Linux-native asynchronous I/O facility ("async I/O", or "aio") has a richer API and capability set than the simple POSIX async I/O facility
linux AIO 则是 linux 内核原声支持的异步 IO 调用，行为更加低级。


`io_uring` 是 2019 年 **==Linux 5.1==** 内核首次引入的高性能 **==异步 I/O 框架==**，能显著加速 I/O 密集型应用的性能

关于 linux IO 模型及 AIO、POSIX AIO 的简
https://cloud.tencent.com/developer/article/2031221
https://formulae.brew.sh/formula/libaio

[译] Linux 异步 I/O 框架 io_uring：基本原理、程序示例与性能压测（2020
https://arthurchiao.art/blog/intro-to-io-uring-zh/



> **POSIX AIO (Asynchronous I/O)** on macOS


``` c++

clang -v
Homebrew clang version 19.1.6

git clone git@github.com:watchpoints/oceanbase.git


brew install libaio
```



###  task03 源码分析 build.sh执行过程


> Check the [Install toolchain](https://oceanbase.github.io/oceanbase/toolchain/) guide for supported OS, GLIBC version requirement, and how to install the C++ toolcha

https://oceanbase.github.io/oceanbase/toolchain/
	

CentOS编译
```shell
# 1. 初始化依赖
./build.sh --init

# 2. 选择编译模式（debug/release等）
./build.sh debug --make
./build.sh release --make

```

### 1. 系统要求

- CentOS 6及以上版本

- 支持的CPU架构：x86_64或aarch64
### 2. 基础编译工具


```
# 必需的基础软件包
yum install -y \
  gcc \
  gcc-c++ \
  make \
  glibc-devel \
  libstdc++-devel \
  git \
  wget \
  perl \
  tar \
  bzip2 \
  which

```

### 3. 编译器版本要求


```shell
# GCC版本要求
- 默认支持C++11标准 (代码中 CPP_STANDARD_OPTION=11)
- 如果使用-DCPP_STANDARD_20=ON，则需要支持C++20标准的GCC版本

# 推荐GCC版本
- CentOS 7: GCC 4.8.5及以上
- CentOS 8: GCC 8.x及以上

```

### 4. 系统资源要求


```shell
# 硬件资源

- CPU: 建议4核及以上（编译会使用所有可用核心 grep -c ^processor /proc/cpuinfo）

- 内存: 建议16GB及以上

- 磁盘空间: 建议50GB及以上（用于源码、依赖和编译输出）

  

# 系统配置

- 最大文件打开数: ulimit -n 65535

- 最大用户进程数: ulimit -u 65535

```

### 5. 依赖库要求


```shell
# 核心依赖包
yum install -y \
  libaio-devel \
  openssl-devel \
  bzip2-devel \
  zlib-devel \
  ncurses-devel \
  libcurl-devel \
  readline-devel \
  mariadb-devel \
  boost-devel

# 如果使用RPM打包还需要
yum install -y \
  rpm-build \
  rpmlint

```

### 6. CMake要求


```shell
- 脚本会使用工具目录中的CMake: ${TOOLS_DIR}/bin/cmake

```



![[Pasted image 20250112163816.png]]




### 流程说明：


#### 入口流程

```# build.sh入口
./build.sh --init
↓
main "$@" 。//$@ 是一个特殊变量，代表脚本运行时传递的所有命令行参数
↓
parse_args  # 设置NEED_INIT=true
↓
do_init     # 执行初始化

```



### MacOS改造方案

# OceanBase 数据库知识导图

[https://www.oceanbase.com/knowledge-base/oceanbase-database-1000000000324072](https://www.oceanbase.com/knowledge-base/oceanbase-database-1000000000324072)  
  

![](https://cdn.nlark.com/yuque/0/2024/png/215144/1728394276721-9bfab5eb-d3af-4fb6-913b-f8b40b386077.png)





## 参考
【1】  OceanBase Development Guide
  https://oceanbase.github.io/oceanbase/ide-settings/
【2】 https://markdown.com.cn/extended-syntax/tables.html
【3】 https://llvm.org/
      https://releases.llvm.org/download.html
      https://formulae.brew.sh/formula/llvm
【4】 # makefile的选项CFLAGS、CPPFLAGS、LDFLAGS和LIBS的区别
- https://www.ink-hz.cn/2021/10/10/computer-system/gcc-compile/
- 深入理解计算机操作系统
【5】 流程图

https://snowdreams1006.github.io/write/mermaid-flow-chart.html

#### 小贴士


一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

二、这个技术的优势和劣势分别是什么

三、这个技术适用的场景。任何技术都有其适用的场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比



