/****************************************
* @file    Life
* @brief   ����¼ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef ORIGINATOR_H
#define ORIGINATOR_H

#include "memento.h"
#include <iostream>
#include <string>

// һ��
class Life
{
public:
	Life() {}
	~Life() {}

	void SetDateTime(std::string dt) {
		std::cout << "Set date time to " << dt << std::endl;
		m_dateTime = dt;
	}

	// �����ڴ�ӡ
	std::string GetDateTime() {
		return m_dateTime;
	}

	// �ָ�����ʱ��
	void SetMemento(DateTime *dt) {
		if (nullptr != dt)
			m_dateTime = dt->GetDateTime();
	}

	// ��������ʱ��
	DateTime *CreateMemento() {
		return new DateTime(m_dateTime);
	}

private:
	std::string m_dateTime;
};

#endif // ORIGINATOR_H
