/****************************************
* @file    IHandler
* @brief   �Ž�ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef IMPLEMENTOR_H
#define IMPLEMENTOR_H

// ����
class IEquipment
{
public:
	virtual ~IEquipment() {}

	// ��
	virtual void PowerOn() = 0;

	// �ر�
	virtual void PowerOff() = 0;
};

#endif // IMPLEMENTOR_H