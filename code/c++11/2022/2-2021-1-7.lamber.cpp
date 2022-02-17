#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
int main()
{
    int sum = 0;
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7};
    //define
    auto sum_func = [&sum](int i) -> void { 
        std::cout << i << ' '; 
        sum += i;
    };

    //call
    std::for_each(c.begin(), c.end(), sum_func);
    std::cout << "sum of all : " << sum << std::endl;
    return 0;
}

class Widget: public std::enable_shared_from_this<Widget> {

};

class Y: public enable_shared_from_this<Y>
{
public:
    shared_ptr<Y> f()
    {
        return shared_from_this();
    }
}
int main()
{
    shared_ptr<Y> p(new Y);
    shared_ptr<Y> q = p->f();
    assert(p == q);
    assert(!(p < q || q < p)); // p and q must share ownership
}


struct AsynchronousPeriodicTimer
{
    // call this periodically on some thread...
    void SetCallback(std::function<void(void)> callback); 
    void ClearCallback(); // clears the callback
}

struct Client
{
    Client(std::shared_ptr< AsynchronousPeriodicTimer> timer) 
        : _timer(timer)

    {
        _timer->SetCallback(
            [this]
            () 
            {
                assert(this); // what if 'this' is already dead because ~Client() has been called?
                std::cout << ++_counter << '\n';
            }
            );
    }
    ~Client()
    {
        // clearing the callback is not in sync with the timer, and can actually occur while the callback code is running
        _timer->ClearCallback();
    }
    int _counter = 0;
    std::shared_ptr< AsynchronousPeriodicTimer> _timer;
}

int main()
{
    auto timer = std::make_shared<AsynchronousPeriodicTimer>();
    {
        auto client = std::make_shared<Client>(timer);
        // .. some code    
        // client dies here, there is a race between the client callback and the client destructor           
    }
}

calss Client : std::enable_shared_from_this<Client>
{
Client(std::shared_ptr< AsynchronousPeriodicTimer> timer) 
    : _timer(timer)

    {

    }

    void Init()
    {
        auto captured_self = weak_from_this(); // weak_ptr to avoid cyclic references with shared_ptr

        _timer->SetCallback(
        [captured_self]
        () 
        {
            if (auto self = captured_self.lock())
            {
                // 'this' is guaranteed to be non-nullptr. we managed to promote captured_self to a shared_ptr           
                std::cout << ++self->_counter << '\n';
            }

        }
        );
    }
    ~Client()
    {
        // the destructor cannot be called while the callback is running. shared_ptr guarantees this
        _timer->ClearCallback();
    
    }
    int _counter = 0;
    std::shared_ptr< AsynchronousPeriodicTimer> _timer;
}