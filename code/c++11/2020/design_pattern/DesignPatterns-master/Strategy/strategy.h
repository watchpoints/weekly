/****************************************
* @file    IHandler
* @brief   ����ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#ifndef STRATEGY_H
#define STRATEGY_H

// ���в���
class ITransport
{
public:
	virtual ~ITransport() {}
	virtual void Run() = 0;
};

#endif // STRATEGY_H