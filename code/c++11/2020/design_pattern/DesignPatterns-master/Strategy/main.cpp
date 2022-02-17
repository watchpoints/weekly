/****************************************
* @file    IHandler
* @brief   ����ģʽ
* @author  ΢�źţ�iwaleon
* @wechat  ΢�Ź��ںţ���Ч����Ա
* @date    2017/10/01
****************************************/

#include "context.h"
#include "concrete_strategy.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete p; p=nullptr;} }
#endif

int main()
{
	ITransport *bike = new Bike();
	ITransport *car = new Car();
	ITransport *train = new Train();

	// ���������г������Ŀ�������������
	Tourists *zhangsan = new Tourists(bike);
	Tourists *lisi = new Tourists(car);
	Tourists *wangwu = new Tourists(train);

	zhangsan->Travel();
	lisi->Travel();
	wangwu->Travel();

	SAFE_DELETE(bike);
	SAFE_DELETE(car);
	SAFE_DELETE(train);

	SAFE_DELETE(zhangsan);
	SAFE_DELETE(lisi);
	SAFE_DELETE(wangwu);

	getchar();

	return 0;
}