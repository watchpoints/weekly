#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>

using namespace std;

class Component
{
public:
	Component(string name) : m_strName(name) {}
	virtual ~Component() {}
	virtual void Add(Component *cmpt) = 0;  // ��ӹ���
	virtual void Remove(Component *cmpt) = 0;  // ɾ������
	virtual Component* GetChild(int index) = 0;  // ��ȡ����
	virtual void Operation(int indent) = 0;  // ��ʾ�������������ķ�ʽ��

private:
	Component();  // ������

protected:
	string m_strName;
};

#endif // COMPONENT_H