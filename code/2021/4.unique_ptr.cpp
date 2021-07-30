#include<iostream>
#include<memory>
void test(std::unique_ptr<int> p)
{
    *p = 10;
}
int main()
{
    std::unique_ptr<int> up(new int(42));
    test(up);//试图传入unique_ptr，编译报错
    std::cout<<*up<<std::endl;
    return 0;
}