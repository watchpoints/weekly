/****************************************
* @file    IHandler
* @brief   �Ž�ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#include "refined_abstraction.h"
#include "concrete_implementor.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete p; p=nullptr;} }
#endif

int main()
{
	// �������� - ��ơ�����
	IEquipment *light = new Light();
	IEquipment *fan = new Fan();

	/**
	* �������� - ����ʽ���ء���λ����
	* ������ʽ���غ͵�ƹ�������������λ���غͷ��ȹ�������
	**/
	ISwitch *pullChain = new PullChainSwitch(light);
	ISwitch *twoPosition = new TwoPositionSwitch(fan);

	// ���ơ��ص�
	pullChain->On();
	pullChain->Off();

	// �򿪷��ȡ��رշ���
	twoPosition->On();
	twoPosition->Off();

	SAFE_DELETE(twoPosition);
	SAFE_DELETE(pullChain);
	SAFE_DELETE(fan);
	SAFE_DELETE(light);

	getchar();

	return 0;
}