/****************************************
* @file    IHandler
* @brief   ְ����ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef CONCRETE_HANDLER_H
#define CONCRETE_HANDLER_H

#include "handler.h"

// ����
class Manager : public IHandler
{
public:
	void HandleRequest(int days) override {
		if (days <= 1) {
			std::cout << "Manager ��׼�� " << days << " ���" << std::endl;
		} else {
			if (nullptr != m_pSuccessor)
				m_pSuccessor->HandleRequest(days);
		}
	}
};

// �ܼ�
class Director : public IHandler
{
public:
	void HandleRequest(int days) override {
		if (days <= 3) {
			std::cout << "Director ��׼�� " << days << " ���" << std::endl;
		} else {
			if (nullptr != m_pSuccessor)
				m_pSuccessor->HandleRequest(days);
		}
	}
};

// �ܲ�
class CEO : public IHandler
{
public:
	void HandleRequest(int days) override {
		if (days <= 7) {
			std::cout << "CEO ��׼�� " << days << " ���" << std::endl;
		} else {
			std::cout << "����ų��٣��Ժ������ϰ�����" << std::endl;
		}
	}
};

#endif // CONCRETE_HANDLER_H