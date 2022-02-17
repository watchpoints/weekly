#include <iostream>
#include <memory>

int main () {

  std::unique_ptr<int> foo ; // lvalue
  std::unique_ptr<int> bar; //  lvalue
  
  //为什么这个成立呢
  foo = std::unique_ptr<int>(new int (101));  // rvalue
  
  //bar = foo; 
  // unique_ptr& operator=(const unique_ptr&) = delete;
  //universal references
  bar = std::move(foo); //  lvalue
  bar = std::move(std::unique_ptr<int>(new int (11))); //rvalue
  std::unique_ptr<int> up3 = std::make_unique<int>(3);
  //

  

  std::cout << "foo: ";
  if (foo) std::cout << *foo << '\n'; else std::cout << "empty\n";

  std::cout << "bar: ";
  if (bar) std::cout << *bar << '\n'; else std::cout << "empty\n";


  std::unique_ptr<int> foo1 =std::unique_ptr<int>(new int (1));
  std::unique_ptr<int> bar2 =std::unique_ptr<int>(new int (2));

  //相互赋值：和普通变量相互赋值一样。

  //提问1. unique_ptr能不能相互赋值，
  //提问2. unique_ptr 如何解决？
  //提问3。 unique_ptr 有什么后果

  //唯一指针相互赋值，
  //小白：不思考题目直接回答：你错误理解：相互引用了。
  //初级：你刷一个算法题目， 翻转tree 最后普通变量一样，不是swap。最后变成一个值了。
  //结果是什么：你不问为什么相互赋值，swap。不是这样效果。
  //unique_ptr ptr --int  a=b b=a

  foo1 = std::move(bar2);
  std::cout << "foo1: ";
  if (foo1) std::cout << *foo1 << '\n'; else std::cout << "empty\n";
  bar2 = std::move(foo1);
  std::cout << "bar2: ";
  if (bar2) std::cout << *bar2 << '\n'; else std::cout << "empty\n";
   

  std::unique_ptr<int> up4 = std::make_unique<int>(3);

  return 0;
}


/**
 * @brief D
 * 
 * @tparam T 
 * @tparam Ts 
 * @param params 
 * @return std::unique_ptr<T> 
 * 
 第一感觉：虽然是标准库的函数make_unique，但是我感觉语法不正确  unique_ptr不能直接赋值的，
 你创造出来一个东西，然后 std::unique_ptr<T> =  std::unique_ptr<T>
 这是语法错误，根本没有执行move操作 
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
//没有名字是临时变量
}*/

