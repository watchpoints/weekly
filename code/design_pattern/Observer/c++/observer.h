/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the Subject is also called the
 * Publisher and the Observer is often called the Subscriber and vice versa.
 * Also the verbs "observe", "listen" or "track" usually mean the same thing.
 */
#ifndef OBSERVER_H
#define OBSERVER_H

#include "subject.h"
#include <iostream>
#include <list>

using namespace std;

// 抽象观察者
class IObserver
{
public:
    virtual void Update(float price) = 0;  // 更新价格
	virtual ~IObserver(){};
};

#endif // OBSERVER_H