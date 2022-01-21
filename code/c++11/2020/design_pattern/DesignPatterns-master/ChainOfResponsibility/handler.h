/****************************************
* @file    IHandler
* @brief   ְ����ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>

// ��������
class IHandler
{
public:
	virtual ~IHandler() {}
	void SetSuccessor(IHandler *successor) { 
		m_pSuccessor = successor;
	}
	virtual void HandleRequest(int days) = 0;

protected:
	IHandler *m_pSuccessor = nullptr;  // �����
};

#endif // HANDLER_H