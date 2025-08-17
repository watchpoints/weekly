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


# STD::Optional in C++: A Guide  
C++ 中的 STD：：Optional：指南

###  std::optional十万个为什么
- 为什么不用直接 构造函数创建一个实例？
- 为什么 `std::optional` 使用 placement new？
- union sizeof怎么计算的
- std::option::emplace  是什么含义
- 支持移动构操作吗？如何支持 栈上实例支持吗


### 优缺点
**零堆分配**：所有对象都在 inline 存储中构造，无需 `new/delete`。

### 数据结构 能满足特性吗

- 
```c++
template <typename T>  
struct optional_storage {  
	struct empty { };  //sizeof(empty)=1
	union {  
		empty _;  
		T value;  
	}; //这个定义不对，因为成员变量是指针
	bool has_value;  
};
```



### 接口能满足特性吗？


### 完整代码
```
template<typename T>
class optional {
private:
    // 用于占位，支持 constexpr 和无参数构造
    struct _Empty { };
    union {
        _Empty _empty;      // dummy member
        T       _value;     // 真正存储 T 的位置
    };
    bool _has = false;

public:
    constexpr optional() noexcept : _empty(), _has(false) { }

    ~optional() {
        reset();
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        reset();
        // placement new 在 _value 内联存储上构造 T
        new (&_value) T(std::forward<Args>(args)...);
        _has = true;
    }

    void reset() noexcept {
        if (_has) {
            _value.~T();   // 手动析构
            _has = false;
        }
    }

    constexpr bool has_value() const noexcept { return _has; }

    T&       operator*()       & { return _value; }
    const T& operator*() const & { return _value; }
    T*       operator->()      { return &_value; }
    const T* operator->() const { return &_value; }
};

```

### 案例1 
- https://en.cppreference.com/w/cpp/utility/optional/emplace.html

- Ceph 在缓存管理层多采用传统的裸指针（如 `Ptr`, `shared_ptr`）或其他方式（如状态标签、缓存条目本身和值类型）；
    
- `optional` 主要用于轻量可选值场景（如 RPC payload、临时配置），而缓存处理更偏向容器管理或引用语义，不使用 `optional<T>`。
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
