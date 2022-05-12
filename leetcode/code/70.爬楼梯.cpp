/*
 * @lc app=leetcode.cn id=70 lang=cpp
 *
 * [70] 爬楼梯
 */

// @lc code=start
class Solution {
public:
    //思考：递归 递归回溯和动态规划直接的的区别？
    int climbStairs(int n) {
        if( 0 ==n || 1 ==n ) return 1;
        if ( 2 == n ) return 2;

        vector<int>dp(n+1,0);
        dp[1] = 1;
        dp[2] = 2;

        for(int i=3;i<n;i++)
        {
            dp[i] =dp[i-1] +dp[i-2];
        }

        return dp[n];
    }
};
// @lc code=end

