#include <iostream>
#include <string>
using namespace std;

class B {
public:
  virtual ~B() {}
};

class DynamicBase {
public:
  DynamicBase()
  {
    cout<< "DynamicBase()"<<endl;
  }
};

 class D : public  B {};
//class D : public virtual B {};
class DynamicTest
{

};
class X {};
struct D1 : B {};
struct D2 : B {};

void test_errinput() {
  B *p = new D1;
  D2 *p1 = static_cast<D2 *>(p); // undefined behavior, no D2 object here
}



void test_static_cast_not()
{
  B a;
  D *ptr1 = static_cast<D *>(&a); //  error down
  //error: cannot convert from base ‘B’ to derived type ‘D’ via virtual base ‘B’
  //https://zh.cppreference.com/w/cpp/language/static_cast
  // D *ptr2 = dynamic_cast<D *>(&a);
}

/**
 * cond:
 * 1 class pointer or ref . int* is wrong  
 * 2. base class must polymorphic (张口说继承时候理解错了，没有继承关系编译的时候也检查不出来，必须是base 是多态就可以了)
 * 
 */ 
void test_dynamic_cast_not()
{   
    B a;
    DynamicTest *ptr2 = dynamic_cast<DynamicTest *>(&a); //ok why ,非继承关系 没有检查错误的
    
    double dd =10;
    //int *ptr2 = dynamic_cast<int*>(&dd);
     //dynamic_cast 中的类型必须是指向完整类类型或 void * 的指针或引用C/C++(695)

    DynamicBase db;
    //DynamicTest *ptr2 = dynamic_cast<DynamicTest *>(&db);
    //https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast?lq=1

}


int main() {
  D *d = new D;
  B *b = static_cast<B *>(d);   // up this works 


                                //编译阶段确定错误
  // X *x = static_cast<X *>(d);   // ERROR - Won't compile
  // X *x1 = dynamic_cast<X *>(d); // runtime error


 test_static_cast_not();
 test_dynamic_cast_not();

  return 0;
}