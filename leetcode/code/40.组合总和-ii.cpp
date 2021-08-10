/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
 */

// @lc code=start
class Solution
{
public:
    vector<vector<int>> combinationSum2(vector<int> &candidates, int target)
    {
        //1. 思路：一共n！个组合。判断不重复，并且 sum target->必须抽象 高度n的n叉tree 进行递归回溯

        vector<vector<int>> result;
        vector<int> path; //递归 不是并发，是一个一个执行的。 循环处理：

        //2 组合不一定包括全部元素
        sort(candidates.begin(), candidates.end());

        //2 细节 退出条件 和变量变化
        int start = 0;
        dfs(candidates, target, start, path, result);
        return result;
    }

    void dfs(vector<int> &candidates, int target,int index, 
             vector<int> &path, vector<vector<int>> &result)
    {
        //退出条件
        if (index >= candidates.size() ||  target < 0)
        {
            return ;
        }
        // find 
        if ( target == 0)
        {
            result.push_back(path);
        }

        for(int i =index;i < candidates.size();i++)
        {  
            /**
            if ( i >index && candidates[i] == candidates[i - 1])
            {
                continue;
            }//解集不能包含重复的组合**/

           path.push_back(candidates[i]);
           dfs(candidates,target-candidates[i],i+1,path,res);
           path.pop_back();

        }

};
// @lc code=end
