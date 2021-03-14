/*
 * @lc app=leetcode.cn id=78 lang=cpp
 *
 * [78] 子集
 * 
 * 
 */
/***
 *  重复 
 */
// @lc code=start
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {

        vector<vector<int>> result;
        vector<int> path;
        dfs(0,nums,path,result);
        return result;
    }
    void dfs(int index ,vector<int>& nums,vector<int>& path,vector<vector<int>> &result)
    {
       if (index > nums.size())
       {
          return ;
       }
       result.push_back(path); //sav
       for (int i=index;i<nums.size();i++)
       {
          path.push_back(nums[i]);
          
          dfs(i+1,nums,path,result);

          path.pop_back();
       }
    }
};
// @lc code=end

