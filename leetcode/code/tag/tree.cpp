
 struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    };

class Solution1
{
public:
    bool isValidBST(TreeNode *root)
    {
        TreeNode* ppre =nullptr;

        return isValidBST(root, ppre);
    }

    bool isValidBST(TreeNode *proot, TreeNode*& ppre)
    {
        if (nullptr == proot)
        {
            return true;
        }
        bool flag = isValidBST(proot->left, ppre);
        if (flag == false )
        {
            return false;
        }
        //忘记判断了。这么重要怎么忘记。
        if (ppre && ppre->val >= proot->val)
        {
            return false;
        }
        ppre = proot;
  
         return isValidBST(proot->right, ppre);
    }
};
class Solution2
{
public:
    //给定一个二叉树，确定它是否是一个完全二叉树。 
    bool isCompleteTree(TreeNode *root)
    {
        int total = getNode(root);
        return isCompleteTree(root, 1,total);
    }
    bool isCompleteTree(TreeNode *root, int index,int total)
    {
        if (root == nullptr)
        {
            return true;
        }

        if (index > total)
        {
            return false;
        }

        return isCompleteTree(root->left, 2 * index, total) && isCompleteTree(root->right, 2 * index + 1, total);
    } 

    int getNode(TreeNode *root)
    {
        if (nullptr == root)
        {
            return 0;
        }

        return 1 + getNode(root->left) + getNode(root->right);
    }

};
//https://www.nowcoder.com/questionTerminal/380d49d7f99242709ab4b91c36bf2acc
int main()
{
    return 0;
}