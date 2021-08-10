#include "base.h"
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
class Solution10 {
public:
    ListNode* oddEvenList(ListNode* head) {
       if(nullptr == head || nullptr ==head->next)
       {
           return head;
       }

       //链表的第一个节点视为奇数节点 .第一个位置位置没有发生变化
       int index =1 ;

       ListNode* ptail = head;
       ListNode* pcur = head;
       ListNode* ppre = nullptr;

       while(pcur)
       {
           if (index ==1 || index ==2 || (index &1) ==0)
           {
               ppre = pcur;
               pcur = pcur->next;//case1 
            //4%2 ==0
           }else 
           {
               ppre ->next =pcur->next;
               pcur->next =ptail->next;
               ptail->next =pcur;
               ptail = pcur;
               pcur =  ppre ->next;

           }


           index++;
       } 
       return head;
    }
};