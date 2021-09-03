/*
 * @lc app=leetcode.cn id=92 lang=cpp
 *
 * [92] 反转链表 II
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
    
    if (nullptr ==head  || nullptr ==head->next)
    {
        return head;
    }
    ListNode myhead(-1);
    myhead.next =head;

    ListNode* phead=&myhead;//在链表头节点固定位置insert元素

    ListNode* ppre =head;//翻转元素 pre节点,默认就是第一个元素

    ListNode* pcur=head->next;//默认第二个元素
    
    ///////////////////////////////////
    //移动left-1次.【left,right】
    for(int i=0;i<left-1;i++)
    {
        phead =ppre;
        ppre = pcur;
        pcur =pcur->next;

        //cout << "phead =" <<phead->val << " ppre =" <<ppre->val
    //<< " pcur =" <<pcur->val <<endl;

    }
 
    ////////////////////////////

    for(int i =left;pcur && i <right;i++ )
    {
        ppre->next =pcur->next;
        pcur->next =phead->next;
        phead->next =pcur;

        pcur =ppre->next;

    //cout << "phead =" <<phead->val << " ppre =" <<ppre->val
    //<< "pcur =" <<pcur->val <<endl;
    }


    return myhead.next;
    

    }
};
// @lc code=end

