#ifndef SUBJECT_H
#define SUBJECT_H

// ������Ӫ��
class ITelco
{
public:
	virtual ~ITelco() {}
	virtual void Recharge(int money) = 0;  // ��ֵ
};

#endif // SUBJECT_H