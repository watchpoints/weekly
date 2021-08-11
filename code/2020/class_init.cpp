#include <iostream>
#include <new>

class Test
{
public:
    Test()
    {
        std::cout << "Constructor" << std::endl;
    };
    ~Test()
    {
        std::cout << "Destructor" << std::endl;
    }
private:
    char mA;
    char mB;
};

char* gMemoryCache = (char *)malloc(sizeof(Test));
//http://www.cplusplus.com/reference/new/operator%20new/
//placement (3)	 void* operator new (std::size_t size, void* ptr) throw();
//Simply returns ptr (no storage is allocated).
int main()
{
    {
        Test* test = new(gMemoryCache) Test();
    }
    {
        Test* test = new(gMemoryCache) Test();
        test->~Test();
    }
}




// operator new example
#include <iostream>     // std::cout
#include <new>          // ::operator new

struct MyClass {
  int data[100];
  MyClass() {std::cout << "constructed [" << this << "]\n";}
};

int main () {

  std::cout << "1: ";
  MyClass * p1 = new MyClass;
  // 1 allocates memory by calling: operator new (sizeof(MyClass)) operator new
  // 2 and then constructs an object at the newly allocated space  new (p2) MyClass

  std::cout << "2: ";
  MyClass * p2 = new (std::nothrow) MyClass;
      // allocates memory by calling: operator new (sizeof(MyClass),std::nothrow)
      // and then constructs an object at the newly allocated space

  std::cout << "3: ";
  new (p2) MyClass;
      // does not allocate memory -- calls: operator new (sizeof(MyClass),p2)
      // but constructs an object at p2

  // Notice though that calling this function directly does not construct an object:
  std::cout << "4: ";
  MyClass * p3 = (MyClass*) ::operator new (sizeof(MyClass));
      // allocates memory by calling: operator new (sizeof(MyClass))
      // but does not call MyClass's constructor

  delete p1;
  delete p2;
  delete p3;

  return 0;
}









/*C++ 网易面试题“让new操作符不分配内存，只调用构造函数”
lacement new的含义
placement new可以实现不分配内存，只调用构造函数。


void *operator new( size_t, void *p ) throw()     { return p; }

作者：周肃
链接：https://www.jianshu.com/p/b52a5df69c88
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。*/