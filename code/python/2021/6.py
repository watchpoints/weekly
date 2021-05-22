from time import sleep, time


def demo1():
    """
    假设我们有三台洗衣机, 现在有三批衣服需要分别放到这三台洗衣机里面洗.
    """
    
    def washing1():
        sleep(3)  # 第一台洗衣机, 需要洗3秒才能洗完 (只是打个比方)
        print('washer1 finished')  # 洗完的时候, 洗衣机会响一下, 告诉我们洗完了
    
    def washing2():
        sleep(2)
        print('washer2 finished')
    
    def washing3():
        sleep(5)
        print('washer3 finished')
    
    washing1()
    washing2()
    washing3()
    
    """
    这个还是很容易理解的, 运行 demo1(), 那么需要10秒钟才能把全部衣服洗完.
    没错, 大部分时间都花在挨个地等洗衣机上了.
    """


def demo2():
    """
    现在我们想要避免无谓的等待, 为了提高效率, 我们将使用 async.
    washing1/2/3() 本是 "普通函数", 现在我们用 async 把它们升级为 "异步函数".
    
    注: 一个异步的函数, 有个更标准的称呼, 我们叫它 "协程" (coroutine).
    """
    
    async def washing1():
        sleep(3)
        print('washer1 finished')
    
    async def washing2():
        sleep(2)
        print('washer2 finished')
    
    async def washing3():
        sleep(5)
        print('washer3 finished')
    
    washing1()
    washing2()
    washing3()
    
    """
    从正常人的理解来看, 我们现在有了异步函数, 但是却忘了定义应该什么时候 "离开" 一台洗衣
    机, 去看看另一个... 这就会导致, 现在的情况是我们一边看着第一台洗衣机, 一边着急地想着
    "是不是该去开第二台洗衣机了呢?" 但又不敢去 (只是打个比方), 最终还是花了10秒的时间才
    把衣服洗完.
    
    PS: 其实 demo2() 是无法运行的, Python 会直接警告你:
        RuntimeWarning: coroutine 'demo2.<locals>.washing1' was never awaited
        RuntimeWarning: coroutine 'demo2.<locals>.washing2' was never awaited
        RuntimeWarning: coroutine 'demo2.<locals>.washing3' was never awaited
    """


def demo3():
    """
    现在我们吸取了上次的教训, 告诉自己洗衣服的过程是 "可等待的" (awaitable), 在它开始洗衣服
    的时候, 我们可以去弄别的机器.
    """
    
    async def washing1():
        await sleep(3)  # 注意这里加入了 await
        print('washer1 finished')
    
    async def washing2():
        await sleep(2)
        print('washer2 finished')
    
    async def washing3():
        await sleep(5)
        print('washer3 finished')
    
    washing1()
    washing2()
    washing3()
    
    """
    尝试运行一下, 我们会发现还是会报错 (报错内容和 demo2 一样). 这里我说一下原因, 以及在
    demo4 中会给出一个最终答案:
        1. 第一个问题是, await 后面必须跟一个 awaitable 类型或者具有 __await__ 属性的
        对象. 这个 awaitable, 并不是我们认为 sleep() 是 awaitable 就可以 await 了,
        常见的 awaitable 对象应该是:
            await asyncio.sleep(3)  # asyncio 库的 sleep() 机制与 time.sleep() 不
            # 同, 前者是 "假性睡眠", 后者是会导致线程阻塞的 "真性睡眠"
            await an_async_function()  # 一个异步的函数, 也是可等待的对象
        以下是不可等待的:
            await time.sleep(3)
            x = await 'hello'  # <class 'str'> doesn't define '__await__'
            x = await 3 + 2  # <class 'int'> dosen't define '__await__'
            x = await None  # ...
            x = await a_sync_function()  # 普通的函数, 是不可等待的
            
        2. 第二个问题是, 如果我们要执行异步函数, 不能用这样的调用方法:
            washing1()
            washing2()
            washing3()
        而应该用 asyncio 库中的事件循环机制来启动 (具体见 demo4 讲解).
    """


def demo4():
    """
    这是最终我们想要的实现.
    """
    import asyncio  # 引入 asyncio 库
    
    async def washing1():
        await asyncio.sleep(3)  # 使用 asyncio.sleep(), 它返回的是一个可等待的对象
        print('washer1 finished')
    
    async def washing2():
        await asyncio.sleep(2)
        print('washer2 finished')
    
    async def washing3():
        await asyncio.sleep(5)
        print('washer3 finished')
    
    """
    事件循环机制分为以下几步骤:
        1. 创建一个事件循环
        2. 将异步函数加入事件队列
        3. 执行事件队列, 直到最晚的一个事件被处理完毕后结束
        4. 最后建议用 close() 方法关闭事件循环, 以彻底清理 loop 对象防止误用
    """
    # 1. 创建一个事件循环
    loop = asyncio.get_event_loop()
    
    # 2. 将异步函数加入事件队列
    tasks = [
        washing1(),
        washing2(),
        washing3(),
    ]
    
    # 3. 执行事件队列, 直到最晚的一个事件被处理完毕后结束
    loop.run_until_complete(asyncio.wait(tasks))
    """
    PS: 如果不满意想要 "多洗几遍", 可以多写几句:
        loop.run_until_complete(asyncio.wait(tasks))
        loop.run_until_complete(asyncio.wait(tasks))
        loop.run_until_complete(asyncio.wait(tasks))
        ...
    """
    
    # 4. 如果不再使用 loop, 建议养成良好关闭的习惯
    # (有点类似于文件读写结束时的 close() 操作)
    loop.close()
    
    """
    最终的打印效果:
        washer2 finished
        washer1 finished
        washer3 finished
        elapsed time = 5.126561641693115
        	(毕竟切换线程也要有点耗时的)
        
    说句题外话, 我看有的博主的加入事件队列是这样写的:
        tasks = [
            loop.create_task(washing1()),
            loop.create_task(washing2()),
            loop.create_task(washing3()),
        ]
        运行的效果是一样的, 暂不清楚为什么他们这样做.
    """


if __name__ == '__main__':
    # 为验证是否真的缩短了时间, 我们计个时
    start = time()
    
    # demo1()  # 需花费10秒
    # demo2()  # 会报错: RuntimeWarning: coroutine ... was never awaited
    # demo3()  # 会报错: RuntimeWarning: coroutine ... was never awaited
    demo4()  # 需花费5秒多一点点
    
    end = time()
    print('elapsed time = ' + str(end - start))


