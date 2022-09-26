#include<iostream>
#include<vector>
using namespace std;

class Solution {
public:
     //algorithms run in O(2指数n) time in the worst case;
     //https://leetcode-cn.com/problems/longest-increasing-subsequence/solution/dai-ma-sui-xiang-lu-dai-ni-xue-tou-dpzi-i1kh6/
    //https://programmercarl.com/other/algo_pdf.html
    int lengthOfLIS(vector<int>& nums) {
      
      vector<int> dp(nums.size(),1); //[1 1 1 1 ]
      int result = 0;

      for( int end =1;end<nums.size();end++)
      {
          for(int start =0;start <end;start++)
          {
              if (nums[end] >nums[start])
              {
                  dp[end] =max(dp[end],dp[start]+1);
              }
          }

           if (dp[end] > result) result = dp[end]; // 取长的子序列
      }

      return result;
      //子序列问题是动态规划的一个重要系列，本题算是入门题目，好戏刚刚开始！
    }
};
int main()
{
    Solution test;
    vector<int> input={10,9,2,5,3,7,101,18};
    cout << "out: =" << test.lengthOfLIS(input) << endl;
    return 0;
}