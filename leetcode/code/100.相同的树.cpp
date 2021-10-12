/*
 * @lc app=leetcode.cn id=100 lang=cpp
 *
 * [100] 相同的树
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
    bool isSameTree(TreeNode* p, TreeNode* q) {

    }
};
// @lc code=end

/**
 
 ## 思路描述
 
 - 思路：tree的定义是递归的，因此递归判断

 - 描述：
 1. 判断 2个root 是否相同
 2. 然后递归判断左右子树。

 ## 复杂度
  
  - 时间和空间复杂度度：一次遍历 o（n）

 ## cpp代码

~~~cpp
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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        
        if(p ==nullptr && q ==nullptr)
        {
            return true; //对称
        }

        if(p ==nullptr || q ==nullptr)
        {
            return false;//不对称
        }
        if(q->val !=p->val)
        {
            return false;//内存不相等
        }
        return isSameTree(p->left,q->left) && isSameTree(p->right,q->right);
      

    }
};
~~~
**/
