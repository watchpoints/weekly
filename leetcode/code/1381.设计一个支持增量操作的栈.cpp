/*
 * @lc app=leetcode.cn id=1381 lang=cpp
 *
 * [1381] 设计一个支持增量操作的栈
 */
//https://github.com/watchpoints/daily-interview/leetcode/code/1381.设计一个支持增量操作的栈.cpp

// @lc code=start
class CustomStack {
private:
    vector<int> m_data;//随机读写
    int m_maxlen;
    int m_top ;
 public:
    CustomStack(int maxSize) {
        m_maxlen = maxSize;
        m_data.resize(maxSize);
        m_top =0;
    }
    
    void push(int x) {
    }
    
    int pop() {
    }
    
    void increment(int k, int val) {

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

