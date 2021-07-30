/*
 * @lc app=leetcode.cn id=494 lang=cpp
 *
 * [494] 目标和
 */

// @lc code=start
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
      int path =0;
      int sum =0;
      dfs(0,sum,nums,target,path);
      return path;
    }
  
  //递归 这里是从到下不依赖反馈，因此无返回值
  //利用参数传递，这里不需要path visted 记录顺序
  // 2N
  void dfs(int start,int sum,vector<int>& nums, int target,int& path)
  {   //01 end 
     // if (start == nums.size() -1)
     if (start == nums.size())
      {
          if (sum == target)
          {
              path++;
          }

          return ;
      }

      //iter 

      dfs(start+1,sum+nums[start],nums,target,path);
      dfs(start+1,sum-nums[start],nums,target,path);
  }

};
// @lc code=end

