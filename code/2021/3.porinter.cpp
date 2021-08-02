#include <iostream>
#include <vector>
#include <memory>
//https://www.cnblogs.com/liumantang/p/12564151.html
using namespace std;

class SimpleTest 
{
public:
    SimpleTest(int test_id) : test_id_(test_id) 
    {}
    ~SimpleTest() {}
    void DoSomething()
    {
        std::cout << "DoSomething ... : " << test_id_ << std::endl;
    }
private:
    int test_id_;
};
//TestAutoPtr1不会core、TestAutoPtr2会core、TestAutoPtr3不会core、TestAutoPtr4会core
//core  vc.resize(5);
//用了unique_ptr为什么会core， 这是unique_ptrbug吗？

void TestAutoPtr5() 
{
    std::cout << "TestAutoPtr5 Test" << std::endl;
    std::vector<std::unique_ptr<SimpleTest>> vc;
    vc.push_back(std::unique_ptr<SimpleTest>(new SimpleTest(5)));
    vc.push_back(std::unique_ptr<SimpleTest>(new SimpleTest(6)));
    vc.push_back(std::unique_ptr<SimpleTest>(new SimpleTest(7)));

    // 1-----
    for (std::vector<std::unique_ptr<SimpleTest>>::iterator iter = vc.begin();
         iter != vc.end(); iter++)
    {
        (*iter)->DoSomething();
    }

    // 2-----
    vc.resize(5);
    //可看出智能指针尽量不要指向vector容器类型，因为当vector扩容时，智能指针便不再生效，引起程序的崩溃或未定义的行为。
    for (std::vector<std::unique_ptr<SimpleTest>>::iterator iter = vc.begin();
         iter != vc.end(); iter++)
    {
        (*iter)->DoSomething();
    }
}

void TestShared_ptr6()
{
    std::cout << "TestAutoPtr5 Test" << std::endl;
    std::vector<std::shared_ptr<SimpleTest>> vc;
    vc.push_back(std::shared_ptr<SimpleTest>(new SimpleTest(5)));
    vc.push_back(std::shared_ptr<SimpleTest>(new SimpleTest(6)));
    vc.push_back(std::shared_ptr<SimpleTest>(new SimpleTest(7)));

    // 1-----
    for (std::vector<std::shared_ptr<SimpleTest>>::iterator iter = vc.begin();
         iter != vc.end(); iter++)
    {
        (*iter)->DoSomething();
    }

    // 2-----
    vc.resize(5);
    //可看出智能指针尽量不要指向vector容器类型，因为当vector扩容时，智能指针便不再生效，引起程序的崩溃或未定义的行为。
    for (std::vector<std::shared_ptr<SimpleTest>>::iterator iter = vc.begin();
         iter != vc.end(); iter++)
    {
        (*iter)->DoSomething();
    }
}
/***
 *  DoSomething ... : 5
DoSomething ... : 6
DoSomething ... : 7
DoSomething ... : 5
DoSomething ... : 6
DoSomething ... : 7
Segmentation fault

 *
 **/

void TestAutoPtr3()
{
    std::cout << "TestAutoPtr3 Test" << std::endl;
    std::shared_ptr<SimpleTest> my_ptr(new SimpleTest(3));
    if (my_ptr.get()) // 判断指针是否为空
    {
        std::shared_ptr<SimpleTest> my_ptr0; // 创建一个新的std::auto_ptr<SimpleTest>对象
        std::cout << "[1]UseCount: " << my_ptr.use_count() << std::endl;
        my_ptr0 = my_ptr; // 复制旧的 my_ptr 给 my_ptr0
        my_ptr0->DoSomething();
        my_ptr->DoSomething();
        std::cout << "[2]UseCount: " << my_ptr.use_count() << std::endl;
    }
}
//Segmentation fault
void TestAutoPtr4()
{
    std::cout << "TestAutoPtr4 Test" << std::endl;
    std::unique_ptr<SimpleTest> my_ptr(new SimpleTest(4));
    if (my_ptr.get()) // 判断指针是否为空
    {
        std::unique_ptr<SimpleTest> my_ptr0; // 创建一个新的std::auto_ptr<SimpleTest>对象
        my_ptr0 = std::move(my_ptr);         // 复制旧的 my_ptr 给 my_ptr0
        my_ptr0->DoSomething();
        my_ptr->DoSomething(); //已经执行move操作：
    }
}
int main()
{
    //TestAutoPtr5(); //core
    //TestShared_ptr6(); core

}