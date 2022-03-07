/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node() {}
    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }
    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/
class Solution {
public:

   Node* treeToDoublyList1(Node* root){

       Node temp;

   }
    Node* treeToDoublyList(Node* root) {
        
        if (!root)
            return root;
        
        Node dummy(0);
        Node *pre = &dummy, *cur = root;
        stack<Node*> stk;
        while (!stk.empty() || cur){
            while(cur){
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top();
            stk.pop();
            cur->left = pre;
            pre->right = cur;
            pre = cur;
            cur = cur->right;
        }
 
        Node *ret = dummy.right;
        ret->left = pre;
        pre->right = ret;
        return ret;
    }
};