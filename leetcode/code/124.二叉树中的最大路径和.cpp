/*
 * @lc app=leetcode.cn id=124 lang=cpp
 *
 * [124] 二叉树中的最大路径和
 */
/Users/wangchuanyi/doc/daily-interview/leetcode/code/124.二叉树中的最大路径和.cpp
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
class Solution1 {
public:
    int maxPathSum(TreeNode* root)
    {
        int maxPathSum = INT_MIN;
        dfs(root, maxPathSum);
        return maxPathSum;
    }

    int dfs(TreeNode* root, int& maxPathSum)
    {
        if (root == nullptr)
            return 0;

        int left = dfs(root->left, maxPathSum);
        int right = dfs(root->right, maxPathSum);
        left = max(0, left);
        right = max(0, right);
        maxPathSum = max(maxPathSum, root->val + left + right);

        return root->val + max(left, right);
    }
};

class Solution {
public:
    /***
     * 01 数据模型：
     *  路径定义：root.val+ 左节点的路径 + 右节点路径 这个是错误的理解
     * 路径定义：root.val+ 左节点到叶子节点最长路径 + 右节点到叶子节最长路径 
     * 
     * 02 数据结构 tree ,tree 特点是无数个路径，类比tree的高度 
     *  区分 一个
     * 
     * 03 算法描述：
     *   一次遍历 计算2个结果：最长路径，和最大路径
     * 04  考察 tree 高度，递归定义 
     *   负数就是对思考干扰项
     */
    int maxPathSum(TreeNode* root)
    {
        int path = INT_MIN;
        dfs(root, path);
        return path;
    }
    //01 什么情况下指针，什么情况引用
    // 引用是通过指针实现. 如何证明引用本身大小是8bit呢？
    //02 返回值是什么？？
    int dfs(TreeNode* root, int& path)
    {
        if (root == nullptr) {
            return 0;
        }

        int left = dfs(root->left, path);
        int right = dfs(root->right, path);

        left = max(0, left);
        right = max(0, right);

        path = max(path, root->val + left + right);

        return root->val + max(left, right);
    }
};
// @lc code=end
