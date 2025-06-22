---
title: c++周刊（c++ weekly):Ep01 - std::string_view
date: 2025-06-20
description: do book
draft: false
tags: 
categories:
  - do book
---

点击<font color="#245bdb">蓝色</font>关注。

><font color="#c0504d">文末有福利，先到先得</font>


#知识地图 #最佳实践 #CPP17


### 1.  什么是string_view？

> std::string_view` is a C++17 library type that provides a lightweight, non-owning reference to a sequence of characters

C++17中我们可以使用std::string_view来获取一个字符串的视图，
字符串视图并不真正的创建或者拷贝字符串，
而只是拥有一个字符串的查看功能。std::string_view比std::string的性能要高很多

访问：https://www.cppreference.com/w/cpp/string/basic_string_view.html

### 2. 使string_view 有啥好处

- ✅ 高频操作字符串,零拷贝 ,性能提高10%
- ✅开发者来说：修改一行代码,代码工作量不复杂，双手赞同 
- ✅对领导来说：方案简单，收益大, 绝对同意 

|**指标**|传统实现 (C++11)|`string_view`优化|收益|
|---|---|---|---|
|内存分配次数|O(n)|**0**|完全消除|
|内存拷贝量|路径总长度|**0字节**|零拷贝|
|解析延迟(1k QPS)|4.2μs/op|1.1μs/op|降低74%|
|CPU缓存命中率|83%|**97%**|+14%|

### 3.  举例行业案例



![](https://s2.loli.net/2025/06/21/yiE6YKJQM98Tbs2.png)


| **项目**        | C++标准    | 字符串优化方案                | 是否用 `string_view` |
| ------------- | -------- | ---------------------- | ----------------- |
| **RocksDB**   | C++17 可选 | 自有 `Slice` 类           | 有限（非核心）           |
| **CephFS**    | C++17 强制 | `const char* + size_t` | 基本未用              |
| **OceanBase** | C++11    | 自有 `ObString`          | 未使用               |
| **TiKV**      | C++17 强制 | 广泛使用                   | ✅ (Rust FFI 桥接)   |

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
 ### **CephFS**
 - 使用 c++17 提供 string_view 代替string
 -  场景 元数据 MDS 查找一个文件时候目录遍历 


### 4. 为了证明你写代码 能力，使用string_view 为系统工程师设计一个题目
>小提示：
>1. 刷题 不光有算法题目 还有系统设计题目,
>2. 掌握数据结构，掌握算法，最后应用到实际项目才算通过，请动手写一个组件或者基础库 API。
>3. 别蒙，都是经常遇到小问题，一点都不难
>   
###  4.1 CephFS路径解析
模块： src/mds/
功能：访问一个文件之前必须路径解析
输入：/mnt/icfs/dir/report.docx`  
输出:逐级解析路径分量（mnt, icfs, dir, report）]
代码位置：MDCache::path_traverse
版本：Reef


 Ceph 版本演进表格，包含发布时间、生命周期和关键特性：

| 版本名称         | 主版本号 | 发布时间           | 停止维护时间         | 关键特性                                                                       |
| ------------ | ---- | -------------- | -------------- | -------------------------------------------------------------------------- |
| **Luminous** | 12.x | 2017-10-01     | 2020-03-01     | • **BlueStore 默认存储后端（取代 FileStore）**<br>• ceph-mgr 管理模块<br>• 支持 10k OSD 集群 |
| **Mimic**    | 13.x | 2018-05-01     | 2020-07-22     | • RGW 动态分片<br>• CephFS 多活 MDS                                              |
| **Nautilus** | 14.x | 2019-03-19     | 2021-06-30     | • 年度发布周期起点<br>• BlueStore 压缩（LZ4/Snappy）                                   |
| **Octopus**  | 15.x | 2020-03-23     | 2022-06-01     | • cephadm 部署工具<br>• RBD 镜像加密                                               |
| **Pacific**  | 16.x | 2021-03-31     | 2023-06-01     | • 跨集群同步<br>• CephFS 子目录配额                                                  |
| **Quincy**   | 17.x | 2022-04-19     | 2024-06-01     | • **全链路 RDMA 支持<br>• SPDK 加速 BlueStore**                                   |
| **Reef**     | 18.x | 2023-08-07     | 2025-08-01（预计） | • 服务网格（S3 代理）<br>• NVMe 感知调度                                               |
| **Squid**    | 19.x | 2024-03-31（预计） | 2026-06-01（预计） | • 分布式元数据索引（CephFS）<br>• QAT 硬件压缩加速                                         |

 核心演进趋势总结：
1. **存储后端革新**：
   - Luminous (v12)：BlueStore 成为默认后端，性能提升 2-3 倍
   - Quincy (v17)：SPDK 集成，NVMe 延迟降低 60%
   - Squid (v19)：硬件加速压缩（QAT）

2. **管理复杂度降低**：
   - Nautilus (v14)：确立年度发布周期
   - Octopus (v15)：cephadm 容器化部署工具

3. **扩展性与性能**：
   - Mimic (v13)：CephFS 多活 MDS，元数据吞吐提升 5 倍
   - Pacific (v16)：跨集群同步，支持多区域部署
   - Reef (v18)：NVMe 感知调度，SSD 寿命延长 30%

4. **生命周期规律**：
   - 平均维护周期：25-27 个月
   - 当前推荐生产版本：Quincy (v17) LTS
   - 最新稳定版：Reef (v18)


### 4.2  传统实现（C++11）



```c++

// 存在多次内存拷贝
std::vector<std::string> split_path(const std::string& path) {
    std::vector<std::string> components;
    size_t start = 0, end = 0;
    
    while ((end = path.find('/', start)) != std::string::npos) {
        if (end != start) {
            // 触发拷贝：构造临时string对象
            components.push_back(path.substr(start, end - start)); 
        }
        start = end + 1;
    }
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

### 4.3 C++17优化实现

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
    if (start < path.length()) {
        // 零拷贝
        components.emplace_back(path.data() + start, path.length() - start); 
    }
    return components;
}

https://www.cppreference.com/w/cpp/string/basic_string_view/find.html
```
### 参考 
- ## [RDMA 架构与实践(技术详解（一）：RDMA概述](https://www.cnblogs.com/bandaoyu/p/16752034.html "发布于 2022-10-04 01:21")
- https://www.cnblogs.com/bandaoyu/p/16752034.html







----------------我是黄金分割线-----------------------------
## 最动人的作品，为自己而写，刚刚好打动别人

  

我在寻找一位积极上进的小伙伴，

一起参与**神奇早起 30 天改变人生计划**，发展个人事业，不妨 试试

  

 1️⃣关注公众号:后端开发成长指南(回复面经获取）获取过去我**全部面试录音**和大厂面试复盘攻略

![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)

2️⃣ **感兴趣的读者可以通过公众号获取老王的联系方式。**

  

 加入我的技术交流群Offer 来碗里 (回复"面经"获取），一起抱团取暖

![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)




#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，别人投入时间完成任务，别人采取措施解决问题了，不要不贪别人功劳，
- 但是不要给自己这样假设：别人完成就等着自己完成了，大家一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现， 
- 别人不这么假设，至少kpi上不会写成自己的，至少晋升不是你，裁员淘汰是，你的整个公司ceo，整个部门总裁，整个领导不帮助一下的，他们不这么想 ，你什么没做，战略是别人10年一点带你研究的多难，项目拆分别人10年完成多少问题，项目实现10年安排组织一点点完成多少bug，多少代码，是不要给自己这样假设：你等了看了观察10年什么做 ，0 贡献，
-  但是不要给自己这样假设，别人全部市场，别人全部市场，别人占据全部客户，一切重要无比，你太差，太才，思考不行，沟通不行，认知不行，去tmd，给别人丢脸。这个方面我无法控制，在这方面经历任何问题应该的。
- 我控制 的事情是 我必须亲自了解行业遇到难题，了解有什么需求，行业解决方案，我可以从三万英尺看问题，像周围人学习，像免费公开英文资料学习，从模仿开始。然后免费公开。我要通过技术拿到百万年薪P7职务，我必须糊涂混沌中走出来
-  目标：拿百万年 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 现状：缺乏实战，渴望提升动手能力公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 成为优秀完成任务，成为团队、公司都认可的核心骨干。优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量

##### 低头走路：
- 一次专注做好一个小事。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。
- 我控制 的事情是 我通过写自己代码拿到百万收益。代码就是杠杆，我必须创造可以运行在2c2g云主机小而美产品出来（服务普通人），而不是运行构建至少10台*64cpu* 300g内存物理机大而全项目（领航者，超越其他产品，出货全球N1，这个还是有停留有限斗争游戏，为top 10人企业服务）我必须糊涂混沌中走出来