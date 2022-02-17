/****************************************
* @file    IHandler
* @brief   ����ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef CONTEXT_H
#define CONTEXT_H

#include "strategy.h"

// ������
class Tourists
{
public:
	Tourists(ITransport *transport) { 
		m_pTransport = transport;
	}

	void Travel() {
		if (nullptr != m_pTransport)
			m_pTransport->Run(); 
	}

private:
	ITransport *m_pTransport = nullptr;
};

#endif // CONTEXT_H