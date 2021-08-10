/*
 * @lc app=leetcode.cn id=53 lang=cpp
 *
 * [53] 最大子序和
 */

// @lc code=start
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
    
     //01 定义子问题的状态
    // dp[i] ：表示以 nums[i] 结尾 的 i个连续子数组的最大和一个。
    
    // 说明：假如 i=10，i =10就是确定的。但是开始位置不确定的。
    //因为子问题10个子串
   
     int n =nums.size();
     vector<int> dp(n,0);

     //02 思考状态转移方程 符合条件 和不符合条件的2个公示
     //全都严格大于 0，那么一定有 dp[i] = dp[i - 1] + nums[i] i-1>=0;
     //1 2 3
     //如果 dp[i - 1] <= 0   dp[i] =  + nums[i]
     dp[0] = nums[0];
     int ret = dp[0];
     //03 遍历 f(n) =n-1; o(n) =n
     for (int i = 1; i < n; i++)
     {   
         //状态转移方程
         if (dp[i - 1] >0 )
         {
             dp[i] = dp[i - 1] + nums[i];
         }else
         {   //状态转移方程
             dp[i]  =nums[i];
             //[-1,2]
         }
         ret = max(ret, dp[i]);
     }
     /**
     for(int i =0;i<n;i++)
     {
       cout<< dp[i] <<endl;
     }**/
     //04 错误认知：n 越大，最有一个值累加越大，返回最后一个就可以了
     // 2次循环变成一次循环。遗漏什么
     //return dp[n-1];
       // [1]
       //[1 2]
       //[-1.2]
     return ret;
    }

    

};
// @lc code=end

