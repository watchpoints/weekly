#include "concrete_subject.h"
#include "concrete_observer.h"

int main()
{
    // 创建主题、观察者
    ConcreteSubject *pSubject = new ConcreteSubject();
    IObserver *pObserver1 = new ConcreteObserver("Jack Ma");
    IObserver *pObserver2 = new ConcreteObserver("Pony");

    // 注册观察者
    pSubject->Attach(pObserver1);
    pSubject->Attach(pObserver2);

    // 更改价格，并通知观察者
    pSubject->SetPrice(12.5);
    pSubject->Notify();
	
    cout<< "     " <<endl;
    
	// 注销观察者
    pSubject->Detach(pObserver2);
    // 再次更改状态，并通知观察者
    pSubject->SetPrice(15.0);
    pSubject->Notify();
	
    delete pSubject;
	delete pObserver1;
    delete pObserver2;
	
    return 0;
}