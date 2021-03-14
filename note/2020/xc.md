## xc




Q:快照创建会占用内存空间吗？
A:不会，快照的创建成本很低，速度很快，只是在这个容器组的一个引用计数上面加1

Q:快照支持多线程同时访问不加锁吗?
A:支持

Q:快照可以长期不释放吗？

A:快照用完必须立即释放（这个很重要），否则会影响这个容器组的数据更新，快照类试与你在数据库中select * from table for update一张表，如果你不释放，那么数据装载进程更新时这个容器组数据回一直等待。
	正确的快照使用方法
	
	try
		{
			//创建快照（最好使用栈的方式，不要用new与delete）
			xc::CSnapshot cSnap(test::g2::CContainer1::GetGroupFullName());
			
			//查询容器组中的具体那一个容器
			xc::CQueryHolder<test::g2::CContainer1::Type> cQueryHolder(cSnap, test::g2::CContainer1::GetContainerName());
			test::g2::CContainer1::Type::iterator itr = 
			cQueryHolder.GetContainer().find(test::g2::CContainer1::FIELD_NO3, "1");
			//处理业务代码
			for(;!itr.eof(); itr++)
			{
				itr.value().Print();
			}
			//退出这个函数时，快照会自动释放
		}
		catch(XC_EXCEPTION & xce)
		{
			printf("%s\n", xce.get_message().c_str());
		}


总结：
1.	快照需要用时创建，用完马上释放
2.	在快照期内避免很费时的代码处理（超过1分种的逻辑处理），可以考虑把这类代码放到快照期之外处理
3.	创建快照（最好使用栈的方式，不要用new与delete），如果快照是new出来的，必须有地方delete





class CSnapshot

- 回顾 锁类型 
https://www.jianshu.com/p/390d6c49346c
自旋锁：不发送线程切换 
互斥锁：阻塞睡眠

https://www.zhihu.com/question/47411729

信号量：非阻塞操作

~~~
int CSemaphoreSimple::tryacquire(int n, int flags) const
{
    SHMMGR_LOG_APPEND(LOG_LEVEL_INFO, "%s", "[SHM_MGR] CSemaphoreSimple::tryacquire");

    return this->op(-1, n, flags | IPC_NOWAIT);
}

int CSemaphoreSimple::tryacquire_read(int n, int flags) const
{
    SHMMGR_LOG_APPEND(LOG_LEVEL_INFO, "%s", "[SHM_MGR] CSemaphoreSimple::tryacquire_read");

    return this->tryacquire(n, flags);
}

int CSemaphoreSimple::tryacquire_write(int n, int flags) const
{
    SHMMGR_LOG_APPEND(LOG_LEVEL_INFO, "%s", "[SHM_MGR] CSemaphoreSimple::tryacquire_write");

    return this->tryacquire(n, flags);
}


  void CTransaction::Commit()
    {
        if (IsOpen())
        {
            SetAndShowState(T_COMMIT_0);
            //��Ч�����
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



- 采用非阻塞方式更新信号量 等价于自旋锁

- shmat() 








XC应该具备这样的功能集合：
1、		提供事务控制
a)	对新增，更新容器组支持事务
b)	同一事务支持多个容器同时提交或则回退
c)	对同一容器做多版本控制，保证更新时不会影响原进程的读取（更新数据不需要重启进程）
d)	支持事务的异常恢复(更新进程更新过程中出现异常，下次更新保证这些异常可以被处理)
2、		提供读取权限控制
a)	对于业务进程，支持对容器的防问设置只读取权限，保证容器的安全，避免因为业务代码越界等原因，修改了容器的内存地址空间，造成整个容器不可用。
b)	对于装载进程，支持对容器的防问设置读写权限

3.	XC加载类是线程不安全的,快照类是线程安全的(多线程同时操作不用加锁处理


4. 如何做到加载一次，处处使用。需要持久化到文件中去。工具类

xc::Startup(iSn, iPsMn, iCn, iPn, strKeyFilePath.c_str(), strSqliteDumpPath.c_str());










http://95.169.24.121:4000/debug/pprof/







