---
title: "TiKV 源码阅读三部曲（二）读流程"
date: 2024-07-8
description: "Tidb"
draft: false
tags: ["Tidb"] 
---



[TiKV](https://github.com/tikv/tikv) 是一个支持事务的分布式 Key-Value 数据库，目前已经是 [CNCF 基金会](https://www.cncf.io/projects/) 的顶级项目。

作为一个新同学，需要一定的前期准备才能够有能力参与 TiKV 社区的代码开发，包括但不限于学习 Rust 语言，

理解 TiKV 的原理和在前两者的基础上了解熟悉 TiKV 的源码。

历史文章：

 [tidb可用vscode调试，请问tikv使用vscode源码调试，执行单元测试吗？](https://asktug.com/t/topic/1028967)





本文是TiKV （版本7.0 ）源码阅读第二篇博客，将主要介绍 以一条读请求为例，介绍当前版本读请求的全链路执行流程。





## 读流程



如果要做到以上读写路径的全链路追踪，最简单的方法便是从集成测试里面寻找一些 case，接着从 Service 模块开始打断点，之后执行调试即可。

在这里推荐 `integrations/server/kv_service.rs` 中的测试，里面的 test 都会构造 `TiKVClient` 发送真实的 RPC 请求

且服务端也基本不包含 Mock 组件，可以完整的去追踪一条 RPC 的全链路流程。



~~~rust
#[test_case(test_raftstore::must_new_cluster_and_kv_client)]
#[test_case(test_raftstore_v2::must_new_cluster_and_kv_client)]
fn test_rawkv()

let (_cluster, client, ctx) = new_cluster();
let v0 = b"v0".to_vec();
let v1 = b"v1".to_vec();
let (k, v) = (b"key".to_vec(), b"v2".to_vec());
    
// Raw get
let mut get_req = RawGetRequest::default();
get_req.set_context(ctx.clone());
get_req.key = k.clone();
let get_resp = client.raw_get(&get_req).unwrap();
assert_eq!(get_resp.value, v0);
~~~







从 [kvproto](https://github.com/pingcap/kvproto/blob/master/proto/tikvpb.proto#L20) 对应的 `service Tikv` 中了解当前 TiKV 支持的 RPC 接口。







https://github.com/pingcap/kvproto/blob/master/proto/tikvpb.proto#L20







# help

[1]https://blog.csdn.net/weixin_42241611/article/details/130080867



## tod02:学习资料

- https://docs.pingcap.com/zh/tidb/stable/tikv-overview

- 阅读文章：https://asktug.com/t/topic/693645

- 【源码合集】TiKV 源码阅读三部曲

  https://mp.weixin.qq.com/s/HgflwnZZMHXaIsFV3PdGAg

- 阅读  Rust 参考手册 中文版
  https://rustwiki.org/zh-CN/reference/expressions/if-expr.html

- TiKV 源码阅读三部曲（一）重要模块

本小节将简单介绍 KVService 及其启动流程，并顺带介绍 TiKV 若干重要结构的初始化流程

- TiKV 源码阅读三部曲（二）读流程

  https://cn.pingcap.com/blog/tikv-source-code-reading-read/

~~~\

// Raw get
let mut get_req = RawGetRequest::default();
get_req.set_context(ctx.clone());
get_req.key = k.clone();
let get_resp = client.raw_get(&get_req).unwrap();
assert_eq!(get_resp.value, v0);
~~~

