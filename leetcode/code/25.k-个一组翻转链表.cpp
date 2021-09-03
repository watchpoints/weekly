/*
 * @lc app=leetcode.cn id=25 lang=cpp
 *
 * [25] K 个一组翻转链表
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
    // ListNode* reverseGroup(ListNode* head,int k);
    ListNode* reverseKGroup(ListNode* head, int k) {
      
      //01 
      if (nullptr == head || nullptr ==head->next || 1 ==k)
      {
          return head;
      }
      
      //02 

      ListNode myhead;
      myhead.next =head;
      int count =0;
      while(head)
      {
          count++;
          head =head->next;
      }
      head =myhead.next;

      //03  循环次数： count/k
 
      ListNode* phead =&myhead;
      ListNode* ppre =head;
      ListNode* pcur =head->next;

       //phead->1(ppre)-->2(pcur)  --3--4 -5

      //默认第一个元素不用翻转，
      //第一个元素翻转后变成最后一个元素
      
     // head>--2---1(pre) ->3(pcur) --4()

     //head>2-1(phead) -->3(ppre) -->4(pcur)
     //len -= k;
     int j =count/k;
      while( j-->0)
      {
          for(int i=0;i<k-1;i++)
          {
               ppre->next =pcur->next;

               pcur->next =phead->next;
               phead->next = pcur;

               pcur =ppre->next;
          }
          if(pcur)
          {
                phead =ppre;
                ppre =pcur;
                pcur =pcur->next;
          }
       

      }
     
     return myhead.next;


    }
};
// @lc code=end

