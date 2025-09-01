---
title: 面试官:指针和应用区别
date: 2025-08-25
description: do book
draft: false
tags: 
categories:
  - do book
---
 各位老师好！
	 
这是CPP面试冲刺周刊 （c++ weekly）
陪你一起快速冲击大厂面试 第四期

**周刊目标**：
- 不是成为C++专家，而是成为C++面试专家

**本期内容**：
 
 - 如何为一个类自定义new/delete

**一页PPT 解释：** （回答有逻辑）

| 维度      | ① 类内 new/delete      | ② 全局 new/delete（弱符号）         | ③ LD_PRELOAD + tcmalloc |
| ------- | -------------------- | ---------------------------- | ----------------------- |
| 生效时机    | **编译期**（静态绑定）        | **链接期**（符号强弱覆盖）              | **运行期**（动态装载器优先级）       |
| 作用范围    | 仅该类/基类查找链            | 全局（除已绑定到类内的 new）             | 全局（动态库与主程序）             |
| 机制      | 作用域查找 + 非虚（无 vtable） | 弱符号 vs 强符号                   | 预加载库先解析同名符号             |
| 是否依赖弱符号 | 否                    | 是（libstdc++ 默认全局 new 为 weak） | 否（依赖 ld.so 解析顺序）        |
| 源码是否需改动 | 需要（在类内写函数）           | 需要（提供全局 operator new/delete） | 不需要（设置环境变量即可）           |
| 常见用途    | 类级内存池、对齐、对象复用        | 全局策略（统计/替换分配器/统一注入）          | 线上快速切换分配器/排查内存/零侵入部署    |

**题目来源**： **C++ Prime**r 第 18 章节
- Exercise 18.9: Declare members new and delete for the QueueItem class.
- 18.1.6 Class Specific new and delete
- Exercise 18.6: Reimplement your Vector class to use operator new, operator delete, placement new, and direct calls to the destructor.

![课本上的题目绝对经典](https://s2.loli.net/2025/08/25/Wzr3ceEmvY9dFwL.png)



**整体知识看板**（看文末）：
- 第一周:c++基础知识高频面试题解析【当前位置】
- 第二周: 专注分布式存储,数据库广告搜索 Ai 辅助驾驶 大厂热门后端开发岗位拆解。
- 第三周：系统架构设计，用未来 10 年发展目标，重新设计原来系统


开始

# 一、面试官：如何为一个类自定义new/delete


C++ 对象的内存分配默认依赖 **全局 `operator new` / `operator delete`**，
本质上调用 **libc malloc/free**，最终走 **系统调用 brk/mmap** 

答案可能很简单（这绝对不是最终结果，面试官要反问的）

```c++
#include <iostream>
using namespace std;

struct A {
    void* operator new(size_t sz) {
        cout << "A::operator new, size = " << sz << endl;
        return ::operator new(sz);
    }
    void operator delete(void* p) {
        cout << "A::operator delete" << endl;
        ::operator delete(p);
    }
};
int main() {
    A* a = new A;  // 输出: A::operator new, size = 1
    delete a;      // 输出: A::operator delete
}

```


如何和已有知识，一步步结合起来，

下面是我推导过程，可能意想不到发现


# 二、小青回答（工作0-3 年 青铜）


如何如何为一个类自定义new/delete 
完全之前从来没有遇到过问题，
很简单，平时不总结，别指望 面试当成超常发挥，

我猜 你可能这样回顾之前准备
- new 和 malloc 有什么区别
- new 申请失败返回 NULL 还是抛出异常
- 自定义一个类函数，虚函数可以吗，好像不行
- operator new 还是选择  placement new 

**暂停 思绪无限发散**，

重新整理 历史题目的的关系

 ## 1.1  普通的函数：函数特性 重载（overload），隐藏（hide），覆盖（override）

- 重载（overload):在相同访问内（一个类），函数名相同,参数不同（**c 语言不支持这样重载**）
- 隐藏（hide): 不同范围内 ,派生类定义与基类同名非虚函数时，基类同名函数被隐藏
- **覆盖 / 重写（Override）****：基类虚函数被派生类虚函数覆盖，运行期判断

	C++多态必要条件 
	① 继承（this 指针)
	② 虚函数重写 ,
	③ 父类指针/引用指向子类对象 


>画外音：重新整理c++函数基本特性 目前就是动态绑定无法解决这个问题，

- 因为new 是运算符 不是函数，不支持重载
- 更重要的 operator new  是默认 static 函数，static 函数无法访问 this 指针，不支持运行时多态


 ## 1.2   普通函数，库函数，系统调用
 
 参考：**序员的自我修养：链接、装载与库**
 
- **new** 是c++运算符，不能重载，语法规定的
-  operator new 是c++ libstdc++ 标准库函数，静态函数，静态函数自然是不能是虚函数

```c++
_GLIBCXX_WEAK_DEFINITION void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)

```
-  malloc 是 c 语言 glibc 标准库的库函数
- brk是系统调用


演示：

new 实现调用2个函数

1. 内存分配 调用全局的 operator new 函数为对象分配足够大小的内存。（4字节）
2. 调用 Foo构造对象
3. 返回对象指针 构造完毕后返回分配并初始化后的对象指针。

![](https://developer.qcloudimg.com/http-save/yehe-1640143/35cc51fabf2e08f0c39d39a8b732a6eb.png)

``` c

 Foo* ptr = new Foo();
   │
   ▼
查找类内 operator new
   │
   ├─存在 → 调用类内 new
   └─不存在 → 调用全局 ::operator new
   │
   ▼
内存分配（malloc / 内存池 / 用户态栈）
   │
   ▼
调用构造函数
   │
   ▼
返回对象指针



```




![](https://img-blog.csdn.net/20170115234108145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBvbGxvbl9rcmo=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)_


1.3 c++角度无法解决这个问题了，c 语言不支持重载，如何解决重载问题 弱符号，编译期静态绑定



| 概念                           | 实现方式             | 发生时机    | 影响范围        |
| ---------------------------- | ---------------- | ------- | ----------- |
| **函数重载 (overloading)**       | 同一作用域内多个函数签名不同   | **编译期** | 仅 C++ 语法    |
| **虚函数覆盖 (overriding)**       | 子类重写父类虚函数        | **运行时** | 仅多态场景       |
| **符号覆盖 (symbol overriding)** | 通过弱符号 + 链接器选择强符号 | **链接期** | 整个可执行文件/动态库 |


## **如何验证 libstdc++ 的 operator new 是弱符号**

我们可以直接用 `nm` 命令验证：

`nm -C /usr/lib/x86_64-linux-gnu/libstdc++.so | grep "operator new"`

典型输出类似：

`0000000000098e80 W operator new(unsigned long) 0000000000098ed0 W operator new[](unsigned long) 0000000000098f20 W operator delete(void*)`

这里的 `W` 就表示 **weak symbol**。


 **为什么 libstdc++ 把 operator new/delete 定义为弱符号**

来看 libstdc++ 的源码（以 GCC 13.x 为例）：

注意到 `_GLIBCXX_WEAK_DEFINITION`，在 GCC 平台上一般展开为：

`#define _GLIBCXX_WEAK_DEFINITION __attribute__ ((weak))`

这意味着 **libstdc++ 提供的 operator new 是弱符号**。


原因：为了支持用户自定义重载

在 Linux 下，符号解析遵循 **ELF 链接规则**：

- **弱符号（weak）**：如果存在同名的**强符号（strong）**，那么最终可执行文件会绑定到强符号。
    
- **强符号（strong）**：用户自己实现的 `operator new` 会被视为强符号。
    
**因此：**
- 如果用户自己实现了全局 `operator new`，**会自动覆盖** libstdc++ 提供的版本。
- 如果用户没有实现，则默认使用 libstdc++ 的弱符号实现。

这就是为什么 libstdc++ 必须把它们标记为弱符号，否则用户无法重载


> 到这里你是否感觉找到最终答案 NO，c++继承复杂之处在这里，
> 上面operator new是全局符号，如果有继承的类呢？

#  三 、小白回答(工作 3-5 年 白银)


如果面试官问：

> “为什么类内 `operator new/delete` 是静态绑定，而全局 `operator new/delete` 是弱符号？”

可以回答：

1. **全局版本**是由 **libstdc++** 提供的默认实现，在源码中标记了 `__attribute__((weak))`，用户可以通过定义自己的全局版本来覆盖默认实现 → **弱符号机制**。
    
2. **类内版本**是一个 **普通静态成员函数**，绑定过程在 **编译期**完成，属于 **静态绑定**，不会走虚函数表，也不会受弱符号影响。
    
3. 当类内和全局版本同时存在，**类内优先**


30 秒总结自定义类实现 new 和/delete：

- 第一是类内 operator new/delete，纯编译期静态绑定，作用域查找，不涉及弱符号； 

- 第二层是全局 operator new/delete，libstdc++ 默认实现是弱符号，你提供强符号即可链接期覆盖； 

- 第三层是LD_PRELOAD + tcmalloc，通过动态装载器在运行时优先解析同名符号，实现零改码劫持。 

三者分别对应编译期、链接期、运行期三个阶段，

优先级依次是：类内静态绑定 > 运行期预加载 > 链接期弱符号

## 2.1 深入理解 C++ new/delete：类内静态绑定 vs 全局弱符号机制



|维度|**类内 `operator new/delete`**|**全局 `::operator new/delete`**|
|---|---|---|
|**作用域**|限于该类及其派生类|全局可见|
|**绑定时机**|编译期静态绑定|链接期符号解析|
|**多态性**|❌ 无多态|❌ 无多态|
|**弱符号**|❌ 不依赖弱符号|✅ 是弱符号|
|**优先级**|优先于全局 `::operator new`|最后兜底|


| **位置**  | **示例**                                        | **作用范围**             | **绑定类型** | **多态性** | **弱符号相关性** |
| ------- | --------------------------------------------- | -------------------- | -------- | ------- | ---------- |
| **全局的** | `void* operator new(std::size_t size)`        | 所有未被类内覆盖的 `new` 表达式  | **弱符号**  | 无       | ✅ 和弱符号有关   |
| **类内的** | `static void* operator new(std::size_t size)` | 仅限该类及其派生类的 `new` 表达式 | **静态绑定** | ❌       | ❌ 与弱符号无关   |

| 特性      | 类内 `operator new/delete` | 全局 `operator new/delete`   |
| ------- | ------------------------ | -------------------------- |
| 绑定方式    | **静态绑定**，编译期决定           | 链接时通过 **弱符号** 可被覆盖         |
| 是否虚函数   | 否                        | 否（但全局版本可被替换）               |
| 重载可见性   | 类内作用域                    | 全局作用域                      |
| 标准库实现   | 普通函数                     | `_GLIBCXX_WEAK_DEFINITION` |
| 可否被用户替换 | 仅在类作用域可自定义               | **用户可定义全局重载**              |


```c++
#include <iostream>
#include <new>
struct Base
{
	static void* operator new(std::size_t size)
	{
		std::cout << "Base new\n";
		return ::operator new(size);
	}
};

struct Derived : Base {

};

int main()
{
	//A class-specific operator new is looked up in the scope of the class and is not virtual.
	
	Derived* p = new Derived; // 调用 Base::operator new 还是 Derived::operator new？
}
```
- **编译期静态绑定**  
    `new Derived` 的查找顺序是：
    1. 在 `Derived` 中查找 `operator new`
    2. 如果没找到，查找 `Base`
    3. 如果都没找到，最后使用全局 `::operator new`
- `Derived` 没有定义 `operator new`，所以直接用到了 **`Base::operator new`**。
- **这不是多态**，不需要虚函数表。


**最终结论**

- **类内 `operator new/delete`**：
    - 是静态成员函数
    - 编译期静态绑定
    - 和弱符号无关
- **全局 `operator new/delete`**：
    - libstdc++ 提供默认实现
    - 被标记为 **weak symbol**
    - 用户可覆盖


## 2.2 编译期，链接期，运行期 


![](https://developer.qcloudimg.com/http-save/yehe-1640143/a004cb2bddbf854cd28b7970f77aea99.png)

| 维度      | ① 类内 new/delete      | ② 全局 new/delete（弱符号）         | ③ LD_PRELOAD + tcmalloc |
| ------- | -------------------- | ---------------------------- | ----------------------- |
| 生效时机    | **编译期**（静态绑定）        | **链接期**（符号强弱覆盖）              | **运行期**（动态装载器优先级）       |
| 作用范围    | 仅该类/基类查找链            | 全局（除已绑定到类内的 new）             | 全局（动态库与主程序）             |
| 机制      | 作用域查找 + 非虚（无 vtable） | 弱符号 vs 强符号                   | 预加载库先解析同名符号             |
| 是否依赖弱符号 | 否                    | 是（libstdc++ 默认全局 new 为 weak） | 否（依赖 ld.so 解析顺序）        |
| 源码是否需改动 | 需要（在类内写函数）           | 需要（提供全局 operator new/delete） | 不需要（设置环境变量即可）           |
| 常见用途    | 类级内存池、对齐、对象复用        | 全局策略（统计/替换分配器/统一注入）          | 线上快速切换分配器/排查内存/零侵入部署    |


####  ① 类内 `operator new/delete`（静态绑定）

```c
源码：
struct Base {
  static void* operator new(std::size_t);
};
struct Derived : Base {};

new Derived
   │
   ├─ 编译器做“名字查找”（先 Derived，后 Base）
   │
   ├─ 若 Derived 未定义 → 绑定到 Base::operator new   ←←← 静态绑定（非多态）
   │
   └─ 生成直接调用指令（无 vtable，无符号覆盖参与）


```

**要点**
- 编译期决定，**不走弱符号**、不走 vtable。
- 仅影响该类（及查找到的基类作用域），**优先级高于全局**。

###  ② 全局 `operator new/delete`（弱符号覆盖，链接期）

```c
源文件们  ──(编译)──► 目标文件们（含符号表） ──(链接)──► 可执行文件 / so

libstdc++ 提供：
  _GLIBCXX_WEAK_DEFINITION
  void* ::operator new(std::size_t);     ← 弱符号（weak）

你的工程若提供：
  void* ::operator new(std::size_t);     ← 强符号（strong）

链接器规则：

  若同名强符号存在 → 选择强符号（你的全局 new）
  
  否则 → 选择弱符号（libstdc++ 默认 new）


```
### ③ `LD_PRELOAD` + tcmalloc（动态链接优先级，运行期）


```c++
运行命令：
  LD_PRELOAD=/usr/lib/libtcmalloc.so ./app

动态装载器 ld.so 加载顺序：
  1) 先装载 LD_PRELOAD 指定的 so（优先级最高）
  2) 再装载主程序与其依赖的其它 so（libc, libstdc++, ...）

符号解析：
  当需要解析 "malloc/free/new/delete" 时
  ├─ 如果在预加载的 tcmalloc.so 中已定义 → 直接绑定到 tcmalloc 版本
  └─ 否则继续在后续库中查找（如 libc）

效果：
  不改源码，即可把 malloc/free/new/delete 劫持到 tcmalloc

```


**要点**

- **运行时**生效，由**动态装载器**决定符号优先级。
- 对主程序和动态库均可生效（除非静态链接/受限环境）。
- 常用于**快速切换分配器/排查内存问题**。


# 四、小王回答( 工作 5-10 年 )：用起来


> 工作 10 年和工作 3 年 在知识不会任何新增 ,结合 3fs 代码说明

## 4.1  DeepSeek 3FS 灵活内存池实现策略


- 自定义：用默认系统的
- 自定义：用第三方库的
- 自定义：自己实现，这个不是分布式存储重点，没有实现。


代码分析：

#### 1. 定义宏开关 CMakeLists.txt
```
option(OVERRIDE_CXX_NEW_DELETE "Override C++ new/delete operator" OFF)

```

#### 2. 重载全局函数 operator new/delete

- src\memory\common\OverrideCppNewDelete.h
- 代码

```c++

#ifdef OVERRIDE_CXX_NEW_DELETE

// Override global new/delete with custom memory allocator.
void *operator new(size_t size) { return hf3fs::memory::allocate(size); }

void operator delete(void *mem) noexcept { hf3fs::memory::deallocate(mem); }

#endif

```


### 3.  dlopen方式加载 动态库

- src/memory/common/GlobalMemoryAllocator.cc


```c++

static void loadMemoryAllocatorLib()
	void *mallocLib = nullptr;
	GetMemoryAllocatorFunc getMemoryAllocatorFunc = nullptr;
	mallocLib = ::dlopen(mallocLibPath, RTLD_NOW | RTLD_GLOBAL);
	
	gAllocator = getMemoryAllocatorFunc();

//
//这里通过环境变量 MEMORY_ALLOCATOR_LIB_PATH 来指定要加载的内存分配器库。 
//例如，如果你想使用jemalloc.tcmalloc： 
//set MEMORY_ALLOCATOR_LIB_PATH=D:\path\to\jemalloc.dll 
//set MEMORY_ALLOCATOR_LIB_PATH=D:\path\to\tcmalloc.dll
void *allocate(size_t size)

	if (gAllocator == nullptr)
	
		mem = std::malloc(allocateSize);
	
	else
		mem = gAllocator->allocate(allocateSize)

```



提供 GetMemoryAllocatorFunc 函数指针类型用于通过固定导出符号加载实现（如 dlsym("getMemoryAllocator")）:

```c++

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

} // namespace hf3fs::memory


```

### 4.2 疑问：智能指针在自定义分配器情况不能调用默认的 delete 操作


### 要点

- 谁分配，谁释放；分配与释放函数必须匹配。
- 对象池/自定义分配接口返回的内存，务必使用自定义 deleter；默认 delete 不适用。
- 启用 3FS 全局 new/delete 重载时，默认智能指针 deleter 可直接用


```c++
  void* mem = hf3fs::memory::memalign(alignof(My), sizeof(My));
  My* obj = new (mem) My(...);
  
  auto deleter = [](My* p){
    p->~My(); //可以直接调用析构函数
    hf3fs::memory::deallocate(p); //归还到内存池
  };
  
  std::unique_ptr<My, decltype(deleter)> up(obj, deleter);

```





# 五、广告时间

c++周刊目的陪你一起快速冲击大厂面试

> 小提示：不要把他看成一个出售给你产品，我只出售给自己
> 在公司做任何事情事情，
> 都必须清楚拆解需求功能，开发周期，最后得到什么结果，
> 同样面试准备也是如此，给自己一个期限 21 天，给自己大纲，然后给自己 21 天学习结果，这样自己才能安心准备下去。 



曾经有一个**让我心跳加速的岗位**放在我面前，  
我没有珍惜。  
等到别人拿到 offer 的那一刻，  
我才追悔莫及！

人世间，最痛苦的事情，  
不是没钱吃饭，  
也不是没房没车，  
而是——**错过了那个能让我逆天改命的机会！**

如果上天再给我一次机会，  
我一定会对那个岗位说三个字：  
**“我要你！”**

如果非要在这份“心动”上加一个期限，  
**一万年太久了……**  
**我只想要——21天！**


**你可能面临两种选择**

### **① 犹豫不前：准备到天荒地老**

“这个岗位太难了，我先准备一下吧。”  
于是你准备1天、1周、1个月、1年……  
等再回头，**3年就这样过去了**。
- 每天忙着搬砖，没时间系统复习
- 每次想起要准备，又感觉心里没底
- 面试知识点更新太快，拿着旧地图找新机会
最后，错过了一次又一次心动的岗位。

### **② 盲目回答：机会就在眼前，却抓不住**

终于等来一场面试，  
你觉得问题很简单，张口就答，  
结果用“几千元思维”回答“百万年薪岗位”。

- 面试官问到C++底层实现，答不上来
- 设计题说到高并发架构，没实战经验
- 一紧张，连项目里真实做过的东西都讲不清

一次面试失利，也许就意味着和理想岗位失之交臂。

### **更残酷的是**

在你犹豫的这几年里，  
找工作的成本越来越高：
- 一个部门、一个领导，可能坚持一年就被解散
- 一个项目，可能在10年、20年后，  
    曾经复杂的业务规则、先进的架构，早已被淘汰
- 市场上新的技术和面试要求，每年都在不断升级

等你回过头来，发现不仅机会没了，  
**连准备的方向都变了**。


 **21天C++面试冲刺周刊**
 ![​一万年太久，只争三周​](https://s2.loli.net/2025/08/24/AMq3IdPaloSnBpv.png)


不是让你成为**C++专家**，  而是让你成为**C++面试专家**。

不是让你疯狂学习新知识，  而是帮你**重新整理已有知识**，  
让你的能力与面试题精准对齐。


因为，21天就够了，  
足够让我火力全开，  
- 一边补齐 C++ 知识点，  
- 一边刷爆经典面试题，  
- 一边撸穿开源项目，  
- 让自己变得不可替代！


### **核心方法论**：
>让你学到每个 c++知识，都关联一个经典面试，并对对应开源项目实践
- **系统备战**  
    每天 20~30 分钟，聚焦 C++ 核心知识，  
    三周时间完成高效梳理。
- **经典面试题**  
    每个知识点都关联一个**高频面试题**，  
    让你知道“为什么考”和“怎么答”。
- **开源项目实践**  
    通过真实项目理解底层原理，  
    不背答案，而是用**实践打动面试官**。
- **场景驱动学习**  
    还原真实面试场景，  
    帮你学会“怎么说服面试官”。
## **21天，你会获得什么？**

- 一份**完整的C++面试知识地图**
- 一套**高频题+解析+项目实践**组合拳
- 一次**全链路模拟面试体验**
- 三周后，面对面试官，你能自信说出：  
    **“问吧，准备好了。”**



>这也是我的面试方法：
- 如果一开始就直接学某个知识点，我常常感觉不到它的实际价值。
- 所以我会先尝试树立一个整体的大局观，就算过程中被现实“啪啪打脸”了又怎样？
- 把每一次面试都当成一场陪练，用面试官的专业视角和真实项目来反推和校正自己的理解，不是更好吗？这种即时、高质量的反馈，是你看多少书、自己一个人闷头琢磨多久，都很难获得的。
整体知识看板（欢迎提供更多线索）：
- 第一周:c++基础知识高频面试题解析【当前位置】
![](https://s2.loli.net/2025/08/24/ZKvcR4F1yoe9th8.png)


- 第二周: 专注分布式存储,数据库广告搜索 Ai 辅助驾驶 大厂热门后端领域项目（基本功)拆解
![](https://mmbiz.qpic.cn/mmbiz_png/DMCnyCrhia37yjSL2xfQSbz0QT05IVAvsibibvAT7xFUzAhCqEKviblv6YpjuPT2VCCzZgMLDtDzGnFmHjuKOjHtTg/640?wx_fmt=png&from=appmsg&tp=webp&wxfrom=5&wx_lazy=1)

- 第三周：系统架构设计，用未来 10 年发展目标，重新设计原来系统

![](https://mmbiz.qpic.cn/mmbiz_png/DMCnyCrhia36iaMAgdk7ib4vIrpf6urrNhdmqVHib41GQAGXDMTPDvib0Ue3LROIocBSCflxytXXVoiap04lR7kOMfwA/640?wx_fmt=png&from=appmsg&wxfrom=5&wx_lazy=1&tp=webp)
历史题目:c++高频面试题

| 序号  | 知识地图   | 题目                                                                                                            |
| --- | ------ | ------------------------------------------------------------------------------------------------------------- |
| 1   | 新特性    | [一分钟讲透:c++新特性string_view](https://mp.weixin.qq.com/s/zBf1Dwp6e4wzXgRXJXrA5w?token=134003807&lang=zh_CN)       |
| 2   | 库的编译链接 | [如何给一个高速行驶的汽车换轮胎（实现一个可扩展c++服务）](https://mp.weixin.qq.com/s/AVlpGxdHK953l_wqVgTu4Q?token=134003807&lang=zh_CN) |
| 3   | STL    | [Traits 技术](https://mp.weixin.qq.com/s/6_oyn6aGUX8M0SmjlL8qWw)                                                |
| 4   | 新特性    | [if constexpr](https://mp.weixin.qq.com/s/JAfX1o-_CWmLU_Yg5mne8Q)<br>                                         |
| 5   | 新特性    | [面试题：C++中shared_ptr是线程安全的吗?](https://mp.weixin.qq.com/s/9HT9sOzf7Nq-qHIOGtjxSQ)                               |
| 6   | 模板     | [C++17 新特性 std::optional](https://mp.weixin.qq.com/s/DwbLC8DYaR2AFzreQQv3CQ)                                  |
| 7   | class  | [c++类的成员函数，能作为线程的参数吗](https://mp.weixin.qq.com/s/HXmCFhUP73c7A6OyN10qZQ)                                      |
| 8   | 编译器    | [const 如何保证const不变](https://mp.weixin.qq.com/s/ZrAB-LUBtSvoknsn5vPfVQ?)                                       |
| 9   | 值语义    | [一道面试题看深拷贝构造函数问题](https://mp.weixin.qq.com/s/c1udiVd-0QPO2W9IyowoHg)                                          |
| 10  | 值语义    | [ 智能指针究竟在考什么](https://mp.weixin.qq.com/s/_-ZG4oMugRTlBnA0zm7g0g)                                              |
| 11  | 指针     | [使用 C++ 智能指针遇到的坑](https://mp.weixin.qq.com/s/pZdxMguvojG0iFRqDaI2Ig?)                                         |
| 12  | 指针     | [指针与引用区别](https://mp.weixin.qq.com/s/98IBKr74VAP9fo-vgBttmQ)                                                  |

## 最动人的作品，为自己而写，刚刚好打动别人



 1️⃣ 如果有更多疑问，联系小王，一起交流，进步

![个人联系方式](https://s2.loli.net/2025/08/13/veChAocQwJONWKE.png)

2️⃣  关注公众号:后端开发成长指南(回复"面经"获取）获取过去我**全部面试录音**和面试复盘。
![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)


#### 抬头看天：走暗路、耕瘦田、进窄门、见微光
- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。