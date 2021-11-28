#include <functional>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

using namespace std::placeholders; //占位符_N所在的命名空间

using CallBackFuncType = function<void(string const &)>;

class Client
{
public:
    string name;
    CallBackFuncType serverFunc;

    Client() : name("Vergo"), serverFunc(nullptr) {}
    ~Client() {}

    void SetCallBack(const CallBackFuncType &func) { serverFunc = func; }
    void DoCallBack() { serverFunc(name); }
};

class Server
{
public:
    Client *m_clt;
    Server() : m_clt(nullptr) { m_clt = new Client; }
    ~Server()
    {
        if (m_clt)
            delete m_clt;
        m_clt = nullptr;
    }

    //回调函数本数
    void MyCallBackFunc(string const &str) { cout << "The name of client is " << str << endl; }
    //注册回调，将this指针绑定到回调函数中
    void RegCallBackFunc()
    {
        if (!m_clt)
            return;
        m_clt->SetCallBack(CallBackFuncType(std::bind(&Server::MyCallBackFunc, this, _1)));
    }
    //回调
    void GiveMeCallBack()
    {
        if (!m_clt)
            return;
        m_clt->DoCallBack();
    }
};

int main()
{

    Server testClass;
    testClass.RegCallBackFunc();
    testClass.GiveMeCallBack();

    return 0;
}
