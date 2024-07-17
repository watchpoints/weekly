---
title: "TiKV 源码阅读三部曲（二）读流程"
date: 2024-07-8
description: "Tidb"
draft: false
tags: ["Tidb"] 
---

 成为tikv贡献者第二天：读请求 全链路跟踪

[TiKV](https://github.com/tikv/tikv) 是一个支持事务的分布式 Key-Value 数据库，

需要一定的前期准备才能够有能力参与 TiKV 社区的代码开发





回顾：

 成为tikv贡献者第一天：搭建环境

 介绍了 如何使用gitpod 设则断点方式 运行一个单元测试， 这样方面跟踪函数调用过程



本文是TiKV （版本7.0 ）源码阅读第二篇博客，

主要介绍 以一条raw_get读请求为例，介绍当前版本读请求的全链路执行流程。

 Raw KV 系列接口是绕过事务直接操纵底层数据的接口，没有事务控制，比较简单。



## 读流程



如果要做到以上读写路径的全链路追踪，最简单的方法便是从集成测试里面寻找一些 case，

接着从 Service 模块开始打断点，之后执行调试即可。

在这里推荐 `integrations/server/kv_service.rs` 中的测试，里面的 test 都会构造 `TiKVClient` 发送真实的 RPC 请求

且服务端也基本不包含 Mock 组件，可以完整的去追踪一条 RPC 的全链路流程。

### 1. 开始debug

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





### 2.  kvproto 中RawGet 怎么和future_raw_get 管理起来的？

   ![image-20240716212941824](D:\db\daily-interview\blog\content\post\tidb\2024\assets\image-20240716212941824.png)

​    kvproto 是 Protocol buffer files for TiKV 定义了RPC服务接口



~~~
代码地址：
 https://github.com/pingcap/kvproto/blob/master/proto/tikvpb.proto
 https://tikv.github.io/doc/kvproto/tikvpb/trait.Tikv.html#tymethod.raw_get
 
service Tikv {
   // Raw commands; no transaction support.
   rpc RawGet(kvrpcpb.RawGetRequest) returns (kvrpcpb.RawGetResponse) {}
}
//语法回顾：
定义服务(Service)  如果想要将消息类型用在RPC(远程方法调用)系统中，可以在.proto文件中定义一个RPC服务接口

~~~





TiKV 包含多个 gRPC service。其中，最重要的一个是 `KvService`，位于 `src/server/service/kv.rs` 文件中。



**这里到了`KvService` 层**

- 文件

  kv.rs

- 定义类和方法。

~~~
// Service handles the RPC messages for the `Tikv` service.

// 这是一个文档注释，说明 Service 结构体用于处理 `Tikv` 服务的 RPC 消息。
pub struct Service<E: Engine, L: LockManager, F: KvFormat> {
    // 定义 Service 结构体，它包含泛型参数 E, L, F，分别代表不同的组件或功能。
    cluster_id: u64,
    // Service 结构体的字段，用于存储集群 ID。
    store_id: u64,
    // For handling KV requests.
    storage: Storage<E, L, F>,
    // Service 结构体的字段，用于处理键值（KV）请求。
    
}
 // 
impl<E: Engine, L: LockManager, F: KvFormat> Tikv for Service<E, L, F> {
    handle_request!(kv_get, future_get, GetRequest, GetResponse, has_time_detail);
    handle_request!(raw_get, future_raw_get, RawGetRequest, RawGetResponse);

~~~



- 宏 `handle_request`，用于简化处理 RPC 请求的函数模板

> 说明：rust  async/await  这一块不太明白 
>
> 阅读文档：https://course.rs/advance/async/future-excuting.html



宏在 Rust 中是一种强大的工具，允许你编写更灵活和可复用的代码。下面是对这段宏代码的逐行注释解释：

```rust
// 定义一个宏 `handle_request`，它接受一组模式匹配的参数。
macro_rules! handle_request {
    // 第一个模式匹配，当提供四个参数时使用。
    ($fn_name: ident, $future_name: ident, $req_ty: ident, $resp_ty: ident) => {

    };
}
```



- 定义方法

```r
// 定义一个泛型异步函数 future_raw_get，它接受三个泛型参数 E, L, F，分别约束为 Engine, LockManager, KvFormat。
fn future_raw_get<E: Engine, L: LockManager, F: KvFormat>(
    // 函数接受一个对 Storage<E, L, F> 类型的引用和一个 RawGetRequest 类型的可变引用。
    storage: &Storage<E, L, F>,
    mut req: RawGetRequest,
) -> impl Future<Output = ServerResult<RawGetResponse>> {
    // 调用 storage 的 raw_get 方法，传入 req 的上下文、列族和键，并获取返回值。
    let v = storage.raw_get(req.take_context(), req.take_cf(), req.take_key());

    // 定义一个异步闭包，它将异步执行。
    // https://rust-lang.github.io/async-book/03_async_await/01_chapter.html
    async move {
        // 等待 v 的 Future 完成并获取结果。
        let v = v.await;
        // 创建一个 RawGetResponse 的默认实例。
        let mut resp = RawGetResponse::default();
        // 尝试从 v 中提取区域错误，如果有，则设置到响应中。
        if let Some(err) = extract_region_error(&v) {
            resp.set_region_error(err);
        } else {
            // 根据 v 的结果进行不同的处理。
            match v {
                // 如果获取到值，将其设置到响应的值字段中。
                //https://doc.rust-lang.org/std/result/
                Ok(Some(val)) => resp.set_value(val),
                // 如果没有找到值，设置响应的 not_found 字段为 true。
                Ok(None) => resp.set_not_found(true),
                // 如果发生错误，将错误信息格式化后设置到响应的 error 字段中。
                Err(e) => resp.set_error(format!("{}", e)),
            }
        }
        // 返回构建好的响应。
        Ok(resp)
    }
}
```



### Storage 



> 根据问题去阅读：raw_get 是怎么被调用的?   
>
> Raw KV 系列接口是绕过事务直接操纵底层数据的接口 获取快照 然后调用接口



在 KVService 中， handle_request 宏将业务逻辑封装到了 future_get 函数中。

在 future_get 函数中，主要使用了 `storage.get(req.take_context(), Key::from_raw(req.get_key()), req.get_version().into())` 函数将请求路由到 Storage 模块去执行。



Storage 模块位于 Service 与底层 KV 存储引擎之间，主要负责事务的并发控制。TiKV 端事务相关的实现都在 Storage 模块中。



Storage 定义在 storage/mod.rs文件中，下面我们介绍下 Storage 几个重要的成员

- engine：代表的是底层的 KV 存储引擎，实际上就是 RaftKV。
- sched：事务调度器，负责并发事务请求的调度工作。
- read_pool：读取线程池，所有只读 KV 请求，包括事务的非事务的，如 raw get、txn kv get 等最终都会在这个线程池内执行。由于只读请求不需要获取 latches，所以为其分配一个独立的线程池直接执行，而不是与非只读事务共用事务调度器。



raw get 只需要调用 engine 的 `async_snapshot` 拿到数据库快照，然后直接读取就可以





在 Storage 模块的raw_get函数中，所有的 task 都会被 spawn 到 readPool 中执行，

具体执行的任务主要包含以下两个工作：

- raw get 只需要调用 engine 的 `async_snapshot` 拿到数据库快照，
- 然后直接读取就可以







/////////////////////////////////////////////最新模块发生了变化，不能100%对应//////////////

有关 3.x 版本的 Storage 模块可以参照 [TiKV 源码解析系列文章（十一）Storage - 事务控制层](https://cn.pingcap.com/blog/tikv-source-code-reading-11)。

- engine

TiKV 把底层 KV 存储引擎抽象成一个 Engine trait

定义见 `storage/kv/mod.rs`。Engine trait 主要提供了读和写两个接口，分别为 `async_snapshot` 和 `async_write`

`async_snapshot` 通过回调的方式把数据库的快照返回给调用者。

//////////////////////////////////////////////////////////////////////////////////////////





readPool 是什么？

代码路径：

tikv-master\src\storage\mod.rs



~~~

pub struct Storage<E: Engine, L: LockManager, F: KvFormat> {
    // TODO: Too many Arcs, would be slow when clone.
    engine: E,
    sched: TxnScheduler<E, L>,
    /// The thread pool used to run most read operations.
    //读取线程池，所有只读 KV 请求，包括事务的和非事务的，如 raw get、txn kv get 等最终都会在这个线程池内执行
    read_pool: ReadPoolHandle,
}
~~~





代码路径：

tikv-master\src\storage\mod.rs

~~~
/// Get the value of a raw key.
pub fn raw_get(
    &self,
    ctx: Context,
    cf: String,
    key: Vec<u8>,
) -> impl Future<Output = Result<Option<Vec<u8>>>> {
~~~







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
  
  - TiKV 线程池性能调优

https://docs.pingcap.com/zh/tidb/stable/tune-tikv-thread-performance
