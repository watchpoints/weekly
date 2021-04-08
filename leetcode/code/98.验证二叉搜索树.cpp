/*
 * @lc app=leetcode.cn id=98 lang=cpp
 *
 * [98] 验证二叉搜索树
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
     //非递归中顺遍历
    //时间复杂度 : O(n)
    //空间复杂度 : O(n)
    bool isValidBST(TreeNode* root) {

        vector<TreeNode* > stack;//stl 是适配器，底层实现是vector
        TreeNode* pre =nullptr;

        //stack 上来不能插任何元素 ，不然root元素重复计算。
        while( !stack.empty() || root )
        {   
            //01 left left left 
            while(root)
            {
                stack.push_back(root);
                root = root->left;
            } //root --null

            root = stack.back();
            stack.pop_back();

             if (pre && pre->val >= root->val )
             {
                 return false;
             }
             pre = root; //

            root =root->right;
        }
        return true;//
    }
};
// @lc code=end

