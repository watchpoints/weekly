//
   1
 2    3
4 5
// 42512

void show(Node* root)
{
    stack<Node*> mystack;
    while(NULl != root || !mystack.empty())
    {
        while(NULl != root)
        {
            mystack.push(root);
            root = root->left;
        }

        root =mystack.top();
        mystack.pop();
        print(root);
        
       root = root->right;
    }
}

class Solution {
public:
    /*
     * @param root: The root of the tree
     * @param A: node in the tree
     * @param B: node in the tree
     * @return: The lowest common ancestor of A and B
     */
    ParentTreeNode * lowestCommonAncestorII(ParentTreeNode * root, ParentTreeNode * A, ParentTreeNode * B) {
        // write your code here

        if( nullptr == root) return nullptr;

        if (root == A || root ==B) return root;
        ParentTreeNode* left = lowestCommonAncestorII(root->left,A,B);
        ParentTreeNode  right = lowestCommonAncestorII(root->left,A,B);

        if ( left  && right )
        {
            return root;
        }

        return left == nullptr?right:left;
    }
};

