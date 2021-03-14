/*
 * @lc app=leetcode.cn id=148 lang=cpp
 *
 * [148] 排序链表
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
 * https://leetcode-cn.com/problems/insertion-sort-list/solution/wei-tu-jie-147dui-lian-biao-jin-xing-cha-ru-pai-xu/
 */

/**
 * func insertionSortList(head *ListNode) *ListNode {
    dummyHead := &ListNode{Val: 0}
    dummyHead.Next = head
    cur := head
    var prev *ListNode
    var temp *ListNode

    for cur != nil && cur.Next != nil { // cur指针扫整个链表
        if cur.Val <= cur.Next.Val {    // 符合递增，继续推进
            cur = cur.Next
        } else {                        // 找到需要变动的cur.Next
            temp = cur.Next             // 保存给temp
            cur.Next = cur.Next.Next    // 删除结点

            prev = dummyHead            // 从dummy开始扫，用prev推进，找插入的位置
            for prev.Next.Val <= temp.Val { // 继续推进
                prev = prev.Next
            }
            // 此时prev.Next.Val更大，插入到 prev 和 prev.Next 之间
            temp.Next = prev.Next 
            prev.Next = temp     // 先改temp.Next，再接给prev.Next
        }
    }

    return dummyHead.Next // 就算头结点发生改变了，也能通过dummyHead.Next获取到头结点
}

作者：xiao_ben_zhu
链接：https://leetcode-cn.com/problems/insertion-sort-list/solution/wei-tu-jie-147dui-lian-biao-jin-xing-cha-ru-pai-xu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
 */
class Solution {
public:
    //我熟悉链表插入操作，因此采用插入排序，需要三指针保证链表完整性。
    //对链表进行原地翻转,不需要额外空间
    //疑问：为什么采用原理翻转，而不是分割不同链表,在合并？
    //前者每次操作后整个链表是完整的，
    //后者必须完整遍历后获取结果，第i次循环，链表不是完整的。

    ListNode* sortList(ListNode* head) {
      
      if (head == NULL || head->next == NULL)
      {
        return head;
      }
     ListNode dummyHead(-1,head); //固定头节点，保持头节点位置保持不变。

     ListNode * pbefore =&dummyHead; //单链表插入，必须知道前面一个节点。不然无法完成插入操作，删除可以

     ListNode*  ptail = head ;//假设第一个节点已经排序完毕。
     ListNode*  pcur = head->next; //当前节点 ，这是未知元素。
     
     
     while (pcur)
     {
         if(pcur->val >= ptail->val)
         {
            //case01-如果递增数据
            ptail = pcur; //有序链表范围扩大。
            pcur =pcur->next;

         }else
         {
             //case02 当前元素在有序链表中位置
             
             pbefore =&dummyHead;// 寻找pur前面一个元素位置
             while (pcur->val > pbefore->next->val)
             {
                 pbefore =pbefore->next;
             }

             //节点翻转
            //pbefore   ptail   pcur
            //|        |     |
            //|        |     |
            //head--- 5 --  4 --- 3 --2 ---1 

            //01 删除pcur元素 4
            ptail->next =pcur->next; //5————>3

            //02 为了保证链表不被破坏 追究一元素 /4--->5
             pcur->next =pbefore->next;
            
            //03 为了保证链表不被破坏 追究一元素 // tail->4
           pbefore->next = pcur;
          
          pcur = ptail->next;//04 移动操作。
             
         }
         
     }

     return dummyHead.next;
     
    }
};
// @lc code=end

