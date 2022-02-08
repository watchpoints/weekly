#include <iostream>
#include <cassert>
#include <memory>

using namespace std;

//转移 unique_ptr 的所有权
void test1(){

    unique_ptr<int> pInt = make_unique<int>(5);

    unique_ptr<int> pInt2 = std::move(pInt);    // 转移所有权
    //cout << *pInt << endl; // 出错，pInt为空
    cout << *pInt2 << endl;
    unique_ptr<int> pInt3(std::move(pInt2));
}
int main()
{
    return 0;
}