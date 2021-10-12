/*
 * @lc app=leetcode.cn id=104 lang=cpp
 *
 * [104] 二叉树的最大深度
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
class Solution
{
public:
    int maxDepth(TreeNode *root)
    {
        if (nullptr == root)
        {
            return 0;
        }
        int depth = 0;
        queue<TreeNode *> myqueue; //层次遍历
        myqueue.push(root);

        while (!myqueue.empty())
        {
            int total = myqueue.size();
            for (int i = 0; i < total; i++)
            {
                TreeNode *ptemp = myqueue.front();
                myqueue.pop(); //pop

                if (ptemp->left)
                {
                    myqueue.push(ptemp->left);
                }
                if (ptemp->right)
                {
                    myqueue.push(ptemp->right);
                }
            }
            depth++;
        }

        return depth;
    }
};
// @lc code=end

/**


## 思路描述
- 方法1 深度优先遍历

- 方法2 广度优先遍历
1. 利用队列遍历，队列本身没有区分高度
2. 通过pop当前层次的元素。

## 复杂度
- 时间复杂度：O(n)


## 代码

~~~cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (root == 0)
        {
            return 0;
        }
        return max(maxDepth(root->left),maxDepth(root->right))+1;
    }
};
~~~

~~~cpp
class Solution
{
public:
    int maxDepth(TreeNode *root)
    {
        if (nullptr == root)
        {
            return 0;
        }
        int depth = 0;
        queue<TreeNode *> myqueue; //层次遍历
        myqueue.push(root);

        while (!myqueue.empty())
        {
            int total = myqueue.size();
            for (int i = 0; i < total; i++)
            {
                TreeNode *ptemp = myqueue.front();
                myqueue.pop(); //pop

                if (ptemp->left)
                {
                    myqueue.push(ptemp->left);
                }
                if (ptemp->right)
                {
                    myqueue.push(ptemp->right);
                }
            }
            depth++;
        }

        return depth;
    }
};
~~~

**/
