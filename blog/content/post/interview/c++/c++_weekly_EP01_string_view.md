---
title: 一分钟讲透:面试官为什么爱问c++新特性之std::string_view
date: 2025-06-20
description: do book
draft: false
tags: 
categories:
  - do book
---
![c++ weekly](https://s2.loli.net/2025/06/24/tU9gyJSP3GkDTiW.png)



#知识地图 #最佳实践 #CPP17


👋 各位老师好（老师是山东对人的一个尊称，就像称呼帅哥美女一样）

 C++周刊 （c++ weekly）第一期 EP01 开始了

**主题：**

今天用一分钟讲清楚—面试官为什么爱问std::string_view。


若有你遇到同样问题：

✅ **已有稳定业务实现**，认为“目前方案已经够好”，不清楚引入新特性是否真的带来价值；

✅ **苦恼于 C++ 学习路径**，不清楚如何学习 c++新特性；

✅ **只停留在了解新特性的表层概念**，却难以将其与真实项目结合，常常觉这是**没用的知识**


这一期，我们将从零开始，
一步步带你理解 `string_view` 背后的设计初衷、使用边界，

以及它如何在项目中为我们节省内存、提升性能。

我们不讲枯燥的标准文档，不抛出脱离工程的例子。

相反，我们将聚焦在**真实项目案例分析**

-----------------------------------------

### 1.  什么是string_view 与 string 有什么区别？

> std::string_view` is a C++17 library type that provides a lightweight,
>  non-owning reference to a sequence of characters



**数据结构**：

| 成员结构                      | 描述                                                                         |
| ------------------------- | -------------------------------------------------------------------------- |
| Member                    | Description                                                                |
| `const_pointer` `_data__` | a pointer to the underlying sequence  <br>(exposition-only member object*) |
| `size_type` `_size__`     | the number of characters  <br>(exposition-only member object*)             |

- 从定义看：和原来的 string 没有任何区别，都是一个指针 指向一个字符串？

- C++17中我们可以使用std::string_view来获取一个字符串的视图
字符串视图并不真正的创建或者拷贝字符串 

- <font color="#ff0000">只有使用权力，没有所有权，不负责资源申请，释放</font>。 




**算法操作**：substr 零拷贝 零内存分配

![](https://s2.loli.net/2025/06/24/AhbMLYnv1ERi872.png)
- **字符串切片（substr）**：不创建新对象，仅生成视图 ➜ 零拷贝
- **函数参数传递**：无需构造临时字符串对象 ➜ 降低内存分配
- std::string_view比std::string的性能要高很多

### 2. 使string_view 有啥好处

- ✅ 高频操作字符串,零拷贝 零内存分配 ,性能提高10%
- ✅开发者来说：修改一行代码,代码工作量不复杂，双手赞同 
- ✅对领导来说：方案简单，收益大, 绝对同意 

| 对比           | string   | string_view | 收益    |
| ------------ | -------- | ----------- | ----- |
| 内存分配次数       | O(n)     | **0**       | 完全消除  |
| 内存拷贝量        | 路径总长度    | **0字节**     | 零拷贝   |
| 解析延迟(1k QPS) | 4.2μs/op | 1.1μs/op    | 降低74% |
| CPU缓存命中率     | 83%      | **97%**     | +14%  |

### 3.  举例行业案例



![使用场景](https://s2.loli.net/2025/06/21/yiE6YKJQM98Tbs2.png)



 **OceanBase (OB)**

- 自定义:`ObString` 类 ，没有使用标准 std 库
```c++
class ObString 
{
  int32_t length_;
  char* ptr_;
};

ObLogExternalStorageCtx::init(const ObString &uri）
```

**RocksDB**
- 自定义：RocksDB 有自己的 `Slice` 类
```c++
class Slice {
  const char* data_;
  size_t size_;
};
```
- 使用场景 WriteBatch 时候Slice类和std::string_view 转换 
- **CephFS**
 - 使用 c++17 提供 string_view 代替string
 -  场景 元数据 MDS 查找一个文件时候目录遍历 

**Tikv:**

- 在 TiKV 这种对性能要求极高的 KV 存储中，大量的 key/value 是临时读取、比较、传递，而非修改；
- TiKV 在访问 RocksDB 时，通过 Rust 封装的 FFI 接口（通过 `rocksdb::DB`），对底层数据读取时不会创建新的字符串，而是传递指针和长度：
- 使用切片可以极大减少内存分配、数据复制、GC 压力；

| 切片比较   | Rust     | C++17    |
| ------ | -------- | -------- |
| 类型所有权  | 不拥有      | 不拥有      |
| 内存分配   | 无        | 无        |
| 拷贝行为   | 零拷贝      | 零拷贝      |
| 用法     | slice 操作 | substr操作 |
| 生命周期风险 | 编译期保障安全  | 存在悬垂引用风险 |

### 4. 动手挑战卡

###  4.1 系统设计
**Ceph MDS 路径解析场景：**
输入：/mnt/data/project1/report.docx  
输出：逐级路径 ["mnt", "data", "project1", "report.docx"]
🎯 考点内容：
- STL 提供基本 API 是是否熟悉 find ，substr ，vector，string？


- 在日常工作用 有没有结果基础库代码？
- 优化 `string_view` 的路径分割组件 保证零内存分配、无拷贝？
- 最后一个report.docx 单词怎么出来的。

📎 面试小陷阱：
- 面试时候，面试官会问，你最近 5 年 是不是一直写代码，你回答自然是
- 面试官说好，他怎么知道你水平？经常会结合项目出一个系统设计题目
- 刷题 不光有算法题目 还有系统设计题目,
- 掌握数据结构，掌握算法，最后应用到实际项目才算通过 
- 都是经常遇到小问题，一点都不难
### 4.2  传统实现（C++11）


```c++

// 存在多次内存拷贝
std::vector<std::string> split_path(const std::string& path) {
    std::vector<std::string> components;
    size_t start = 0, end = 0;
    
    while ((end = path.find('/', start)) != std::string::npos) {
        if (end != start) {
            components.push_back(path.substr(start, end - start)); 
        }
        start = end + 1;
    }
    // 最后一个单词
    if (start < path.length()) {
        // 再次拷贝
        components.push_back(path.substr(start)); 
    }
    return components;
}

// 调用示例
auto parts = split_path("/volumes/project_data/user/docs/report.docx");


```

**问题**：
1. 每个`substr()`调用触发堆分配（小字符串优化失效于>15字符）
2. 解析长路径`/a/b/c/d/e/f`产生6次内存分配+拷贝
3. 内存碎片加剧（尤其高频操作）

### 4.3 C++17实现

```c++
std::vector<std::string_view> split_path_sv(std::string_view path) {
    std::vector<std::string_view> components;
    size_t start = 0, end = 0;
    
    while ((end = path.find('/', start)) != std::string_view::npos) {
        if (end != start) {
            // 零拷贝：仅记录指针和长度
            components.emplace_back(path.data() + start, end - start); 
        }
        start = end + 1;
    }
    //最后一个单词
    if (start < path.length()) {
        // 零拷贝
        components.emplace_back(path.data() + start, path.length() - start); 
    }
    return components;
}

https://www.cppreference.com/w/cpp/string/basic_string_view/find.html
```


- 小思考：C++ std::string::npos 表示什么无符号类型？ 为什么不用-1 表示
![](https://s2.loli.net/2025/06/24/9giykQ8jLNzJbaD.png)


### 课后作业（可选）
![https://leetcode.cn/problem-list/string/](https://s2.loli.net/2025/06/24/sHMmuiSptXPckUB.png)


-------------------

## 最动人的作品，为自己而写，刚刚好打动别人

  

我在寻找一位积极上进的小伙伴，

一起参与**神奇早起 30 天改变人生计划**，发展个人事业，不妨 试试

  

 1️⃣关注公众号:后端开发成长指南(回复"面经"获取）获取过去我**全部面试录音**和大厂面试复盘攻略

![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)

2️⃣ **感兴趣的读者可以通过公众号获取老王的联系方式。**


 加入我的技术交流群Offer 来碗里 (回复"面经"获取），一起抱团取暖
![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)

本群目标是：
#### 抬头看天：走暗路、耕瘦田、进窄门、见微光

- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个**逃避问题**表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。


#### 低头走路：一次专注做好一个小事
- 不扫一屋 何以扫天下。
- 让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。


## ceph产品路线图


| 版本名称         | 主版本号 | 发布时间           | 停止维护时间         | 关键特性                                                                       |
| ------------ | ---- | -------------- | -------------- | -------------------------------------------------------------------------- |
| **Luminous** | 12.x | 2017-10-01     | 2020-03-01     | • **BlueStore 默认存储后端（取代 FileStore）**<br>• ceph-mgr 管理模块<br>• 支持 10k OSD 集群 |
| **Mimic**    | 13.x | 2018-05-01     | 2020-07-22     | • RGW 动态分片<br>• CephFS 多活 MDS                                              |
| **Nautilus** | 14.x | 2019-03-19     | 2021-06-30     | • 年度发布周期起点<br>• BlueStore 压缩（LZ4/Snappy）                                   |
| **Octopus**  | 15.x | 2020-03-23     | 2022-06-01     | • cephadm 部署工具<br>• RBD 镜像加密                                               |
| **Pacific**  | 16.x | 2021-03-31     | 2023-06-01     | • 跨集群同步<br>• CephFS 子目录配额                                                  |
| **Quincy**   | 17.x | 2022-04-19     | 2024-06-01     | • 全链路 RDMA 支持<br>• <font color="#245bdb">SPDK 加速 BlueStore</font>          |
| **Reef**     | 18.x | 2023-08-07     | 2025-08-01（预计） | • 服务网格（S3 代理）<br>• NVMe 感知调度                                               |
| **Squid**    | 19.x | 2024-03-31（预计） | 2026-06-01（预计） | • 分布式元数据索引（CephFS）<br>• QAT 硬件压缩加速                                         |
