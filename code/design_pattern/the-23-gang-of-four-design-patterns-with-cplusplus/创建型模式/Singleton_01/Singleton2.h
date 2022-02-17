#include <iostream>
using namespace std;

template <class T>
class Singleton
{
protected: // 可以被继承,Implementing Singleton with CRTP
  Singleton() {}

public:
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

  static T &get_instance()
  {
    if (!instance)
      instance = new T_Instance; // new T()
    return *instance;
  }



private:
  struct T_Instance : public T
  {
    T_Instance() : T() {}
  };
  
  static  T *instance ;
 
 private:
   //线程安全
   class GarbageCreate //内部类-创建数据
   {
   public:
     GarbageCreate() //构造函数
     {
       cout << " safe create before main()" << endl;
       Singleton<T>::get_instance(); //在main函数之前执行，不存在线程竞争问题
     }
   };

   static GarbageCreate m_mainBefore; // 创建
   
};

template <class T>
T *Singleton<T>::instance = nullptr;

template <class T>
typename  Singleton<T>::GarbageCreate Singleton<T>::m_mainBefore; //线程安全

/**
 * 
 * 
 *  error: need ‘typename’ before ‘Singleton<T>::GarbageCreate’ because ‘Singleton<T>’ is a dependent scope


 * //https://stackoverflow.com/questions/51974670/implementing-singleton-with-crtp
class A : public Singleton<A>
{
protected:
  A() {}
};

int main()
{
  auto &x = A::get_instance();
}**/