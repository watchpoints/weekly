#include <iostream>
#include <future>
#include <thread>

void test1()
{
	std::promise<int> p;
    std::future<int> f = p.get_future();
 
    std::thread t([&p]{
        try {
            // code that may throw
            throw std::runtime_error("Example");
        } catch(...) {
            try {
                // store anything thrown in the promise
                p.set_exception(std::current_exception());
            } catch(...) {} // set_exception() may throw too
        }
    });
 
    try {
        std::cout << f.get();
    } catch(const std::exception& e) {
        std::cout << "Exception from the thread: " << e.what() << '\n';
    }
    t.join();
}
//g++ -std=c++11 01.cpp -lpthread 
int main()
{
    // future from a packaged_task
    std::packaged_task<int()> task([]{ return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread t(std::move(task)); // launch on a thread
 
    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, []{ return 8; });
 
    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{ p.set_value_at_thread_exit(9); }).detach();
 
    std::cout << "Waiting..." << std::flush;
	
	
    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
    t.join();
	
	test1();
}

/***
future 封装的是一种访问异步操作的机制
std::future provides a mechanism to access the result of asynchronous operations。
https://en.cppreference.com/w/cpp/thread/promise

promise 本质是一个类似我们打印输出中占位符的东西，你可以理解它就是一个等待数据装填的坑，它是一个“承诺”，承诺未来会有相应的数据（模板实现）

packaged_task 是对一个任务的抽象，
我们可以给其传递一个函数来完成其构造。
相较于 promise，它应该算是更高层次的一个抽象了
****/