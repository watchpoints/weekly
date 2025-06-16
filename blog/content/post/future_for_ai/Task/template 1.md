---
title: 三天从 0 到 1 实现一个编译器，你也可以
date: 2025-04-12
description: c++
draft: false
tags:
  - 小而美
categories:
  - do book
---







# 一、 开发环境搭建

### 1. 云主机

1. Gitpod：**一个基于云原生技术的在线集成开发环境（IDE）** ，也是阅读和调试github源代码利器
2. ccls:vscode 插件  C/C++/ObjC language server supporting cross references, hierarchies, completion and semantic highlight
3. CMake：vscode 插件

### 2.  依赖安装


这个脚本主要用于交叉编译和测试针对 ARM Linux 架构的 C 语言程序

#### **安装qemu-user-static**

sudo apt-get update 
sudo apt-get install -y qemu-user-static

which qemu-aarch64-static
/usr/bin/qemu-aarch64-static


#### clang-18

- Ubuntu默认仓库中没有clang-18
- 需要添加LLVM官方仓库来获取较新版本的clang

```shell
# 添加LLVM官方仓库密钥 
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add - 
# 添加仓库(根据您的Ubuntu版本选择对应的代号，如focal/jammy等) 
sudo add-apt-repository "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-18 main" 
sudo apt-get update


sudo apt install -y clang-18 lld-18 lldb-18 clangd-18

clang-18 --version
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin


lld-18 --version
lld is a generic driver.
Invoke ld.lld (Unix), ld64.lld (macOS), lld-link (Windows), wasm-ld (WebAssembly) instead

# 1. 添加LLVM 19仓库 
sudo add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main" 
sudo apt update 
# 2. 安装lld-19 
sudo apt install -y lld-19

sudo apt install -y clang-18 lld-19

```
#### c++17

g++ --version
g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
g++ -dM -E -x c++ /dev/null | grep -F __cplusplus
#define __cplusplus 201703L


####  bison 编译器依赖安装（必不可少）

sudo apt install flex bison
sudo apt install -y build-essential cmake flex bison graphviz libgraphviz-dev

sudo apt install graphviz libgraphviz-dev

sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu cmake -
DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ ..

### 3. 知识补充

**arm架构和x86架构**
- ARM架构是一种基于RISC（精简指令集计算机）的处理器架构，最初是为移动设备和嵌入式系统设计的。ARM处理器具有低功耗、高性能和低成本等优点，因此在移动设备、智能家居、工业控制等领域得到广泛应用。
- x86架构是一种基于CISC（复杂指令集计算机）的处理器架构，最初是为个人电脑设计的。x86处理器具有高性能和广泛的软件支持等优点，因此在个人电脑、服务器等领域得到广泛应用。

**如何在x86cpu 上编译支持 arm 架构程序：** - uname -m x86_64
- 因为国产化的普及，尤其一些证券和银行行业，已经开始走信创的路线,自己买 `arm` 平台的 `CPU`，这个成本着实吃不消，于是尝试 `x86` 平台运行 `arm` 平台的容器来降本增效
- 用户模式模拟器： qemu-user-static 允许您在一种 CPU 架构的 Linux 系统上运行为另一种 CPU 架构编译的 Linux 程序。它不是一个完整的虚拟机，它只模拟 CPU 指令集和一些基本的系统调用，将其他系统调用直接传递给宿主机的 Linux 内核。
- 用途： 它的主要用途是交叉开发和测试。例如，您可以在一台 x86/x64 架构的开发机器上编译和测试为 ARM 架构嵌入式设备编写的程序，而无需拥有实际的 ARM 硬件

**export QEMU_LD_PREFIX=/usr/aarch64-linux-gnu**

QEMU_LD_PREFIX 指向的目录是交叉编译工具链的安装路径，它里面包含了目标架构（在这里是 ARM64 Linux）运行程序所需的所有核心共享库和动态链接器。
这些文件是在您安装交叉编译环境时由系统自动放置的，您通常不需要手动管理它们。


**GCC 替代品:Clang 的主要目标之一是提供GCC 的替代方案，
尤其是在编译大型项目时，Clang 能够比GCC 更快地完成编译工作

Clang-18 是==一个C、C++、Objective-C 和Objective-C++ 编程语言的编译器前端的特定版本==，它是基于LLVM 编译器的。

- 更具体地说，它指的是Clang 项目的第18 个主要版本。Clang 作为GCC（GNU 编译器套件）的替代品而开发，它支持多种C++ 标准（包括C++11、C++14、C++17 和部分C++20）

- 英特尔为C/C++ 编译器全面采用 LLVM

英特尔的长期编译器专家 James Reinders透露，将在下一代英特尔 C/C++ 编译器中使用 LLVM 开源基础架构；并分享了一些相关信息。  

“LLVM 有助于实现为[英特尔架构](https://zhida.zhihu.com/search?content_id=179428535&content_type=Article&match_order=1&q=%E8%8B%B1%E7%89%B9%E5%B0%94%E6%9E%B6%E6%9E%84&zhida_source=entity)，提供最佳 C/C++ 编译器的目标。最新的英特尔 C/C++ 编译器，使用 LLVM，可提供更快的编译时间、更好的优化、增强的标准支持

- 特尔工程师报告，新的基于 LLVM 的 oneAPI DPC++/C++ Compiler 2021.3 的构建时间，相较经典 C++ 编译器要减少了 14%

https://zh.wikipedia.org/zh-cn/Clan


## clang/llvm 介绍
LLVM 项目是模块化和可重用编译器以及工具链的集合，clang 是其编译器前端。
相比较于 GNU 编译工具链，其优势在于编译速度更快，静态检查工具更加完善，可拓展性更强等等。
GNU 工具链与 LLVM 工具链主要区别如下表：

|项目|GNU工具链|LLVM工具链|
|---|---|---|
|C编译器|gcc|clang|
|C++编译器|g++|clang++|
|binutils|GNU binutils|LLVM binutils|
|汇编器|GNU as|集成汇编器|
|链接器|ld.bfd,ld.gold|LLVM linker ld.lld|
|运行时|libgcc|compiler-rt|
|原子操作|libatomic|compiler-rt|
|C语言库|GNU libc glibc|LLVM libc|
|C++标准库|libstdc++|libc++|
|C++ABI|libsupcxx|libc++abi|
https://pages.openeuler.openatom.cn/embedded/docs/build/html/master/features/clang_llvm.html

|   |   |   |   |
|---|---|---|---|
|++17|完全支持|从GCC 5版本开始支持，到GCC 7版本已完全支持，是GCC 11到13版本的默认模式|-std=c++17 o|


**安装aarch64交叉编译工具链**


sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu 
sudo apt-get install -y libstdc++-12-dev-arm64-cross

arch64-linux-gnu-gcc --version
aarch64-linux-gnu-gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

# 二、 阅读 程序入口 


test.sh 脚本是一个自动化测试框架，用于编译、运行和验证针对特定目标架构（在本例中是 aarch64 ARM Linux）的 C 语言程序。


1. 集成测试： 它不仅仅测试自定义编译器，还测试整个编译和执行链条：

- 自定义编译器 (ax) 的输出。

- clang-18 的汇编和链接能力（特别是交叉编译能力）。

- qemu-aarch64-static 在宿主机上模拟 ARM64 环境并运行程序的能力。

- 动态链接器和自定义库 (libstd.so) 是否能被正确加载和使用。

1. 输出验证： 脚本将程序运行的实际输出与预定义的 .out 文件进行比较。这是一种回归测试的形式，确保程序在每次修改或编译后仍然产生相同且正确的输出。

```
ax $NAME.c | clang-18 -xassembler-with-cpp /dev/stdin -target aarch64-linux-gnueabihf -march=armv8a -fuse-ld=lld-19 -lstd -L./ -o $TMPD/$NAME
```
这一整行命令的作用是：

1. 首先，使用自定义的编译器 (../build/compiler) 将当前的 C 源代码文件 ($NAME.c) 编译成汇编代码。
2. 然后，将这些汇编代码通过管道传递给 clang-18。】
3.  clang-18 接收到汇编代码后，会将其汇编并链接，生成一个针对 ARM64 Linux 平台的动态链接可执行文件。
4. 这个生成的可执行文件会被保存到 /tmp/tst/ 目录下，并以原始 C 文件的名称命名 (例如 hello)。

其中的 -target aarch64-linux-gnueabihf 明确告诉 clang-18，
它要编译生成的目标代码是针对 ARM64 (aarch64) 架构的 Linux 系统的。
这就需要系统中存在 aarch64 的交叉编译能力。

sudo apt install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu


sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu 
sudo apt-get install -y libstdc++-12-dev-arm64-cross

arch64-linux-gnu-gcc --version
aarch64-linux-gnu-gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0



自动化编译： 脚本会遍历当前目录下所有的 .c 文件，并对它们进行编译。
这个编译过程是交叉编译，
	即将源代码从宿主机（例如 x86 Linux）编译成目标机（aarch64 Linux）的可执行文件

1. 自定义编译器测试： 脚本使用 ../build/compiler（即 ax）作为第一阶段的编译器，这表明它很可能是一个自定义的或正在开发的编译器前端。test.sh 的一个主要作用就是测试这个自定义编译器是否能正确地将 C 代码转换为汇编代码。

1. 集成测试： 它不仅仅测试自定义编译器，还测试整个编译和执行链条：

- 自定义编译器 (ax) 的输出。

- clang-18 的汇编和链接能力（特别是交叉编译能力）。

- qemu-aarch64-static 在宿主机上模拟 ARM64 环境并运行程序的能力。

- 动态链接器和自定义库 (libstd.so) 是否能被正确加载和使用。


1. 输出验证： 脚本将程序运行的实际输出与预定义的 .out 文件进行比较。这是一种回归测试的形式，确保程序在每次修改或编译后仍然产生相同且正确的输出。

1. 错误报告： 如果编译失败（CE）或输出不匹配，脚本会给出明确的错误提示，这对于开发和调试非常有用
2. 




# 三. 试点班编译器


https://gitlab.eduxiji.net/T202510699205664/Compiler

C0语法编译器-西北工业大学编译原理试点班
https://github.com/zhangyongmao/compiler_C0

https://github.com/happy-lx/C0-compiler


1. 从CMakeLists.txt分析项目依赖：
- 必需依赖：Graphviz、Flex、Bison
- 可选依赖：ANTLR4(已注释掉)
- 编译器要求：g++/gcc或MSVC，C++17标准
2. 编译流程：
- 首先需要生成词法分析器(Flex)和语法分析器(Bison)代码
- 然后编译整个项目
- 可选择生成语法分析图(Graphviz)
3. 需要检查系统是否安装：
- Graphviz开发包
- Flex和Bison工具
- C++编译器(g++/clang++)
4. 项目特点：
- 支持ARM32/ARM64代码生成
- 使用现代C++17特性


**C++17核心特性**

- **结构化绑定(Structured Bindings)**：

允许从元组/结构体直接解包变量

示例：`auto [x,y] = getPoint();`

- **if/switch初始化语句**：

在条件语句中直接初始化变量

示例：`if (auto it = m.find(key); it != m.end()) {...}`

- **内联变量(Inline Variables)**：

简化头文件中的变量定义，避免多重定义错误

- **constexpr lambda**：

允许lambda表达式在编译期执行

**2. 项目中可能使用的特性**

从代码结构看可能使用了：

- **std::optional**：安全处理可能缺失的值
- **std::variant**：类型安全的联合体
- **文件系统库(std::filesystem)**：处理文件路径和目录
- **并行算法**：提升性能(但项目未明显使用)
- 


**. 标准编译流程（非交叉编译）**

`# 进入项目目录 cd build/Compiler  # 创建构建目录并配置 mkdir -p build && cd build cmake ..  # 编译项目 make -j$(nproc)`
#### 资料
- https://bcmi.sjtu.edu.cn/home/limu/tiger/Lec/score.pdf
- https://bcmi.sjtu.edu.cn/~limu/tiger/Res/Tiger_Compiler_Doc.pdf  上海交通大学计算机系 编译原理课程设计
### 目录结构

目录结构分析及功能说明：

- .git/: Git 版本控制系统目录，用于跟踪代码变更。
- utils/: 存放各种实用工具函数或辅助类，可能包含通用数据结构、算法或 I/O 助手。
- test/: 包含编译器的测试用例，用于验证编译器各个阶段的正确性。
- symboltable/: 实现符号表，编译器中的一个基本数据结构，用于存储标识符（变量、函数、类等）的信息，如类型、作用域和内存位置。
- main.cpp: 程序的入口点，负责协调编译器各个阶段的运行。
- ir/: 专注于中间表示 (Intermediate Representation, IR)。前端处理源代码后，通常会将其转换为 IR，这是一种更抽象和独立于机器的表示，便于优化和代码生成。

- frontend/: 包含编译器的前端组件，包括：

- 词法分析 (Lexical Analysis)：将源代码分解成标记 (tokens)。

- 语法分析 (Syntax Analysis)：根据标记构建解析树或抽象语法树 (AST)。

- 语义分析 (Semantic Analysis)：检查语义错误（例如，类型不匹配、未声明的变量）并收集代码生成所需的信息
- - doc/: 项目文档，可能包括设计文档、用户手册或 API 参考。

- changelog.txt: 记录项目随时间推移的更改、新功能和错误修复。

- cpp/: 可能包含共享的 C++ 源文件或一个在编译器不同部分使用的特定 C++ 相关模块。

- backend/: 负责编译的最后阶段，包括：

- 优化 (Optimization)：提高生成代码的性能或减小其大小。

- 代码生成 (Code Generation)：将中间表示转换为目标机器代码（例如，汇编代码或可执行二进制文件）
https://github.com/happy-lx
#### 2023_func_04_arr_defn3



简而言之，AST.cpp 是编译器前端的核心部分，负责将源代码的结构抽象地表示出来，为后续的编译阶段提供了一个方便操作的数据结构



../build/compiler -S -T  2023_func_04_arr_defn3.c

../build/compiler -S -T  2023_func_04_arr_defn3.c




## 我是谁

大家好，我是小义同学，

**刚刚好，是最难得的美好**

我就在这里 ，我刚刚好。

欢迎链接我。 个人微信：wang_cyi


![image.png](https://s2.loli.net/2025/04/03/1Qbo8463tKEqkeT.png)


我是一个15年c++开发经验工程师，
我果断舍去大而全项目，
我选择了“小而美”的产品，让我的服务运行在2核2G一年100元的云主机。
我必须舍去过去电信，电商，存储经验开发经验
重新用户需求，重新设计项目。

我设定了一个切实可行的目标：
让别人愿意毫无负担地拿出100元，而你也能全力以赴将这100元完成
刚刚好。

【《最好的济南》济南城市形象宣传片 济南市中英双语外宣片】
https://www.bilibili.com/video/BV1TW4y1372y/

刚刚好，是最难得的美好，

是一种胸怀，

是一种境界，有智慧、有诗意、有余地，

更有“百尺竿头更进一步”的广阔空间、
“百舸争流千帆竞”的奔涌姿态
<iframe src="//player.bilibili.com/player.html?isOutside=true&aid=947706170&bvid=BV1TW4y1372y&cid=1001200489&p=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"></iframe>

<iframe src="//player.bilibili.com/player.html?isOutside=true&aid=947706170&bvid=BV1TW4y1372y&cid=1001200489&p=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"></iframe>






