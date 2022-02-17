/****************************************
* @file    DateTime
* @brief   ����¼ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef MEMENTO_H
#define MEMENTO_H

#include <iostream>
#include <string>

// ����ʱ��
class DateTime
{
public:
	DateTime(std::string dt) : m_dateTime(dt) {}
	~DateTime() {}

	void SetDateTime(std::string dt) {
		m_dateTime = dt;
	}

	std::string GetDateTime() {
		return m_dateTime;
	}

private:
	std::string m_dateTime;
};

#endif // MEMENTO_H
