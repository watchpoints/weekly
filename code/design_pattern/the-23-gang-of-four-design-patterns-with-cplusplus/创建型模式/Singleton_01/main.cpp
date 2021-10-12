

#include "Singleton2.h"

class A : public Singleton<A>
{
protected:
  A() 
  {
    cout << "class A : public Singleton<A> A()" << endl;
  }
};

int main()
{
  auto &x = Singleton<A>::get_instance();
}