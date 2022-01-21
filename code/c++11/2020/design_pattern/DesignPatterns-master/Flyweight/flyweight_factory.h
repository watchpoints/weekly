#ifndef FLYWEIGHT_FACTORY_H
#define FLYWEIGHT_FACTORY_H

#include "concrete_flyweight.h"
#include <map>

// ���ڻ�ȡ���
class PlayerFactory
{
public:
	// ��� T/CT ������ڣ���ֱ�Ӵ���Ԫ�ػ�ȡ�����򣬴���һ���¶�����ӵ���Ԫ���У�Ȼ�󷵻ء�
	static IPlayer* getPlayer(std::string type)
	{
		IPlayer *p = NULL;
		if (m_map.find(type) != m_map.end()) {
			p = m_map[type];
		}
		else {
			// ���� T/CT ����
			if (type == "T") {
				std::cout << "Terrorist Created" << std::endl;
				p = new Terrorist();
			}
			else if (type == "CT") {
				std::cout << "Counter Terrorist Created" << std::endl;
				p = new CounterTerrorist();
			}
			else {
				std::cout << "Unreachable code!" << std::endl;
			}
			// һ��������������뵽 map ��
			m_map.insert(std::make_pair(type, p));
		}
		return p;
	}

private:
	// �洢 T/CT ������Ԫ�أ�
	static std::map<std::string, IPlayer*> m_map;
};

#endif // FLYWEIGHT_FACTORY_H