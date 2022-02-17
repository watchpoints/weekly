#include <iostream>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;

void test1()
{
	//int& i=42; //invalid initialization of non-const reference of type ‘int&’ from a temporary of type ‘int
	const int& i=42;
	
	//C++11标准添加了右值引用(rvalue reference)，这种引用只能绑定右值，不能绑定左值
	int&& right=42;
	string const & temp="aaa";
	
}
// 函数1，接受左值引用
void process_copy(const std::vector<int>& vec_) {
    // do_something
    std::vector<int> vec(vec_); //  不能修改左值，所以要拷贝vector
    vec.push_back(42);
}

// 函数2，接受右值引用
void process_copy(std::vector<int> && vec) {
    vec.push_back(42); // 直接修改右值
}


//为什么使用左值引用不能叫做移动呢？
//process_copy1不能修改该临时对象。
//右值引用传递对象，则代表调用者（有意或无意的）保证不会在之后继续使用该对象
void test2()
{
	std::vector<int> data;
    process_copy(data); // 调用函数1
	
    process_copy(std::vector<int>()); // 调用函数2，临时对象作为右值，函数内部无需拷贝，降低开销
}

class Person {
public:
    int* data;

public:
    Person() : data(new int[1000000]){ cout<< "Person() "<<endl;}
    ~Person() { delete [] data; }

    // 拷贝构造函数，需要拷贝动态资源
    Person(const Person& other) : data(new int[1000000]) {
        std::copy(other.data,other.data+1000000,data);
		cout<< "Person(const Person& other) "<<endl;
    }

    // 移动构造函数，无需拷贝动态资源
    Person(Person&& other) : data(other.data) {
        other.data=nullptr; // 源对象的指针应该置空，以免源对象析构时影响本对象
		
		cout<< "Person( Person&& other) "<<endl;
    }
};
// 这里不用引用目的，就是去构造一个对象，看看如何构造
void func(Person p){
    // do_something
} 
//将动态数据移动到新对象中后，应该解除与源对象的关系
void test3()
{
	Person p; //这样显示转换为右值之后，应保证之后不再使用该对象
    func(p);    // 调用Person的拷贝构造函数来创建实参
	
    func(Person()); // 调用Person的移动构造函数来创建实参 Person(Person&& other) : data(other.data) { 这里没有调用
}

//对非临时对象调用移动构造函数
//这样显示转换为右值之后，应保证之后不再使用该对象
void test4()
{
	Person p1;
    func(std::move(p1)); // 调用移动构造函数，应保证之后不再使用p1
	//cout <<*(p1.data)<<endl; //Segmentation fault

    Person p2;
    func(static_cast<Person&&>(p2)); // 调用移动构造函数后，应保证之后不再使用p2
	//cout <<*(p2.data)<<endl; //Segmentation fault
	//swap(null,ptr)

}

/**
class Person
{
private:
    int age;
    string name;
    int* data;

public:
    Person() : data(new int[1000000]){}
    ~Person() { delete [] data; }

    // 拷贝构造函数
    Person(const Person& p) :
    age(p.age),
    name(p.name),
    data(new int[1000000]){
        std::copy(p.data, p.data+1000000, data);
        cout << "Copy Constructor" << endl;
    }

    // 拷贝赋值运算符
    Person& operator=(const Person& p){
        this->age = p.age;
        this->name = p.name;
        this->data = new int[1000000];
        std::copy(p.data, p.data+1000000, data);
        cout << "Copy Assign" << endl;
        return *this;
    }

    // 移动构造函数
    Person(Person &&p) :
    age(std::move(p.age)),
    name(std::move(p.name)),
    data(p.data){
        p.data=nullptr; // 源对象的指针应该置空，以免源对象析构时影响本对象
        cout << "Move Constructor" << endl;
    }

    // 移动赋值运算符
    Person& operator=(Person &&p){
        this->age = std::move(p.age);
        this->name = std::move(p.name);
        this->data = p.data;
		
		
        p.data=nullptr; //原来不能使用了，一般不要对普通对象进行move操作，这样不安全。
        cout << "Move Assign" << endl;
        return *this;
    }
};**/

void test5()
{
    Person p;
    Person && pp = static_cast<Person &&>(p);
    cout<<pp.data<<endl;
    Person && pp1 = dynamic_cast<Person &&>(p);
   // Person* pp2 = dynamic_cast<Person *>(p);//语法错误 
    cout<<pp1.data<<endl;
}
//g++ -std=c++11 -g 05_class_move.cpp 
int main()
{
    //test2();
	//test3();
	//test4();
    test5();
    return 0;
}






// 01 为避免用户在派生类中重载一个虚函数

//02 https://www.oschina.net/translate/use-c-11-inheritance-control-keywords-to-prevent-inconsistencies-in-class-hierarchies?cmp


//03 使用C++11继承控制关键词来防止在类层次结构上的不一致

/**
int foo(42);
int& bar = foo;  // OK: foo 在此是左值，将它的内存空间与 bar 绑定在一起
int& baz = 42;   // Err: 42 是右值，不能将它绑定在左值引用上
const int& qux = 42;  // OK: 42 是右值，但是编译器可以为它开辟一块内存空间，绑定在 qux 上**/