/*
 * @lc app=leetcode.cn id=147 lang=cpp
 *
 * [147] 对链表进行插入排序
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
    //大家好，考试开始了，考试是不允许大声喧哗的，直接写代码。

    //1 采用数据结构：单链表插入排序

    //2 算法步骤：考察删除和插入基本，更加复杂概念不考虑。
    //这也是链表的优点。 copy(begin,end,inertinerator)

    //关键点1：单链表，删除和插入 需要知道前面一个节点。 三路指针

    //关键点2: 随着遍历开始，链表头节点是不断发送变化的。 需要固定头节点。
    //类比翻转单链表，链表开始head 不断发送变化的。

    //3 time：o(n) 最好情况
    //space：o(1)

    //排序--插入排序（一次遍历链表）--> 删除和插入基本操作--->需要 3个变量--->返回head变化的-->固定头节点方法——>while 2个情况

    ListNode* insertionSortList(ListNode* head) 
    {
        if (nullptr == head || nullptr == head->next)
        {
            return head;//easy case1 head->null
        }
       
       //关键点2: 随着遍历开始，链表头节点是不断发送变化的。 需要固定头节点。
       ListNode fixed(-1,head);

      //关键点1：单链表，删除和插入 需要知道前面一个节点。 三路指针
      ListNode* pcur = head->next;
      ListNode* ppre = head; // 插入排序假设第一个元素有序，如果后面元素比他小，插入第一个元素前面fixed发挥作用了
      ListNode* ptail =head;


      //链表原地翻转
      while(pcur)
      {
        //这里分 2个情况 
        //eays case 2  1->2(pcur)->3->4->5-null

        if (pcur->val > ppre->val)
        {
            ppre = pcur;
            pcur = pcur->next;
            
            //eays case 2  1->2->3(pcur)->4->5-null
            //链表什么都没有改变
        }else
        {
            //类比 翻转链表 
            //在链表中插入一个元素，整个链表是完整的.

            ptail = &fixed;

            // ->10 --20 
            //insert 15
            //find 10
            while( pcur->val >= ptail->next->val )
            {
               ptail = ptail->next;
            }

            //在链表中插入一个元素，整个链表是完整的. ptail

            //eays case 3   --->10(ptail) ->20(ppre) ->15(pcur) ->30

            //move 
            ppre->next = pcur->next; //保证删除之后链表完整性 20->30

            //insert 

            pcur->next = ptail->next; // 保证插入一个链表完整性 15->20

            //ptail 发送变化

            ptail->next = pcur; //10->15->20

        }
        
      }//end while
      return fixed.next;
    }
};
// @lc code=end

