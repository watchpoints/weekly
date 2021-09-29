/*
 * @lc app=leetcode.cn id=347 lang=cpp
 *
 * [347] 前 K 个高频元素
 */

// @lc code=start
class Solution
{
public:
    vector<int> topKFrequent1(vector<int> &nums, int k)
    {

        //01 统计每个元素出现次数 time:o（n logn）
        map<int, int> counts;
        for (auto key : nums)
        {
            counts[key]++; //map key是有序的。但是value无法排序。 需要办法解决！
        }

        //02 为什么是小heap space:o(k)
        //假如 知道一个元素元素出现的频率，如果判断是否k个大，
        //只要和假设前面k个最小的一个比较就可以

        /**定义一下比较函数*/
        auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) { return a.second > b.second; };
        // 定义优先级队列
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> minHeap(cmp);

        //03 求top k time:o（n logn）
        //https://www.cnblogs.com/developing/articles/10890903.htmlC++11新特性——for遍历
        for (auto &key : counts)
        {
            if (minHeap.size() < k)
            {
                minHeap.emplace(key.first, key.second);
            }
            else if (key.second > minHeap.top().second)
            {
                minHeap.pop();
                minHeap.emplace(key.first, key.second);
            }
        }

        //04 打印
        vector<int> ret;
        while (!minHeap.empty())
        {
            int temp = minHeap.top().first;
            minHeap.pop();
            ret.push_back(temp);
        }

        return ret;
    }

    vector<int> topKFrequent(vector<int> &nums, int k)
    {
         //01 统计每个元素出现次数 time:o（n logn）
        map<int, int> counts;
        for (auto key : nums)
        {
            counts[key]++; //map key是有序的。但是value无法排序。 需要办法解决！
        }

        //02 快速排序：构造一个无序数组

        vector<pair<int, int>> data;
        //time:o(n)
        for (auto &k : counts)
        {
            data.emplace_back(k.first,k.second);
        }
        //03 调用快速排序
        quicksort(data,0,data.size()-1,k);
    }
};
// @lc code=end
