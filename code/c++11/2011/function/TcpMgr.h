class TcpMgr
{
public:
    void SetDataHandleCb(std::function<void(const string &)> cb)
    {
        m_handleDataCb = cb;
    }
    void OnRecvedData(const string &data)
    {
        // 调用DataMgr::HandleData
        m_handleDataCb(data);
    }

private:

    std::function<void(const string &)> m_handleDataCb; //
};

/**
 多个类之间的交叉调用

后记：

std::function：
https://en.cppreference.com/w/cpp/utility/functional/function


Class template std::function is a general-purpose polymorphic function wrapper. 
类模板 std::function 是一个通用的多态函数包装器。

Instances of std::function can store, copy, and invoke any CopyConstructible Callable target 
-- functions, lambda expressions, bind expressions, or other function objects, as well as pointers to member functions and pointers to data members.

std::function 的实例可以存储、复制和调用任何 CopyConstructible Callable 目标
-- 函数、lambda 表达式、绑定表达式或其他函数对象，以及指向成员函数的指针和指向数据成员的指针。
**/