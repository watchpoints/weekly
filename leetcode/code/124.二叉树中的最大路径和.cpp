/*
 * @lc app=leetcode.cn id=124 lang=cpp
 *
 * [124] 二叉树中的最大路径和
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
class Solution {
public:
    int maxPathSum(TreeNode* root) {
       int  maxPathSum =INT_MIN;
       dfs(root,maxPathSum);
       return maxPathSum;
    }

    int dfs(TreeNode* root,int& maxPathSum)
    {
        if (root == nullptr) return 0;

        int left =dfs(root->left,maxPathSum);
        int right =dfs(root->right,maxPathSum);
        left =max(0,left);
        right =max(0,right);
        maxPathSum = max(maxPathSum,root->val + left+right);

        return root->val + max(left,right);
    }
};
// @lc code=end

