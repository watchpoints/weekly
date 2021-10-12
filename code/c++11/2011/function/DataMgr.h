#include "TcpMgr.h"

class DataMgr 
{
public:
	void Init()
	{
		m_pTcpMgr = std::make_shared<TcpMgr>();
        m_pTcpMgr->SetDataHandleCb(std::bind(&DataMgr::HandleData, this, std::placeholders::_1)); // 在初始化中为 TcpMgr 设置回调函数
    }
	void HandleData(const string &data)
	{
		// ...
	}
	
private:
	boost::shared_ptr<TcpMgr> m_pTcpMgr;
};























//原文链接：https://blog.csdn.net/guotianqing/article/details/103511900

/**
 
_1: std::placeholders
https://blog.csdn.net/dxgzg/article/details/109957805
https://www.cnblogs.com/lzya/p/4912993.html


this:why 类地址


 **/