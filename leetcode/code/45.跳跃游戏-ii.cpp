/*
 * @lc app=leetcode.cn id=45 lang=cpp
 *
 * [45] 跳跃游戏 II
 */

// @lc code=start
class Solution {
public:
    int jump(vector<int>& nums) {
     
     int n =nums.size();
     //01 定义状态
     vector<int> dp(n,n); //达数组的第i个位置,使用最少的跳跃次数
     //最坏情况：[1,1,1]
     //02 初始化状态
     dp[0] = 0;

     //03 构造状态转移方程：索引
     ////watch
     // 条件：start+a[start] >=end   0<=start <end
     ////变化：dp[end] =min(dp[end],dp[start]+1)
     for (int end = 1; end < n; end++)
     {
         for (int start = 0; start < end; start++)
         {
             if (start + nums[start] >= end)
             {
                 dp[end] = min(dp[end], dp[start] + 1);
             }
         }
     }
     //04 
     //f(n) =n*(n+1)/2
     // o(n) =n2;
     return dp[n-1];
    }
};
// @lc code=end

