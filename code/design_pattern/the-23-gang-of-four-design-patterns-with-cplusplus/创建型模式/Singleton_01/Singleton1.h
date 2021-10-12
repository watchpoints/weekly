#include <iostream>
using namespace std;

template <class T>
class Singleton
{
public:
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

  static T &get_instance()
  {
    if (!instance)
      instance = new T;
    return *instance;
  }

protected:
  Singleton() = default;
  static inline T *instance = nullptr;
};