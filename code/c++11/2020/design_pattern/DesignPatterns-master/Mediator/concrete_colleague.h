#ifndef CONCRETE_COLLEAGUE_H
#define CONCRETE_COLLEAGUE_H

#include "colleague.h"
#include <iostream>

// ����ͬ����
class ConcreteColleague : public IColleague
{
public:
	using IColleague::IColleague;

	// ͨ���н��ߣ����Լ�����Ϣ������ȥ
	virtual void sendMessage(const IMediator& mediator, const std::string& message) const override {
		mediator.distributeMessage(this, message);
	}

private:
	// �������Է����ߵ���Ϣ
	virtual void receiveMessage(const IColleague* sender, const std::string& message) const override {
		std::cout << getName() << " received the message from "
			<< sender->getName() << ": " << message << std::endl;
	}
};

#endif // CONCRETE_COLLEAGUE_H