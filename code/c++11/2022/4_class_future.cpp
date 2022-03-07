#include <iostream>
#include <future>
#include <thread>
 
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
}
//https://en.cppreference.com/w/cpp/thread/future
//https://www.cnblogs.com/haippy/p/3280643.html

//https://www.cnblogs.com/haippy/p/3239248.html

//https://www.cnblogs.com/haippy/p/3239248.html


#include <thread>
#include <future>
#include <iostream>
#include <chrono>

int main(){
    auto future = std::async(std::launch::async, [](){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 42;
    });

    // Do something else ?

    std::cout << future.get() << std::endl;

    return 0;
}