/*
 * @lc app=leetcode.cn id=113 lang=cpp
 *
 * [113] 路径总和 II
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

/*
 *  public interface Interceptor {
    void before(Invocation invocation);
 
    String intercept(Invocation invocation);
 
    void after(Invocation invocation);
 */

class Solution {
private:
//01 定义全局遍历 ，遍历tree计算必须条件
vector<vector<int>> resutl;
vector<int> path;
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        
        //02 递归回溯
        dfs(root,targetSum);
        return resutl;
    }

    void dfs(TreeNode* root, int targetSum)
    {
        if (nullptr == root) return ;
        //开始一个节点处理方式 Interceptor 如何计算
        path.push_back(root->val);
        
        if (root->left == nullptr && root->right == nullptr &&  targetSum -root->val == 0)
        {
            resutl.push_back(path);
        }

        dfs(root->left,targetSum - root->val);
        dfs(root->right,targetSum -root->val);
        //离开一个节点:如何计算
        path.pop_back();
    }
};
//https://blog.csdn.net/qq_15719613/article/details/105158642
// @lc code=end

