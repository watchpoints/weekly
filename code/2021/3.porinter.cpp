#include <iostream>
#include <vector>
#include <memory>

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
    for (std::vector<std::unique_ptr<SimpleTest>>::iterator iter = vc.begin();
         iter != vc.end(); iter++)
    {
        (*iter)->DoSomething();
    }
}
int main()
{
    TestAutoPtr5();
}