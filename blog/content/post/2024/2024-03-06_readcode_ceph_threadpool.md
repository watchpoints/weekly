---
title: "源码阅读：请你设计一个线程池"
date: 2024-03-03
draft: false
tags: ["tools"]
---











![image.png](https://s2.loli.net/2024/03/06/1d4fyhMsCV2w3He.png)

在温和的春夜里，路灯投下淡淡的黄光，小王沿着柳树成荫的街道散步，思绪随着微风轻轻飘散。不远处，他的邻居老王也正慢慢踱步，一如既往地享受着夜的安宁。

“晚上好，老王！”小王热情地打招呼。

老王抬头，嘴角上扬，开心地回应：“晚上好，小王。夜晚散步，别有一番风味啊。”

两人自然地并肩走着，闲聊起邻里间的琐事和生活的点点滴滴。随着对话的深入，





小王提问： 来我熟记了各种线程池把八股文 为什么，还是不行



老王回答： 从基本的一个调用开始，亲自用一下，观察你发现什么问题





我来演示一下基本用法

~~~

BlueStore.cc (src\os\bluestore) line 8472 : class ShallowFSCKThreadPool : public ThreadPool

 const size_t thread_count = cct->_conf->bluestore_fsck_quick_fix_threads;
		//定义工作队列
    typedef ShallowFSCKThreadPool::FSCKWorkQueue<256> WQ;
    std::unique_ptr<WQ> wq(
      new WQ(
        "FSCKWorkQueue",
        (thread_count ? : 1) * 32,
        this,
        sb_info_lock,
        sb_info,
	sb_ref_counts,
        repairer));
    //定义一个线程池，指定线程个数thread_count
    ShallowFSCKThreadPool thread_pool(cct, "ShallowFSCKThreadPool", "ShallowFSCK", thread_count);
    //定义线程池的工作队列,这个例子工作队列放在外面
    thread_pool.add_work_queue(wq.get());
    if (depth == FSCK_SHALLOW && thread_count > 0) {
      //not the best place but let's check anyway
      ceph_assert(sb_info_lock);
      thread_pool.start();
    }
    
    
class ShallowFSCKThreadPool : public ThreadPool
{
public:
  ShallowFSCKThreadPool(CephContext* cct_, std::string nm, std::string tn, int n) :
    ThreadPool(cct_, nm, tn, n) {
  }

template <size_t BatchLen>
struct FSCKWorkQueue : public ThreadPool::WorkQueue_
~~~





我在来演示一个例子



~~~
// ThreadPoolSingleton是一个继承自ThreadPool的单例类
class ThreadPoolSingleton : public ThreadPool {
public:
  // 显式的构造函数，构造时会初始化父类ThreadPool并启动线程池
  explicit ThreadPoolSingleton(CephContext *cct)
    : ThreadPool(cct, "librbd::ImageUpdateWatchers::thread_pool", "tp_librbd",
                 1) {
    start(); // 调用ThreadPool的start方法启动线程池
  }
  // 重写的析构函数，销毁对象时停止线程池
  ~ThreadPoolSingleton() override {
    stop(); // 调用ThreadPool的stop方法停止线程池
  }
};

// 声明一个指向ContextWQ类型（工作队列）的指针，默认初始化为nullptr
ContextWQ *m_work_queue = nullptr;

// 创建一个工作队列的方法
void create_work_queue() {
  if (m_work_queue != nullptr) {
    return; // 如果工作队列已经存在，则不做任何操作
  }
  // 查找或者创建一个ThreadPoolSingleton类型的单例对象作为线程池
  auto& thread_pool = m_cct->lookup_or_create_singleton_object<
    ThreadPoolSingleton>("librbd::ImageUpdateWatchers::thread_pool",
                         false, m_cct);
  // 创建一个新的ContextWQ对象作为工作队列，并使用配置中定义的操作线程超时设置
  m_work_queue = new ContextWQ("librbd::ImageUpdateWatchers::work_queue",
                               ceph::make_timespan(
                                 m_cct->_conf.get_val<uint64_t>("rbd_op_thread_timeout")),
                               &thread_pool);
}

// 销毁工作队列的方法
void destroy_work_queue() {
  if (m_work_queue == nullptr) {
    return; // 如果工作队列不存在，则不做任何操作
  }
  m_work_queue->drain(); // 清空工作队列
  delete m_work_queue; // 删除工作队列对象
}


// 定义一个函数send_notify，用于发送通知
// handle是要通知的句柄，watcher是更新监视器的上下文
void send_notify(uint64_t handle, UpdateWatchCtx *watcher) {
    // 创建一个新的LambdaContext对象，该对象封装了一个lambda表达式
    // 当调度给定的Context时，lambda表达式将被调用
    Context *ctx = new LambdaContext(
      [this, handle, watcher](int r) { // lambda捕获this指针、handle和watcher
        handle_notify(handle, watcher); // 在lambda内部调用handle_notify函数处理通知
      });

    m_work_queue->queue(ctx, 0); // 将新创建的LambdaContext对象添加到工作队列中，准备执行
  }
  
创建了一个 LambdaContext 对象，这个对象接收一个 lambda 表达式，用于异步执行完成通知的操作。
最终，通过 queue 方法将这个上下文添加到工作队列中以便执行。
这里 lambda 表达式捕获了 this 指针、handle和watcher的值，并在后台线程中调用 handle_notify 函数。
~~~



小王提问：我看到都是继承，里面有工作队列和放入任务，怎么被线程调用呢



老王回答：线程池 统一提供接口



1.3   线程池的执行函数
        函数worker为线程池的执行函数：

     voidThreadPool::worker(WorkThread *wt)
    
        其处理过程如下：
    
        1）首先检查_stop标志，确保线程池没有关闭。
    
        2）调用函数join_old_threads把旧的工作线程释放掉。检查如果线程数量大于配置的数量_num_threads，就把当前线程从线程集合中删除，并加入_old_threads队列中，并退出循环。
    
        3）如果线程池没有暂时中止，并且work_queues不为空，就从last_work_queue开始，遍历每一个工作队列，如果工作队列不为空，就取出一个item，调用工作队列的处理函数做处理。      




小王提问：工作队列 为什么还是继承，难道任务 处理，存储 可以采用不同形式？



老王回答：虚基类 WorkQueue 提供了结构 根据不同需求，采取不同结构

### 虚基类 WorkQueue_

```c
struct WorkQueue_ {
    std::string name;
    time_t timeout_interval, suicide_interval;
    WorkQueue_(std::string n, time_t ti, time_t sti)
      : name(std::move(n)), timeout_interval(ti), suicide_interval(sti)
    { }
    virtual ~WorkQueue_() {}
    /// Remove all work items from the queue.
    virtual void _clear() = 0;
    /// Check whether there is anything to do.
    virtual bool _empty() = 0;
    /// Get the next work item to process.
    virtual void *_void_dequeue() = 0;   //取出待处理的items
    /** @brief Process the work item.
     * This function will be called several times in parallel
     * and must therefore be thread-safe. */
    virtual void _void_process(void *item, TPHandle &handle) = 0;
    /** @brief Synchronously finish processing a work item.
     * This function is called after _void_process with the global thread pool lock held,
     * so at most one copy will execute simultaneously for a given thread pool.
     * It can be used for non-thread-safe finalization. */
    virtual void _void_process_finish(void *) = 0;
  };
```



### 四类 work_queue

- BatchWorkQueue
  - 每次可以取出多个待处理item
  - 该WorkQueued的item存放容器需要自行定义
  - 需要自行实现如下接口（主要函数）：
    - `virtual void _dequeue(std::list<T*> *) = 0`: 如何从队列中work_queue中拿出items
    - `virtual bool _enqueue(T *) = 0`: 入队列接口
    - `virtual void _process(const std::list<T*> &items, TPHandle &handle) = 0`: 批处理接口
- WorkQueueVal
  - 适用于处理原始值类型或者小对象
  - 将T类型item的值存储队列
  - 存储T类型值的容器需要自行实现
  - 处理缓存容器已经实现，用于存在中间值：
    - std::list<U> to_process; //待处理list， 从放入_void_dequeue()拿出的元素U，每次存入一个
    - std::list<U> to_finish; //_void_process_finish会处理该list中的元素，每次处理一个
  - 需要自行实现如下接口：
    - `bool _empty() override = 0`: 判断容器非空
    - `virtual void _enqueue(T) = 0;`: 入队列接口
    - `virtual void _process(U u, TPHandle &) = 0;`: 处理元素U的函数
- WorkQueue
  - 适用于处理大对象或者动态申请内存的对象
  - 存储容器需要自行实现
  - 需要自行实现如下接口：
    - `virtual bool _enqueue(T *) = 0;`: 入workqueue接口
    - `virtual T *_dequeue() = 0;`： 取work_queue item接口
    - `virtual void _process(T *t, TPHandle &) = 0;` : item处理接口
- PointerWQ
  - 适用于处理大对象或者动态申请内存的对象，比WorkQueue更加方便，但是没有WorkQueue抽象
  - 存储容器已经实现：`std::list<T *> m_items`
  - 只需要实现`virtual void process(T *item) = 0;`, 用于item处理

### 可直接使用的两种实现

- **class GenContextWQ **: public ThreadPool::WorkQueueVal<GenContext<ThreadPool::TPHandle&>*>
- **class ContextWQ** : public ThreadPool::PointerWQ<Context>



- 创建队列结构

```c
class New_WQ : public ThreadPool::PointerWQ<Context>
{
    public:
        New_WQ(const std::string &name, time_t ti, ThreadPool *tp)
            : ThreadPool::PointerWQ<Context>(name, ti, 0, tp) {
                this->register_work_queue();
            }
        void process(Context *fin) override;
};

void New_WQ::process(Context *fin)
{
    fin->complete(0);
}
```

- 启动线程池

```c
1. 创建ThreadPool
thread_pool = new ThreadPool(cct, "thread_pool", "daemon_tp", g_conf()->rep_thread_pool_nr);
thread_pool->start();

2. 创建队列
work_queue = new New_WQ("daemon", 60, thread_pool);
```

- 投递任务

```c
Context *ctx = new Test_TheadPool();
work_queue->queue(ctx);
```


### 学习方法

~~~
 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

 二、这个技术的优势和劣势分别是什么 


 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

 四、技术的组成部分和关键点。

 五、技术的底层原理和关键实现

 六、已有的实现和它之间的对比
~~~









