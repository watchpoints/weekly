#include <iostream>

using namespace std;

template <class T>
struct Node
{
    T data;
    Node *next;
    Node(T value)
    {
        data = value;
        next = nullptr;
    }
};

template <class T>
class CircularQueue
{
private:
    Node<T> *m_head;
    Node<T> *m_tail;
    int capacity;
    int length; 

public:
    CircularQueue(int size)
    {
        capacity = size;
        length = 0;
        for (int i = 0; i < size; i++)
        {
            Node<T> *ptemp = new Node<T>(i);
            if (i == 0)
            {
                m_tail = ptemp;
                m_head = ptemp;
            }
            else
            {
                m_tail->next = ptemp;
                m_tail = ptemp;
                if (i == size - 1)
                {
                    m_tail->next =m_head;
                }
            }
        }

          // empty  m_tail == m_head;
          m_tail =  m_head;
    }
    bool isEmpty() {
        if(length==0)
            return true;
        return false;
    }
    
    bool isFull() {
        if(length==capacity)
            return true;
        return false;
    }

     /** Insert an element into the circular queue. Return true if the operation is successful. */

     //小王疑问：仿照rock的channlewirte写法没有问题，但是&&怎么操作根部不过
     // rock 采取用stl queu操作 你该怎么操作
     
     bool enQueue(T &&elem) 
     {
         if (isFull())
         {   
             cout<< "isFull" <<endl;
             return false;
         }

         m_tail->data =std::forward<T>(elem); //left使用
         //函数的实现只有一个版本。
         //forward 有2个版本时满足完美转发。
         //具体的拷贝构造，构造函数 复制函数怎么操作的 自定义。

         m_tail = m_tail->next;
         length ++;

         return true;
     }
     
     bool 
    

};

int main(int argc, char *argv[])
{
    CircularQueue<int> myqueue(3);
    myqueue.enQueue(1);
    myqueue.enQueue(2);
    myqueue.enQueue(3);
    myqueue.enQueue(4);
    
    cout << myqueue.Rear() <<endl;

    return 0;
}