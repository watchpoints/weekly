#include <iostream>
using namespace std;

//单链表 
template <class T>
struct Node
{
    T data;
    //why 这里为什么不用基本类型指针呢 T* ？使用基本类型 int  a ?
    //why STL 成员 都是封装的迭代器?
    //容器的数据 都是值拷贝
    Node *next;
    Node(T &&value,Node* pnext)
    {
        next = pnext;
        data = forward<T>(value);
    }
    Node()
    {
        next = nullptr;
    }
}

template <class T>
//queue 是适配器容器, 使用 list 作为底层容器。
//循环队列 
class MyCircularQueue
{
private:
    // head 
    Node<T> *m_phead;
    Node<T> *m_ptail;

    int m_capacity;
    int m_length;
    MyCircularQueue & operator=(MyCircularQueue && other);
public:

 MyCircularQueue(int capacity);
 ~MyCircularQueue(); 
 
 bool write(T && value);
 bool read(T & value);

 // 模仿std::queue接口设计
 //What should dequeue return if the queue is empty
 //reference& front()
 //void emplace (Args&&... args)

 T max();

}

template <class T>
//构造循环链表
MyCircularQueue<T>::MyCircularQueue(int capacity)
{   

    //分配capacity大小
    m_capacity = capacity;
    m_length = 0;

    //empty  m_phead == m_ptail && len ==0  
    //full  m_phead == m_ptail->next  && len !=1 

    Node<T>* pcur = new Node<T>();
    m_phead = pcur;
    m_ptail = pcur;
   
    for (int i =1;i<capacity;i++)
    {
        pcur = new Node<T>();

        m_ptail->next = pcur;
        m_ptail = pcur;
    }

    m_ptail->next = m_phead; 
}

//思考：这里为不重写2个版本的 T value, const T && value
template <class T>
bool MyCircularQueue<T>::write(T && value)
{
    if (m_length == m_capacity)
    {
        return false;
    } //约束条件：有容量的

    //第一个元素
    if (0 == m_length)
    {
         m_ptail->data = forward<T>(value);
    }else
    {
        m_ptail = m_ptail->next;
        m_ptail->data = forward<T>(value);
    }

    m_length ++;

    return true;
}

template <class T>
bool MyCircularQueue<T>::read(T & value)
{
    if (m_length == 0)
    {
        return false;
    } 
     //front 
     value = std::move(m_phead->data)
     //pop 
     m_phead =  m_phead->next;
     m_length --;

     return true;
}

int main()
{
    MyCircularQueue<string> queue(10);
    for (int i = 0; i < 5; i++)
    {   
        string temp("abc");
        temp.append(to_string(i));
        queue.write(temp);
        queue.write(std::move(temp));
    }
    cout << "read 1 max ==="<<queue.max()<<endl; 
    cout << "read 2 max ==="<<queue.max()<<endl; 
    for (int i = 0; i < 10; i++)
    {
        string temp;
        queue.read(temp);
        cout << temp << endl;
    }
    return 0;
}