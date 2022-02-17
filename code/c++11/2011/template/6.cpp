#include <iostream>

using namespace std;

class Widget{};

template<typename T>
void func(T&& param){}

//Widget工厂函数
Widget widgetFactory() 
{
    return Widget();
}

//类型别名
template<typename T>
class Foo
{
public:
    typedef T&& RvalueRefToT;
};

int main()
{
    int x = 0;
    int& rx = x;
    //auto& & r = x; //error，声明“引用的引用”是非法的！

    //1. 引用折叠发生的语境1——模板实例化
    Widget w1;
    func(w1); //w1为左值，T被推导为Widget&。代入得void func(Widget& && param);
              //引用折叠后得void func(Widget& param)

    func(widgetFactory()); //传入右值，T被推导为Widget，代入得void func(Widget&& param)
                           //注意这里没有发生引用的折叠。

    //2. 引用折叠发生的语境2——auto类型推导
    auto&& w2 = w1; //w1为左值auto被推导为Widget&，代入得Widget& && w2，折叠后为Widget& w2
    auto&& w3 = widgetFactory(); //函数返回Widget，为右值，auto被推导为Widget，代入得Widget w3

    //3. 引用折叠发生的语境3——tyedef和using
    Foo<int&> f1;  //T被推导为int&，代入得typedef int& && RvalueRefToT;折叠后为typedef int& RvalueRefToT

    //4. 引用折叠发生的语境3——decltype
    decltype(x)&& var1 = 10;  //由于x为int类型，代入得int&& rx。
    decltype(rx) && var2 = x; //由于rx为int&类型，代入得int& && var2，折叠后得int& var2

    return 0;
}