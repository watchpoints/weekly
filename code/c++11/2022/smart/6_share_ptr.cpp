#include <cassert>
#include <iostream>
#include <memory>
using namespace std;
/**
 *  TODO
 * Q1. 一个base 类不声明为虚析构函数，share_ptr能正确释放吗？
 * A1: https://zhuanlan.zhihu.com/p/75553222

 * Q2 share_pt根本不知道创建内容t 是否继承，enable_shared_from_this 真没设置Weakptr?
   A2:_Set_ptr_rep_and_enable_shared
      https://0cch.com/2020/08/05/something-about-enable_shared_from_this/

      _Can_enable_shared，[萃取]

      如果目标类型有内嵌类型_Esft_type，
      并且目标类型和内嵌类型的指针是可转换的，
      也就是有继承关系，
      那么类型结果为true_type，反之为false_type。

 *
 */
class B
{
public:
    ~B()
    {
        std::cout << "~B\n";
    }
};

class D : public B
{
public:
    ~D()
    {
        std::cout << "~D\n";
    }
};

void test_unique()
{
    std::cout << "---------test_unique free base class --------------\n";
    B *raw_pointer = new D();
    delete raw_pointer; //~B

    std::unique_ptr<B> unique_pointer = std::unique_ptr<B>(new D()); // right value
    unique_pointer = nullptr;                                        //~B

    std::unique_ptr<B> unique_pointer_use_make_unique = std::make_unique<D>(); // right value
    unique_pointer_use_make_unique = nullptr;                                  //~B
}

void test_share_ptr()
{
    std::cout << "---------test_share free base calss --------------\n";
    std::shared_ptr<B> shared_pointer = std::shared_ptr<B>(new D());
    shared_pointer = nullptr;

    std::shared_ptr<B> shared_pointer_use_make_shared = std::make_shared<D>();
    shared_pointer_use_make_shared = nullptr;
}
int main()
{

    test_share_ptr();
    test_unique();

    return 0;
}
