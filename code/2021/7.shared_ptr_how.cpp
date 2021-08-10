//来源：公众号编程珠玑
#include <iostream>
#include <memory>
//shared_ptr循环引用而产生内存泄漏
//
void func0(std::shared_ptr<int> sp)
{
    std::cout << "fun0:" << sp.use_count() << std::endl;
}
void func1(std::shared_ptr<int> &sp)
{
    std::cout << "fun1:" << sp.use_count() << std::endl;
}

//拷贝 
//函数调用：相同的参数 建立公共的类
int main()
{
    auto sp = std::make_shared<int>(1024); //全局遍历

    func0(sp); //拷贝一次
    func1(sp); //拷贝二次
               //被多次拷贝赋值。
    return 0;
}