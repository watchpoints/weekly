/*
 * @lc app=leetcode.cn id=239 lang=cpp
 *
 * [239] 滑动窗口最大值
 */

// @lc code=start
class Solution
{
public:
    /** 
     * https://leetcode-cn.com/problems/sliding-window-maximum/solution/ru-he-shan-chu-sui-ji-yi-ge-zhi-by-aleaf-whvf/
     * 1. 思路：
     *    给我大小为k的数组，求最大值 很多方法 遍历，堆
     *    这里删除第一个元素（做不到，删除最大值），然后在添加最后新元素（可以做到）
     *    此时最大值 与删除第一个元素元素关系是什么？
     *    是
     *    与不是
     *    这个判断
     *   【i-k,,,,i】   i-k <=j<=i
     * 2. 描述：
     *    2.1 遍历 n个元素 0到k元素 优先级队列 返回最大值
     *    2.2 遍历剩余n-k个元素，增加一个元素，返回最大值
     *    最大值：在滑动窗口内，需要index判断。
     *    如果符合条件：就是最大值
     *    如果不符合条件，删除一个元素 重复执行。
     * 3. 复杂度：
     *    时间复杂度：O(n log n)
     *    空间复杂度：O(n).这里不是o（k）【123456】
     * 
     * 4. 回顾：
     *    看到难题目要拆分熟悉部分。
     *    top k马上想到堆排序。
     *    大堆 获取最大值。【1，2 3】 --[3,2,1]-删除1怎么办？
     *   //【5，4，3，2，1，】
     *    
     *    前置知识：队列滑 动窗口
     * 
     */
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        //01
        priority_queue<pair<int, int>> bigheap;
        //nums[indxe]--index
        //A priority queue is a container adaptor that provides constant time lookup of the largest (by default) element
        //02  不够k个元素
        for (int i = 0; i < k; i++)
        {
            //https://en.cppreference.com/w/cpp/container/priority_queue
            bigheap.emplace(nums[i], i); //push({nums[i],i});
        }
        vector<int> ret;
        ret.push_back(bigheap.top().first); // k个元素有一个最大值

        //03 k个元素获取一个最大值

        for (int i = k; i < nums.size(); i++)
        {
            //队列插入一个新元素o（logn）
            bigheap.emplace(nums[i], i); //push({nums[i],i});

            //如果最大值，是需要删除的第一个 [i-k,i i+1]
            // 最大值合法
            while (bigheap.top().second <= i - k)
            {
                bigheap.pop();

                //Removes the top element from the priority queue.
            }
            ret.push_back(bigheap.top().first);
        }

        return ret;
    }
};
// @lc code=end
