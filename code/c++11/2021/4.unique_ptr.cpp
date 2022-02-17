#include<iostream>
#include<memory>
using namespace std;
//https://mp.weixin.qq.com/s?__biz=MzI2OTA3NTk3Ng==&mid=2649285268&idx=1&sn=1832bbcf0f1c6a363efb31695d88da63&chksm=f2f991f3c58e18e5b002f48a99eb6b3d395507f889534661cd5a20478ddaeeb93690fe5bad5a&scene=21#wechat_redirect
//不太合适 有core的风险
void test(unique_ptr<int> p)
{
    *p = 10;
}
//引入麻烦 不太合适
void test2(std::unique_ptr<int> &p)
{
    *p = 10;
}

std::unique_ptr<int> test1(int i)
{
    return std::unique_ptr<int>(new int(i));
}
class NoCopy
{
public:
  std::unique_ptr<int> m_data;
};

//error: declared here unique_ptr(const unique_ptr&) = delete;
//场景：禁止一个类进行拷贝
void test1()
{
	NoCopy data;
	data.m_data= test1(10);
	//NoCopy data1 =data;
   
	
}
void test2()
{
	unique_ptr<int> up(new int(42));
    //test(up);//试图传入unique_ptr，编译报错
	//unique_ptr(const unique_ptr&) = delete;
	//test(up.get());//传入裸指针作为参数
	std::cout<<*up<<std::endl;
	test(move(up));
    //std::cout<<*up<<std::endl; 有core的风险
	
	std::unique_ptr<int> up1 = test1(10);
    std::shared_ptr<int> up2 = test1(10);
    std::cout<<*up1<<std::endl;
	std::cout<<*up2<<std::endl;
}
//g++ -std=c++11 2.cpp

void test3()
{
	std::shared_ptr<int> sp1(new int(10));
	std::shared_ptr<int> sp2(sp1);

	std::weak_ptr<int> wp(sp2);
	//输出和 wp 同指向的 shared_ptr 类型指针的数量
	cout << wp.use_count() << endl;
	//释放 sp2
	sp2.reset();
	cout << wp.use_count() << endl;
	//借助 lock() 函数，返回一个和 wp 同指向的 shared_ptr 类型指针，获取其存储的数据
	cout << *(wp.lock()) << endl;
}
int main()
{
	test3();
	test1();
	return 0;
}