#  服务治理-GRPC

>  耗时25*7

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题







### 二、这个技术的优势和劣势分别是什么 



### 三、这个技术适用的场景。









### 四、技术的组成部分和关键点。



- grpc-go+线程模型 

![](http://jiangew.me/assets/images/post/20181016/grpc-03-06.png)

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比

- 深入浅出gRPC



#  共享内存 xc 

>  耗时25*7

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题



适用场景：

- 进程是**CPU密集型**，不是IO密集型，数据缓存在本地，非远程。

- 一个机器：一个进程数据缓存过大，造成无法启动多个进程并行处理，无法利用多核。需要共享内存保存一份数据。

XC是把原有进程私有的静态的内存数据，通过统一的容器的方式，存储到共享内存中，

减少减少每个进程对内存的要求，并支持高性能访问要求。

- 容器化调动，一般进程方式。



### 二、这个技术的优势和劣势分别是什么 

- 优势：快，

- 劣势：进程间通信方式采用共享内存不符合主流设计，单机不能无限扩展进程。

  



### 三、这个技术适用的场景。



- 多读，少刷新，预先知道内存大小。

- 一次性更新多个容器（表）

  





### 四、技术的组成部分和关键点。



XC加载类是线程不安全的,快照类是线程安全的(多线程同时操作不用加锁处理)



Q:快照创建会占用内存空间吗？

A:不会，快照的创建成本很低，速度很快，只是在这个容器组的一个引用计数上面加1

 

Q:快照支持多线程同时访问不加锁吗?

A:支持



Q:快照可以长期不释放吗？

A:快照用完必须立即释放（这个很重要），否则会影响这个容器组的数据更新，快照类试与你在数据库中select * from table for update一张表，如果你不释放，那么数据装载进程更新时这个容器组数据回一直等待。

总结：

1. 快照需要用时创建，用完马上释放

2. 在快照期内避免很费时的代码处理（超过1分种的逻辑处理），可以考虑把这类代码放到快照期之外处理

3. 创建快照（最好使用栈的方式，不要用new与delete），如果快照是new出来的，必须有地方delete



高并发必须需要持久化

- 防止单机故障，重启后加装，dump 采用sqlite3存储

- l 一个事务可以包含多个容器的更新与创建

  l 系统同一时间对同一容器只允许一个更新事务

  l 系统支持更新异常时，下次自动恢复

  l 一个事务支持同时对多个容器进行更新，并一次提交或则回退

### 五、技术的底层原理和关键实现

入口：xc_console.cpp

CSnapshot

- 构造时候无锁方式更新计数。模拟原子操作不停while循环处理semop 直到成功

~~~
CSnapshot::CSnapshot(const CTransaction &cTransaction) throw(XC_EXCEPTION) : m_pContainerGroup(NULL), m_strGroupName(cTransaction.GetGroupName().c_str())
    {
        m_pContainerGroup = cTransaction.GetContainerGroup();
        CCounterMgr::GetInstance().PlusCounter(m_pContainerGroup->GetCounterIdx());
    }

    void CSnapshot::Release()
    {
        if (m_pContainerGroup != NULL)
        {
            CCounterMgr::GetInstance().MinusCounter(m_pContainerGroup->GetCounterIdx());
            m_pContainerGroup = NULL;
        }
    }
~~~



- 技术点：信号量semaphore  https://github.com/golang/sync/blob/master/semaphore/semaphore.go

  （ c语言的一般来说信号量和条件变量效果类似，前者主要进程 后者线程）

CConsole

- CConsole::TestCmd(aistring strCommand)-->xc::CTransaction.commit





感觉上更新需要加锁，但是一直看load代码没有找到位置，现在找到了

~~~c++
  enum ETransState
    {
        T_CREATE = 0,
        T_PROCESS,
        T_COMMIT_0,
        T_COMMIT_1,
        T_COMMIT_2,
        T_COMMIT_SUC,
        T_ROLLBACK_0,
        T_ROLLBACK_1,
        T_ROLLBACK_SUC,
        T_MINUX_GROUP_COUNT,
        T_DESTROY
    };

const char * CTransMgr::m_pTransStateDesc[] =
{
    "transaction create!",
    "transaction being processing![Create a container!]",
    "commit start...",
    "commit 1 stage finish![Update container effect]!",
    "commit 2 stage finish![Clean up the old container]!",
    "commit successfully!",
    "rollback start...",
    "rollback 1 stage finish![Clean up the temporary container]!",
    "rollback successfully!",
    "minus group count!",
    "transaction destroy!"
};  
void CTransaction::Commit()
    {
        if (IsOpen())
        {
            SetAndShowState(T_COMMIT_0);
            CContainerGroup *pOldGroup = CGroupMgr::GetInstance().ReplaceGroup(
                m_pTransData->m_strGroupName.c_str(),
                m_pTransData->m_pContainerGroup,
                m_pTransData->m_bAllGroupUpdate);
            SetAndShowState(T_MINUX_GROUP_COUNT);
            CCounterMgr::GetInstance().MinusCounter(m_pTransData->m_pContainerGroup->GetCounterIdx());
            SetAndShowState(T_COMMIT_1);
            FreeContainerGroup(pOldGroup, true);
            SetAndShowState(T_COMMIT_2);
            SetAndShowState(T_COMMIT_SUC);
            WriteSysInfo(FormatString("commit cost time![group_name:%s, second:%d]",
                                      m_pTransData->m_strGroupName.c_str(),
                                      time(NULL) - m_pTransData->m_tmCreateTime));
            ReleaseTransData();
        }
    }

~~~



安全

- load A 读取数据，B放入临时内存 C 提交 **在提交时候才去加锁**，java内存模型更新也是如此。
- "xc load from sqlite finish!  class CLoad4Sqlite : public CLoad

### 六、已有的实现和它之间的对比

- Mysql 的MVCC
- [RWMutex](https://juejin.im/post/5dfc4a1f6fb9a016561cadde) 实现 go1.14\src\sync\rwmutex.go
- Linux 读写锁实现 
- 智能指针
- ACID 事务原子性  WAL（Write-Ahead Logging）
- docker专栏 你6个月没看了，订阅有何用。