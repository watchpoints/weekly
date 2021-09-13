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
