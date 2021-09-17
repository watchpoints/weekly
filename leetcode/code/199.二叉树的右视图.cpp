/*
 * @lc app=leetcode.cn id=199 lang=cpp
 *
 * [199] 二叉树的右视图
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
    vector<int> rightSideView(TreeNode* root) {
     
        vector<int> output;
        if(nullptr == root)
        {
            return output;//潜台词：queue的每个元素都必须存在
        }
        queue<TreeNode*> myqueue;//细节：能放int 就能放指针
        myqueue.push(root);

        while(!myqueue.empty())
        {
            int levelnode =myqueue.size(); //计算当前当初
            
            for(int i=0;i<levelnode;i++)
            {
                TreeNode* ptemp =myqueue.front();
                myqueue.pop(); 

                if(i == levelnode-1 )
                {
                    output.push_back(ptemp->val);
                }

                //队列里同时存在i，i+1 层的元素. 
                //通过levelnode-1 判断i层最后一个元素。

                if(ptemp->left)
                {
                    myqueue.push(ptemp->left);
                }

                if(ptemp->right)
                {
                    myqueue.push(ptemp->right);
                }
            } 
        }

        return output;
    }
};
// @lc code=end

