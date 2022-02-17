#include<iostream>
#include<memory>
using namespace std;

struct Node
{
    int _value;

    //shared_ptr<Node> _next;
    //shared_ptr<Node> _prev;

    weak_ptr<Node> _next;
    weak_ptr<Node> _prev;

    ~Node()
    {
        cout << " ~Node()  "<<_value << endl;
    }
    Node( int input)
    {
        _value = input;
    }
};



int main()
{
    shared_ptr<Node> sp1(new Node(1));
    shared_ptr<Node> sp2(new Node(2));

    sp1->_next = sp2; //为什么：shared_ptr 转变成 weak_ptr 不增加 shared_ptr引用计数呢？
    sp2->_prev = sp1; //为什么：shared_ptr 转变成 weak_ptr 不增加 shared_ptr引用计数呢？

    cout<<sp1.use_count()<<endl; //引用计数-->1 不是 2

    cout << sp2.use_count() << endl; //引用计数-->1 不是 2

    return 0;
}
//https://blog.csdn.net/qq_36430106/article/details/89441856