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

 介绍了 如何使用gitpod 断点调试 运行一个单元测试， 这样方面跟踪函数调用过程



本文是TiKV （版本7.0 ）源码阅读第二篇博客，

主要介绍 以一条raw_get读请求为例，介绍当前版本读请求的全链路执行流程。

目标

1. 通过断点调试了解RPC,KvService，Storage 之间分层关系。
2. 快照与Storage 关系？





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



说明：还有一个测试方式  使用官方提供的client-go/client-rust直接访问PD和TiKV



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



看完上面代码，我第一个疑问是 tikv 中的 region是 与key 是什么关系？

 Region 是数据分片的基本单元，这个概念看了还是不懂，不需要记这样概念。

继续提问

TiKV 使用 region 来划分数据，每个 region 包含一定范围的键值对。

接下里问题就是region与Storage 关系？



**Storage** 

代码路径：

tikv-master\src\storage\mod.rs

在 KVService 中， handle_request 宏将业务逻辑封装到了 future_raw_get函数中。

 Storage 模块的raw_get函数中

~~~
/// Get the value of a raw key.
    pub fn raw_get(
        &self,
        ctx: Context,
        cf: String,
        key: Vec<u8>,
    ) -> impl Future<Output = Result<Option<Vec<u8>>>> s
~~~



具体执行的任务主要包含以下两个工作：

- raw get 只需要调用 engine 的 `async_snapshot` 拿到数据库快照，
- 然后直接读取就可以.



下面是KvGet流程参考对比

TiKV作为gRPC的Server端，提供了KvGet接口的实现，相关调用堆栈为：

```arduino
+TiKV::kv_get (grpc-poll-thread)
 +future_get
  +Storage::get
   +Storage::snapshot (readpool-thread)
   +SnapshotStore::get
     +PointGetterBuilder::build
     +PointGetter::get
```

在一次KvGet调用中，函数执行流程会在grpc-poll-thread和readpool-thread中切换，

其中前者为gRPC的poll thread，请求在被路由到Storage层后，会根据读写属性路由到不同的线程池中，

只读语义的Get/Scan请求都会被路由到ReadPool中执行





**Storage 是什么？**



Storage 定义在 storage/mod.rs文件中，下面我们介绍下 Storage 几个重要的成员

- engine：代表的是底层的 KV 存储引擎，实际上就是 RaftKV。

- sched：事务调度器，负责并发事务请求的调度工作。

- read_pool：读取线程池，所有只读 KV 请求，包括事务的非事务的，如 raw get、txn kv get 等最终都会在这个线程池内执行。

  由于只读请求不需要获取 latches，所以为其分配一个独立的线程池直接执行，而不是与非只读事务共用事务调度器。

- RawAPI仅支持最基本的针对单Key操作的Set/Get/Del及Scan语义
- TxnAPI提供了基于ACID事务标准的接口，支持多Key写入的原子性

~~~

/// [`Storage`](Storage) implements transactional KV APIs and raw KV APIs on a
/// given [`Engine`]. An [`Engine`] provides low level KV functionality.

/// [`Engine`] has multiple implementations. When a TiKV server is running, a
/// [`RaftKv`](crate::server::raftkv::RaftKv) will be the underlying [`Engine`]
/// of [`Storage`]. The other two types of engines are for test purpose.
///
/// [`Storage`] is reference counted and cloning [`Storage`] will just increase
/// the reference counter. Storage resources (i.e. threads, engine) will be
/// released when all references are dropped.
///
/// Notice that read and write methods may not be performed over full data in
/// most cases, i.e. when underlying engine is
/// [`RaftKv`](crate::server::raftkv::RaftKv), which limits data access in the
/// range of a single region according to specified `ctx` parameter. However,
/// [`unsafe_destroy_range`](crate::server::gc_worker::GcTask::
/// UnsafeDestroyRange) is the only exception. It's always performed on the
/// whole TiKV.
///
/// Operations of [`Storage`](Storage) can be divided into two types: MVCC
/// operations and raw operations. MVCC operations uses MVCC keys, which usually
/// consist of several physical keys in different CFs. In default CF and write
/// CF, the key will be memcomparable-encoded and append the timestamp to it, so
/// that multiple versions can be saved at the same time. Raw operations use raw
/// keys, which are saved directly to the engine without memcomparable- encoding
/// and appending timestamp.

pub struct Storage<E: Engine, L: LockManager, F: KvFormat> {
    // TODO: Too many Arcs, would be slow when clone.
    engine: E,
    sched: TxnScheduler<E, L>,
    /// The thread pool used to run most read operations.
    //读取线程池，所有只读 KV 请求，包括事务的和非事务的，如 raw get、txn kv get 等最终都会在这个线程池内执行
    read_pool: ReadPoolHandle,
}
~~~



`storage` 结构实现了在给定的 [`Engine`] 上执行事务性键值（KV）API和原始键值（KV）API

从raw_get开始，复杂的先不看。



~~~
/// Get the value of a raw key.
pub fn raw_get(
    &self,
    ctx: Context,
    cf: String,
    key: Vec<u8>,
) -> impl Future<Output = Result<Option<Vec<u8>>>> {
~~~



### 3. Storage::raw_get流程分析



参考文章：

- PointGet的一生 https://tidb.net/blog/d6444c63

  

该方法主要执行以下几个步骤：
1. 从上下文中提取必要的信息，如优先级、任务元数据等。
2. 在一个异步块中执行命令，记录和检查相关指标和时间。
3. 创建快照上下文并获取快照。
4. 编码键、收集查询信息并从存储中获取键值。
5. 收集和记录统计数据及命令执行时间。
6. 最终返回结果。



~~~rust

/// 获取原始键的值。
/// 
/// 该方法接收一个上下文、列族cf和键key，返回一个Future，
/// 输出是一个包含结果的Option<Vec<u8>>类型。
pub fn raw_get(
    &self,
    ctx: Context,
    cf: String,
    key: Vec<u8>,
) -> impl Future<Output = Result<Option<Vec<u8>>>> {
 
    // 创建并启动带忙碌检查的读取池
    self.read_pool_spawn_with_busy_check(
        busy_threshold,
        async move {
            // 增加特定命令的计数
            KV_COMMAND_COUNTER_VEC_STATIC.get(CMD).inc();
            SCHED_COMMANDS_PRI_COUNTER_VEC_STATIC
                .get(priority_tag)
                .inc();

            // 检查API版本是否匹配
            Self::check_api_version(api_version, ctx.api_version, CMD, [&key])?;

            // 记录命令持续时间
            let command_duration = Instant::now();

            // 创建快照上下文
            let snap_ctx = SnapContext {
                pb_ctx: &ctx,
                ..Default::default()
            };

            // 获取快照
            let snapshot =
                Self::with_tls_engine(|engine| Self::snapshot(engine, snap_ctx)).await?;

            // 获取桶信息
            let buckets = snapshot.ext().get_buckets();

            // 创建原始存储
            let store = RawStore::new(snapshot, api_version);

            // 获取列族信息
            let cf = Self::rawkv_cf(&cf, api_version)?;

            // 内部命令执行块
            {
                // 记录起始时间
                let begin_instant = Instant::now();
                let mut stats = Statistics::default();

                // 编码原始键
                let key = F::encode_raw_key_owned(key, None);

                // 获取键值
                let r = store
                    .raw_get_key_value(cf, &key, &mut stats)
                    .map_err(Error::from);

}

~~~



克服语法问题

~~~rust
 // 获取快照
let snapshot = Self::with_tls_engine(|engine| Self::snapshot(engine, snap_ctx)).await?;
await? 用于等待异步操作完成并检查是否有错误发生。如果有错误，它将返回该错误，并使用.await?引发适当的错误处理

pub type Result<T> = std::result::Result<T, Error>;
pub type Callback<T> = Box<dyn FnOnce(Result<T>) + Send>;

#[inline]
fn with_tls_engine<R>(f: impl FnOnce(&mut E) -> R) -> R {
    // Safety: the read pools ensure that a TLS engine exists.
    unsafe { with_tls_engine(f) }
}

/// Get a snapshot of `engine`.
    fn snapshot(
        engine: &mut E,
        ctx: SnapContext<'_>,
    ) -> impl std::future::Future<Output = Result<E::Snap>> {
        kv::snapshot(engine, ctx)
            .map_err(txn::Error::from)
            .map_err(Error::from)
    }


~~~





`read_pool_spawn_with_busy_check` 的泛型函数，

它的作用是将一个异步任务（`future`）提交给一个读取池（`read_pool`），

并在提交前检查是否繁忙

~~~rust
// 定义一个泛型函数 read_pool_spawn_with_busy_check，它接受以下参数：
fn read_pool_spawn_with_busy_check<Fut, T>(
    &self, // &self 表示这是一个方法，它借用了当前实例。
    busy_threshold: Duration, // busy_threshold 参数，表示繁忙阈值。
    future: Fut, // future 参数，是一个异步任务。
    priority: CommandPri, // priority 参数，表示任务的优先级。
    task_id: u64, // task_id 参数，表示任务的唯一标识符。
    metadata: TaskMetadata<'_>, // metadata 参数，包含任务的元数据。
    resource_limiter: Option<Arc<ResourceLimiter>>, // resource_limiter 参数，表示资源限制器的可选引用。
) -> impl Future<Output = Result<T>> // 返回一个实现了 Future trait 的类型，其输出是 Result<T> 类型。
where
    Fut: Future<Output = Result<T>> + Send + 'static, // Fut 必须是可发送的（Send）并且 'static 生命周期，且其输出必须是 Result<T>。
    T: Send + 'static, // T 必须是可发送的（Send）并且 'static 生命周期。
{
    // 检查繁忙阈值，如果返回 Err，则构造一个错误并立即返回。
    if let Err(busy_err) = self.read_pool.check_busy_threshold(busy_threshold) {
        let mut err = kvproto::errorpb::Error::default();
        err.set_server_is_busy(busy_err); // 设置错误为服务器繁忙。
        return Either::Left(future::err(Error::from(ErrorInner::Kv(err.into()))));
    }

    // 如果检查通过，则将 future 任务提交给读取池。
    Either::Right(
        self.read_pool
            .spawn_handle(
                future, // 提交的异步任务。
                priority, // 任务的优先级。
                task_id, // 任务的唯一标识符。
                metadata, // 任务的元数据。
                resource_limiter, // 资源限制器的可选引用。
            )
            .map_err(|_| Error::from(ErrorInner::SchedTooBusy)) // 将 spawn_handle 可能的错误转换为统一的错误类型。
            .and_then(|res| future::ready(res)), // 使用 and_then 来处理 spawn_handle 的结果。
    )
}
~~~





## 小结

```arduino
+TiKV::kv_get (grpc-poll-thread)
 +future_get
  +Storage::get
   +Storage::snapshot (readpool-thread)
   +SnapshotStore::get
     +PointGetterBuilder::build
     +PointGetter::get
```





## 参考文档

- 【1】RaftStorage https://xieyu.github.io/blog/tikv/thread_local_engine.html
- 【2】PointGet的一生  https://tidb.net/blog/d6444c63
- 【3】[基于 Send 和 Sync 的线程安全](https://course.rs/advance/concurrency-with-threads/send-sync.html#基于-send-和-sync-的线程安全)







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

- [11] https://segmentfault.com/a/1190000044959813

- [12]PointGet的一生

     https://tidb.net/blog/d6444c63

  
