
#include <iostream>
using namespace std;
//结构
//01 设计循环队列的数据结构：链表实现,支持任意类型T
//02 满足读写功能. 参数怎么设计，如何优化
//03 完成max avg 功能  参数怎么设计 不用算法进行优化。
//总结：上来去优化算法干扰设计结构设计。忘记本质

template <class T>
struct Node
{
    T m_data;
    //int_data
    Node *next;
    Node()
    {
        next = NULL;
    }
};

template <class T>
class MyCircularQueue
{
public:
    MyCircularQueue(int size);
    ~MyCircularQueue();
    bool write(T&& elem);
    bool read(T &elem);

public:
    T max();
    T second_max();
    T min();
    T t_min();
    T avg();

private:
    Node<T>* m_head;
    Node<T>* m_tail;
    //Node * m_tail
    int m_capacity;
    int m_length;
};

//MyCircularQueue::MyCircularQueue(int size)
template <class T>
MyCircularQueue<T>::MyCircularQueue(int size)
{
    m_capacity = size;
    m_length = 0;

    // 计算长度 时间复杂度 O(1)
    // m_head ==  m_tail  isEmpty
    // m_tail->next == m_head  isFull
    Node<T> *pcur = new Node<T>();
    Node<T> *ptemp = NULL;
    m_head = pcur;
    m_tail = pcur;

    for (int i = 1; i < size; i++)
    {
        ptemp = new Node<T>();
        pcur->next = ptemp;
        pcur = ptemp;
    }
    pcur->next = m_head;
}

template <class T>
bool MyCircularQueue<T>::write(T &&elem)
{
    if (m_length == m_capacity && m_head == m_tail->next)
    {    
         cout << "full ";
        return false;
    }
    
    // m_head ==  m_tail isEmpty
    if (0 == m_length && m_head == m_tail )
    {
        m_tail->m_data = forward<T>(elem);
    }
    else
    {
        m_tail = m_tail->next;
        
        m_tail->m_data = forward<T>(elem);
        
    }
    m_length++;

    cout <<m_length  << " write "<<m_tail->m_data<< endl;

    return true;
}

template <class T>
bool MyCircularQueue<T>::read(T &elem)
{
    if (0 == m_length )
    {
        cout << " empty" <<endl;
        return false;
    }
    cout <<m_length  << " read "<<m_head->m_data<<endl;

    //code here 
    elem = move(m_head->m_data); 
    m_head = m_head->next;

    //code here
    m_length --;

    return true;
}

template <class T>
T MyCircularQueue<T>::max(){
  T ret;
  Node<T>* ptemp =m_head;
  while (ptemp !=m_tail)
  {
      if (ptemp->m_data > ret)
      {
          ret = ptemp->m_data;
      }
      ptemp = ptemp->next;
  }
    if (ptemp->m_data > ret)
    {
        ret = ptemp->m_data;
    }
  return ret;
}

int main()
{
    MyCircularQueue<string> queue(10);
    for (int i = 0; i < 10; i++)
    {   
        string temp("abc");
        temp.append(to_string(i));
        queue.write(std::move(temp));
    }
    cout << "read 1 max ==="<<queue.max()<<endl; 
    cout << "read 2 max ==="<<queue.max()<<endl; 
    for (int i = 0; i < 10; i++)
    {
        string temp;
        queue.read(temp);
        //cout << temp << endl;
    }
    return 0;
}
//g++ 1.cpp -std=c++11