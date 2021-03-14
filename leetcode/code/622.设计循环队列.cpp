/*
 * @lc app=leetcode.cn id=622 lang=cpp
 *
 * [622] 设计循环队列
 */

// @lc code=start
//https://leetcode-cn.com/problems/design-circular-queue/solution/cxun-huan-dui-lie-shu-zu-shi-xian-by-wo-ju-zi-chao/
class MyCircularQueue
{
private:
    vector<int> data; 
    int head;      
    int tail;
    int mask;  
public:
    MyCircularQueue(int k)
    {
        mask = k;
        head = 0;
        tail = -1;
        m_buffer.resize(mask); //
        // 
    }

    bool isEmpty()
    {
        return m_readindex == m_readindex;
    }

    bool isFull()
    {
        return (m_writeindex+1)%m_capacity == m_readindex;
    }
    
    //向循环队列插入一个元素。如果成功插入则返回真
    bool enQueue(int value)
    {
        if (isFull())
        {
            return false;
            //超过容量
        }
        m_buffer[m_writeindex] = value;
        m_writeindex = (m_writeindex+1) % m_capacity;
        return true;
    }

    bool deQueue()
    {
        if (isEmpty())
        {
            return false;
            //m_writeindex ==m_readindex == -1;
            //如果没有记录
        }
    
        m_readindex = (m_readindex+1) % m_capacity;
        
        return true;
    }
    //从队首获取元素。如果队列为空，返回 -1
    int Front()
    {
        if (true == isEmpty())
        {
            return -1;
        }
        return m_buffer[m_readindex];
    }
    //获取队尾元素。如果队列为空，返回 -1
    int Rear()
    {
        if (isEmpty())
        {
            return -1;
        }

        return m_buffer[(m_writeindex -1+m_capacity)%m_capacity];
    }
};

class MyCircularQueue1
{
private:
    vector<int> m_buffer; //三板斧： 数据结构用数组模拟环，这个大家都知道
    int m_capacity;       //最大容量
    int m_readindex;      //代表head，表示已经写入，还没有读取走。前面一个位置已经读取走了。索引小的最早来，最早走。
    int m_writeindex;     //代表tail，已经写入元素位置，下一个元素才是需要写入的。
    //问题1 开始位置等于结束位置时候，如何判断此时是空队列 还是满队列 不清楚
    //问题2:如果一直读取，超过写入怎办？
    //问题3:如果一直写入，覆盖之前写入怎办？
public:
    MyCircularQueue1(int k)
    {
        m_capacity = k;
        m_readindex = -1; //上来不能直接下标获取，不让直接越界了。
        m_writeindex = -1;////上来不能直接下标获取，不让直接越界了
        m_buffer.resize(k); //
        // 
    }

    bool isEmpty()
    {
        return m_readindex == -1;
    }

    bool isFull()
    {
        return (m_writeindex+1)%m_capacity ==m_readindex;
    }
    
    //向循环队列插入一个元素。如果成功插入则返回真
    bool enQueue(int value)
    {
        if (isFull())
        {
            return false;
            //超过容量
        }
        //难点：循环队列插入第一元素：
        if (isEmpty())
        {
           m_writeindex =m_readindex = 0;  //运算符优先级 从右到左
    
        }else{
          m_writeindex = (m_writeindex+1) % m_capacity;
        }

        m_buffer[m_writeindex] = value;
        return true;
    }

    bool deQueue()
    {
        if (isEmpty())
        {
            return false;
            //m_writeindex ==m_readindex == -1;
            //如果没有记录
        }
        //难点：循环队列只剩下最后一个元素。
        if (m_readindex == m_writeindex)
        {
             m_writeindex =m_readindex = -1;
            //细节地方1 
            //假如容量是10，插入5个元素，读取4个，还是剩余最有一个元素，tail=end。
            //在读取一个元素。整个循环队列为空。
            //此时在写入。位置重写移动回到原来位置。
        }else
        {
             m_readindex = (m_readindex+1) % m_capacity;
        }
        return true;
    }
    //从队首获取元素。如果队列为空，返回 -1
    int Front()
    {
        if (true == isEmpty())
        {
            return -1;
        }
        return m_buffer[m_readindex];
    }
    //获取队尾元素。如果队列为空，返回 -1
    int Rear()
    {
        if (isEmpty())
        {
            return -1;
        }

        return m_buffer[m_writeindex];
    }
};

/**
 * Your MyCircularQueue object will be instantiated and called as such:
 * MyCircularQueue* obj = new MyCircularQueue(k);
 * bool param_1 = obj->enQueue(value);
 * bool param_2 = obj->deQueue();
 * int param_3 = obj->Front();
 * int param_4 = obj->Rear();
 * bool param_5 = obj->isEmpty();
 * bool param_6 = obj->isFull();
 */
// @lc code=end
