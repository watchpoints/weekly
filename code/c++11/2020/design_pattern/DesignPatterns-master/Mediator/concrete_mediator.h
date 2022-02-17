#ifndef CONCRETE_MEDIATOR_H
#define CONCRETE_MEDIATOR_H

#include "mediator.h"

// �����н���
class ConcreteMediator : public IMediator
{
public:
	// �������ߵ���Ϣ���͸����в����ߣ����������������Լ���
	virtual void distributeMessage(const IColleague* sender, const std::string& message) const override {
		for (const IColleague* c : getColleagues())
			if (c != sender)  // ��Ҫ����Ϣ���͸��Լ�
				c->receiveMessage(sender, message);
	}
};

#endif // CONCRETE_MEDIATOR_H