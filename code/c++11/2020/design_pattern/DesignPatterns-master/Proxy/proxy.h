#ifndef PROXY_H
#define PROXY_H

#include "subject.h"
#include "real_subject.h"
#include <iostream>

// �����
class Proxy : public ITelco
{
public:
	Proxy() : m_pCMCC(NULL) {}
	~Proxy() { delete m_pCMCC; }

	// ���� 50 ����
	void Recharge(int money) override {
		if (money >= 50) {
			if (m_pCMCC == NULL)
				m_pCMCC = new CMCC();
			m_pCMCC->Recharge(money);
		}
		else {
			std::cout << "Sorry, too little money" << std::endl;
		}
	}

private:
	CMCC *m_pCMCC;
};

#endif // PROXY_H