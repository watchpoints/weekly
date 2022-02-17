/****************************************
* @file    IHandler
* @brief   �Ž�ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include "implementor.h"

// ����
class ISwitch
{
public:
	ISwitch(IEquipment *equipment) { m_pEquipment = equipment; }
	virtual ~ISwitch() {}

	// �򿪵���
	virtual void On() = 0;

	// �رյ���
	virtual void Off() = 0;

protected:
	IEquipment *m_pEquipment = nullptr;
};

#endif // ABSTRACTION_H