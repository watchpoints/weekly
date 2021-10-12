/**


## 思路描述：

- 方法1:直接做题。字符串操作感觉有点麻烦。
1.1 遍历一个tree

1.2 计算从root到叶子节点每个路径，然后保存全局遍历vector

1.3 用字符串变成整数然后在累加。

- 方法2: 遍历过程同时完成计算。
2.1. 递归遍历tree, 
     计算：cur=last*10+cur->val; 
     root时候last为0
2.2. 从下到上返回结果时候，搜集每个叶子节点路径的值。


## 复杂度：
- 时间复杂度分析： O(n)，其中 n是二叉树的节点个数，对每个节点访问一次。

## cpp代码

~~~
class Solution
{
public:
    int sumNumbers(TreeNode *root)
    {
        int last = 0; //root上一个元素为0
        return dfs(root, last);
    }

    int dfs(TreeNode *root, int last)
    {
        if (root == nullptr)
        {
            return 0; //退出条件1 case1 【1,null,5】
        }
        if (nullptr == root->left && nullptr == root->right)
        {
            return last * 10 + root->val; //退出条件2 case【1,null,null】 完成叶子节点计算。
        }
        //非叶子节点 递归逻辑

        return dfs(root->left, last * 10 + root->val) + dfs(root->right, last * 10 + root->val);
    }
};
~~~

**/

/*
 * @lc app=leetcode.cn id=129 lang=cpp
 *
 * [129] 求根节点到叶节点数字之和
 */

class Solution
{
public:
    int sumNumbers(TreeNode *root)
    {
        int last = 0; //root上一个元素为0
        return dfs(root, last);
    }

    int dfs(TreeNode *root, int last)
    {
        if (root == nullptr)
        {
            return 0; //退出条件1 case1 【1,null,5】
        }
        if (nullptr == root->left && nullptr == root->right)
        {
            return last * 10 + root->val; //退出条件2 case【1,null,null】 完成叶子节点计算。
        }
        //非叶子节点 递归逻辑

        return dfs(root->left, last * 10 + root->val) + dfs(root->right, last * 10 + root->val);
    }
};
// @lc code=end
