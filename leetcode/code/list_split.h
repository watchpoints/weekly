#include "base.h"
//分割链表
class Solution86 {
public:

   //【1，2，3，4，5 】 x=3 head->[1 2 3 4 5]
   //【5，4，3，2，1 】 x=3 head->[2 1  5 4 3]
    ListNode* partition(ListNode* head, int x) {
        
        if(nullptr ==head || nullptr ==head->next)
        {
            return head;
        }

        ListNode myhead;
        myhead.next =head;

       ListNode *ptail =&myhead; //保留 两个分区中每个节点的初始相对位置
       ListNode* pcur =head;
       ListNode* ppre =&myhead;//单链表特点：删除一个节点 必须知道前面一个节点

       while(pcur)
       {
           if( pcur->val >=x)
           {   
                // 保持不变  【5，4，3，2，1 】 x=3 head->[2 1  5 4 3]
               ppre =pcur;
               pcur =pcur->next;
              
           }else if (pcur == ptail->next)
           {
               //ptail->【1（pcur），2，3，4，5 】 x=3 head->[1 2 3 4 5]
               // 目的目的就是在ptail后面
               ppre =pcur;
               pcur =pcur->next;

               ptail = ptail->next;

           }else
           {
               //反转
               //ptail->5，4，3(ppre)，2（pcur），1 
               //链表特点：修改其中一个位置，不影响整体

               ppre->next = pcur->next;
               pcur->next =ptail->next;
               ptail->next =pcur; //2(ptail) ->5 ->4 ->3 ->1
               ptail = pcur;

               pcur =pcur->next; //bug1 超时 死循环

           }
       }

       return myhead.next;
    }
};