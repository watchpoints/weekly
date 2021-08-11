#include <iostream>
using namespace std;
 
 
class CB
{
public:
    //假设这个完全正确，后面也是用不了。
    virtual  void f(int)
    {
        cout << "CB::f(int)" << endl;
    }
};
 
 
class CD : public CB
{
public:
    void f(int,int)
    {
        cout << "CD::f(int,int)" << endl;
    }
    
     virtual  void f(int)
    {
        cout << "CD::f(int)" << endl;
    }
    void test()
    {
        //f(1); // error: too few arguments to function call, expected 2, have 1; did you mean 'CB::f'?
    }
};
 
 
int main(int argc, char* argv[])
{   
    CD d;
    //d.f(1); //隐藏了
    CB b=d;// 多态 跟普通对象没有关系，为什么？ 多态 关键是什么？
    b.f(1);  //不是多态
    
    CB* pb =&d;//多态 跟派生类 分配在 堆上和栈上 没有关系，为什么？ 多态 关键是什么？
    pb->f(1);
    
    CB* pb2 =&b;//多态 跟指针也没 没有关系，为什么？ 多态 关键是什么？
    pb2->f(1);

    return 0;
}