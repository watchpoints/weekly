#include <iostream>

using namespace std;

template <class T>
struct Node
{
    T data;

    Node *next;
   
    //comapre
    bool operator()(const T&n1,const T&n2) const
    {
        return n1.data < n2.data;
    }
    bool operator<(const T& other) const
    {
        return this->data < other->data;
    }
};
template <class T>
class MyCircularQueue
{
    // step01-- 采用什么数据结构存储循环队列？
    // 1.1 马上想到book 网络文章都是数组int a[10],根本想不到还有链表这个事情。感觉链表不行。经典 c++primer 例子用链表模拟的
    // https://www.mdnice.com/writing/502e9e93ad5a4d0887e7accec5457a38
    // 1.2   支持任意类型  如果存储数据比较复杂呢 有一定容量可以提交分配
private:
    Node<T> *m_head;
    Node<T> *m_tail;
    int m_length;
    int m_capacity;

    // step02- 如何设计一个类呢.采用什么读写操作 和copy 操作
public:
    bool write(T &&value);
    bool read(T &value);
   
   // step03--- 如何 max main avg
    T& max();
    T& min();
    T& min_second();
    T& max_second();
    

public:
    MyCircularQueue(const MyCircularQueue &other) = delete; //拷贝构造函数没有返回值
    MyCircularQueue &operator=(MyCircularQueue &other) = delete;
    MyCircularQueue(int capacity);
};
template <class T>
bool MyCircularQueue<T>::write(T &&value)
{
    if (m_length == m_capacity)
        return false;
    if (0 == m_length)
    {
        m_tail->data = std::forward<T>(value);
    }
    else
    {
        m_tail = m_tail->next;
        m_tail->data = std::forward<T>(value);
    }
    m_length++;
    return true;
}
template <class T>
bool MyCircularQueue<T>::read(T &value)
{
    if (0 == m_length)
        return false;
 
    //here 操作引用名字：*ptr
     value = std::move(m_head->data);
     m_head = m_head->next;
    //here
    
    m_length --;
    return true;
}

template <class T>
T& MyCircularQueue<T>::max()
{
     T *a =m_head; 
     //visited
     Node<T>* pcur = m_head->next;
     while (pcur !=m_head)
     { 
       if (pcur > a)
       {
         a = pcur; //why  std::move
       }
       pcur = pcur->next;
     }
     return a;
}
int main()
{
    // MyCircularQueue<string> queue(10);
    /**
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
     }**/
    return 0;
}

// https://www.zhihu.com/question/43513150
//如何理解C++中的move语义？
// 1 右值引用是move语义的基础. 右值可以对外不可见，可以安全的释放
// 2 templte 是move语义的的扩展。右值引用 可以执行left值
// Use std::move on rvalue references
