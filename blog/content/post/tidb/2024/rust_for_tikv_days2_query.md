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





本文是TiKV （版本7.0 ）源码阅读第二篇博客，将主要介绍 以一条raw_get读请求为例，介绍当前版本读请求的全链路执行流程。

Raw KV 系列接口是绕过事务直接操纵底层数据的接口，没有事务控制，比较简单





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





raw_get 于 [kvproto](https://github.com/pingcap/kvproto/blob/master/proto/tikvpb.proto#L20) 关系？



从 [kvproto](https://github.com/pingcap/kvproto/blob/master/proto/tikvpb.proto#L20) 对应的 `service Tikv` 中了解当前 TiKV 支持的 RPC 接口。



当我们要为 TiKV 添加一个新的 API 时，首先就要在 kvproto 项目中添加相关消息体的定义，并在这里添加相关代码。



TiKV 包含多个 gRPC service。其中，最重要的一个是 `KvService`，位于 `src/server/service/kv.rs` 文件中。

`KvService` 定义了 

- TiKV 的 `kv_get`，`kv_scan`，`kv_prewrite`，`kv_commit` 等事务操作的 API，用于执行 TiDB 下推下来的复杂查询和计算的 `coprocessor` API，以及 `raw_get`，`raw_put` 等 Raw KV API。

- `batch_commands` 接口则是用于将上述的接口 batch 起来，以优化高吞吐量的场景。

- 另外，TiKV 的 Raft group 各成员之间通信用到的 `raft` 和 `batch_raft` 接口也是在这里提供







**KVService**

文章原文：最常用的 KvGet 接口为例介绍读流程

在 KVService 中， 
handle_request 宏将业务逻辑封装到了 future_get 函数中。
在 future_get 函数中，
主要使用了 storage.get(req.take_context(), Key::from_raw(req.get_key()), req.get_version().into()) 
函数将请求路由到 Storage 模块去执行。



我copy重写

//handle_request!(raw_get, future_raw_get, RawGetRequest, RawGetResponse);



在 KVService 中， 
handle_request 宏将业务逻辑封装到了 future_get 函数中。
在 future_raw_get函数中，
主要使用了 storage.get(req.take_context(), Key::from_raw(req.get_key()), req.get_version().into()) 
函数将请求路由到 Storage 模块去执行。

- 定义类

~~~
/// Service handles the RPC messages for the `Tikv` service.
// 这是一个文档注释，说明 Service 结构体用于处理 `Tikv` 服务的 RPC 消息。
pub struct Service<E: Engine, L: LockManager, F: KvFormat> {
    // 定义 Service 结构体，它包含泛型参数 E, L, F，分别代表不同的组件或功能。
    cluster_id: u64,
    // Service 结构体的字段，用于存储集群 ID。

    store_id: u64,
    // Service 结构体的字段，用于存储存储 ID。

    /// Used to handle requests related to GC.
    // TODO: make it Some after GC is supported for v2.
    gc_worker: GcWorker<E>,
    // Service 结构体的字段，用于处理与垃圾收集（GC）相关的请求。
    // 注释中的 TODO 表示当前可能不支持 GC 的 v2 版本，待支持后应将其改为 Some。

    // For handling KV requests.
    storage: Storage<E, L, F>,
    // Service 结构体的字段，用于处理键值（KV）请求。
    
}
~~~



- handle_request  我看懂 之间kimi注释。

~~~rust
impl<E: Engine, L: LockManager, F: KvFormat> Tikv for Service<E, L, F> {
    handle_request!(kv_get, future_get, GetRequest, GetResponse, has_time_detail);
    handle_request!(raw_get, future_raw_get, RawGetRequest, RawGetResponse);
    handle_request!(
        raw_batch_get,
        future_raw_batch_get,
        RawBatchGetRequest,
        RawBatchGetResponse
    );
~~~



这段 Rust 代码定义了一个宏 `handle_request`，用于简化处理 RPC 请求的函数模板。

宏在 Rust 中是一种强大的工具，允许你编写更灵活和可复用的代码。下面是对这段宏代码的逐行注释解释：

```rust
// 定义一个宏 `handle_request`，它接受一组模式匹配的参数。
macro_rules! handle_request {
    // 第一个模式匹配，当提供四个参数时使用。
    ($fn_name: ident, $future_name: ident, $req_ty: ident, $resp_ty: ident) => {
        // 调用自身，但使用五个参数的模式，添加默认的时间细节参数 `no_time_detail`。
        handle_request!($fn_name, $future_name, $req_ty, $resp_ty, no_time_detail);
    };
    // 第二个模式匹配，当提供五个参数时使用。
    ($fn_name: ident, $future_name: ident, $req_ty: ident, $resp_ty: ident, $time_detail: tt) => {
        // 为每个请求生成一个处理函数，函数名由 `$fn_name` 指定。
        fn $fn_name(&mut self, ctx: RpcContext<'_>, req: $req_ty, sink: UnarySink<$resp_ty>) {
            // 检查集群 ID 是否匹配，如果不匹配则拒绝请求。
            reject_if_cluster_id_mismatch!(req, self, ctx, sink);
            // 将请求转发到代理。
            forward_unary!(self.proxy, $fn_name, ctx, req, sink);
            // 记录请求开始时间。
            let begin_instant = Instant::now();

            // 获取请求的来源和资源控制上下文。
            let source = req.get_context().get_request_source().to_owned();
            let resource_control_ctx = req.get_context().get_resource_control_context();
            // 初始化资源组优先级。
            let mut resource_group_priority = ResourcePriority::unknown;
            // 如果存在资源管理器，则使用它来消费惩罚并获取资源组优先级。
            if let Some(resource_manager) = &self.resource_manager {
                resource_manager.consume_penalty(resource_control_ctx);
                resource_group_priority = ResourcePriority::from(resource_control_ctx.override_priority);
            }
            // 增加资源组计数器。
            GRPC_RESOURCE_GROUP_COUNTER_VEC
                .with_label_values(&[resource_control_ctx.get_resource_group_name(), resource_control_ctx.get_resource_group_name()])
                .inc();
            // 调用异步函数 `$future_name` 来处理请求。
            let resp = $future_name(&self.storage, req);
            // 创建一个异步任务来处理响应。
            let task = async move {
                // 等待异步响应完成。
                let resp = resp.await?;
                // 计算经过时间并设置到响应中。
                let elapsed = begin_instant.saturating_elapsed();
                set_total_time!(resp, elapsed, $time_detail);
                // 将响应发送回客户端。
                sink.success(resp).await?;
                // 观察请求持续时间。
                GRPC_MSG_HISTOGRAM_STATIC
                    .$fn_name
                    .get(resource_group_priority)
                    .observe(elapsed.as_secs_f64());
                // 记录请求来源的指标。
                record_request_source_metrics(source, elapsed);
                // 返回成功结果。
                ServerResult::Ok(())
            }
            // 处理异步任务中的错误。
            .map_err(|e| {
                // 日志记录网络错误。
                log_net_error!(e, "kv rpc failed";
                    "request" => stringify!($fn_name)
                );
                // 增加失败计数器。
                GRPC_MSG_FAIL_COUNTER.$fn_name.inc();
            })
            // 映射结果以满足函数签名要求。
            .map(|_| ());
            // 在 RpcContext 上下文中异步执行任务。
            ctx.spawn(task);
        }
    }
}
```

宏定义中的 `ident` 和 `tt` 是宏的模式匹配中的标记，分别表示标识符和文本标记。`ident` 用于捕获变量名或其他标识符，而 `tt` 用于捕获代码片段或文本。

宏的两个模式分别处理不同数量的参数。第一个模式是一个快捷方式，当调用者只提供四个参数时，自动添加一个默认的第五个参数。第二个模式是实际生成函数代码的部分，它使用捕获的参数来创建一个处理特定请求的函数。

注意，宏中的 `!` 操作符用于调用其他宏，例如 `reject_if_cluster_id_mismatch!` 和 `forward_unary!`，这些宏在代码中没有显示定义，可能是在其他地方定义的。

此外，宏代码中使用了异步代码块 `async move`，这允许在异步任务中捕获外部的变量。`await?` 用于等待异步操作的结果，并在发生错误时提前返回。最后，`ctx.spawn(task);` 将异步任务添加到 RpcContext 的执行器中，以便异步执行。



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

Storage 模块位于 Service 与底层 KV 存储引擎之间，主要负责事务的并发控制。

TiKV 端事务相关的实现都在 Storage 模块中。



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
