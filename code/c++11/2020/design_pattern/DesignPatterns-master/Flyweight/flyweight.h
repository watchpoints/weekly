#ifndef FLYWEIGHT_H
#define FLYWEIGHT_H

#include <iostream>
#include <string>

// ��� - ��������ʹ��
class IPlayer
{
public:
	virtual ~IPlayer() {}

	// ��������
	virtual void assignWeapon(std::string weapon) = 0;

	// ʹ��
	virtual void mission() = 0;

protected:
	std::string m_task; // �ڲ�״̬
	std::string m_weapon; // �ⲿ״̬
};

#endif // FLYWEIGHT_H