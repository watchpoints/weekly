#include <iostream>
#include <functional>
using namespace std;

typedef std::function<void()> FP;
void run_fun_ptr(FP fp);
FP get_fun_ptr();
FP get_fun_ptr_ref();

int main()
{
    run_fun_ptr(get_fun_ptr());
    run_fun_ptr(get_fun_ptr_ref());
    return 0;
}

void run_fun_ptr(FP fp)
{
    if(fp)
    {
        fp();
    }
}

FP get_fun_ptr()
{
    int a = 2;
    return [=](){cout << "= a:"<<a << endl;};
}

FP get_fun_ptr_ref()
{
    int a = 2;
    return [&](){cout << "& a:"<<a << endl;};
}

//陷阱1：默认引用捕获可能带来的悬挂引用问题
//陷阱2：捕获this陷阱


#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
typedef std::function<void()> FP;

class Point
{
public:
    Point(int x,int y):m_x(x),m_y(y)
    {

    }
    void print()
    {
        s_print_history.push_back([=](){std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;});
        std::cout << "(X:" << m_x << ",Y:" << m_y <<")" << std::endl;
    }

    static void print_history()
    {
        std::for_each(s_print_history.begin(),s_print_history.end(),[](FP p){
            if(p)
                p();
        });
    }

private:
    int m_x;
    int m_y;
    typedef std::function<void(void)> FP;
    static std::vector<FP> s_print_history;
};
std::vector<FP> Point::s_print_history = std::vector<FP>();

int main()
{
    std::unique_ptr<Point> p;
    p.reset(new Point(1,1));p->print();
    p.reset(new Point(2,2));p->print();
    p.reset(new Point(3,3));p->print();
    
    Point::print_history();
    return 0;
}
//https://blog.csdn.net/czyt1988/article/details/80149695