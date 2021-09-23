/*
 * @lc app=leetcode.cn id=1381 lang=cpp
 *
 * [1381] 设计一个支持增量操作的栈
 */
//https://github.com/watchpoints/daily-interview/leetcode/code/1381.设计一个支持增量操作的栈.cpp

// @lc code=start
//time (1),space:o(n)
class CustomStack
{
private:
    vector<int> m_data; //随机读写
    int m_maxlen;
    int m_top; //下一个元素位置 m_data没有元素
public:
    CustomStack(int maxSize)
    {
        m_maxlen = maxSize;
        m_data.resize(maxSize); //无扩容操作
        m_top = 0;              //空栈
    }

    void push(int x)
    {
        //满： m_top == m_maxlen
        if (m_top >= m_maxlen)
        {
            return;
        }

        m_data[m_top++] = x;
    }

    int pop()
    {
        //空栈 m_top == 0
        if (m_top <= 0)
        {
            return -1;
        }

        return m_data[--m_top];
    }

    void increment(int k, int val)
    {

        int size = min(k, m_top);
        for (int i = 0; i < size; i++)
        {
            m_data[i] += val;
        }
    }
};

/**
 * Your CustomStack object will be instantiated and called as such:
 * CustomStack* obj = new CustomStack(maxSize);
 * obj->push(x);
 * int param_2 = obj->pop();
 * obj->increment(k,val);
 */
// @lc code=end

class MyQueue
{
public:
    /** Initialize your data structure here. */
    MyQueue()
    {
        m_total = 0;
    }
    /** Push element x to the back of queue. */
    //每次插入一个元素，需要判断是否读取过。然后还原到读取状态
    void push(int x)
    {

        m_total++;
        //
        if (!m_output.empty())
        {
            while (!m_output.empty())
            {
                m_input.push(m_output.top());
                m_output.pop();
            }
        }

        m_input.push(x);

        while (!m_input.empty())
        {
            m_output.push(m_input.top());
            m_input.pop();
        }
    }

    /** Removes the element from in front of queue and returns that element. */
    int pop()
    {
        m_total--;
        int val = peek();
        m_output.pop();
        return val;
    }

    /** Get the front element. */
    int peek()
    {
        //序列：第一个入队列的，需要第一个出队列

        return m_output.top();
    }

    /** Returns whether the queue is empty. */
    bool empty()
    {

        return m_total == 0;
    }

private:
    stack<int> m_input;  //入队列操作
    stack<int> m_output; //出队列操作
    int m_total;         //记录大小
};
ï