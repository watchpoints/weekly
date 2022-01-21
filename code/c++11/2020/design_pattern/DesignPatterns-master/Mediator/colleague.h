#pragma once

#include "mediator.h"
#include <string>

class IMediator;

// ����ͬ����
class IColleague
{
public:
	IColleague(const std::string& name) : m_strName(name) {}
	std::string getName() const { return m_strName; }

	// ͨ���н��ߣ����Լ�����Ϣ������ȥ
	virtual void sendMessage(const IMediator& mediator, const std::string& message) const = 0;
	// �������Է����ߵ���Ϣ
	virtual void receiveMessage(const IColleague* sender, const std::string& message) const = 0;

private:
	std::string m_strName;
};