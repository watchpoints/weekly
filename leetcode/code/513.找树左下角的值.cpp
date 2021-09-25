/*
 * @lc app=leetcode.cn id=513 lang=cpp
 *
 * [513] 找树左下角的值
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
class Solution1
{
public:
    //思路1 ：深度优先搜索：中序遍历(In-Order Traversal)
    //特点：回溯时候在访问该元素。
    //https://zh.wikipedia.org/wiki/%E6%A0%91%E7%9A%84%E9%81%8D%E5%8E%86
    int findBottomLeftValue(TreeNode *root)
    {
        int maxlevel = -1;
        int leftValue = 0;
        int curlevel = 0;

        dfs(root, curlevel, maxlevel, leftValue);
        return leftValue;
    }
    //元素比较：每层最left
    void dfs(TreeNode *root, int curlevel, int &maxlevel, int &left)
    {
        if (nullptr == root)
        {
            return; //结束条件 必备
        }
        if (root->left)
        { //减少一层递归
            dfs(root->left, curlevel + 1, maxlevel, left);
        }
        //优化：裁剪
        //其他遍历方式可以吗？可以
        //这里上来设置maxlevel=3。小于3的不需要设置

        if (curlevel > maxlevel)
        {
            maxlevel = curlevel;
            left = root->val;
        }

        if (root->right)
        { //减少一层递归
            dfs(root->right, curlevel + 1, maxlevel, left);
        }
    }
};

class Solution
{
public:
    int findBottomLeftValue(TreeNode *root)
    {
        int output = 0;
        queue<TreeNode*> myqueue;
        myqueue.push(root); //循环条件

        while (!myqueue.empty())
        {
            int len = myqueue.size();

            for (int i = 0; i < len; i++)
            {
                TreeNode *ptemp = myqueue.front();
                myqueue.pop();
                if (i == 0)
                {
                    output = ptemp->val;
                    //每层第一个元素
                }
                if (ptemp->left)
                {
                    myqueue.push(ptemp->left);
                }

                if (ptemp->right)
                {
                    myqueue.push(ptemp->right);
                }
            }

            return output;
        }
    }
};
// @lc code=end

/**
 
## 思路描述：

- 思路1 ：深度优先搜索
1. 看到这个题目马上dfs遍历，
但是最左边 节点的值如何寻找呢？不清楚
用vector 表示没一层记录？还是定义2个变量表示。
2. 观察 左边不一定是left的left。
   题目 右视图，【先顺遍历】这里存储全部元素。
3. 简化 题目是每层最 left的。定义2个变量就可以。
4. 重点：只访问最left元素。其他元素不访问。 
  只有i层 大于i-层

- 描述：
1. 假设当前层次i层，maxlevel=i,中序遍历(In-Order Traversal)
2. 当前层次i+1,第一遇到 i+1>maxlevel 命中。
3. 然后继续root.right中序遍历
参考：
https://leetcode-cn.com/problems/find-bottom-left-tree-value/solution/dai-ma-sui-xiang-lu-dai-ni-xue-tou-er-ch-w3og/
https://leetcode-cn.com/problems/find-bottom-left-tree-value/solution/di-gui-jian-yi-hua-tu-by-zhouzihong-x2yp/

- 思路2:

只要深度优先松搜索实现的代码，广度也可以。
这里同样问题。每一层第一个，就是最left。
- 描述：
1. 定义队列；
2. 遍历队列：队列同时存在i ，i+1层元素。
   统计当前层的个数
3. 第一个就是当前层次的。
4.重复 步骤2和3.

## 回顾:o(n)
题目--右视图--左视图--叶子节点的左视图---定义2该个变量
--优化 前序遍历改为-->中序遍历
**/
