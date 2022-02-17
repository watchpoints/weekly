/*
 * @lc app=leetcode.cn id=328 lang=cpp
 *
 * [328] 奇偶链表
 *
 * https://leetcode-cn.com/problems/odd-even-linked-list/description/
 *
 * algorithms
 * Medium (65.63%)
 * Likes:    413
 * Dislikes: 0
 * Total Accepted:    105.2K
 * Total Submissions: 160.4K
 * Testcase Example:  '[1,2,3,4,5]'
 *
 * 给定一个单链表，把所有的奇数节点和偶数节点分别排在一起。请注意，这里的奇数节点和偶数节点指的是节点编号的奇偶性，而不是节点的值的奇偶性。
 * 
 * 请尝试使用原地算法完成。你的算法的空间复杂度应为 O(1)，时间复杂度应为 O(nodes)，nodes 为节点总数。
 * 
 * 示例 1:
 * 
 * 输入: 1->2->3->4->5->NULL
 * 输出: 1->3->5->2->4->NULL
 * 
 * 
 * 示例 2:
 * 
 * 输入: 2->1->3->5->6->4->7->NULL 
 * 输出: 2->3->6->7->1->5->4->NULL
 * 
 * 说明:
 * 
 * 
 * 应当保持奇数节点和偶数节点的相对顺序。
 * 链表的第一个节点视为奇数节点，第二个节点视为偶数节点，以此类推。
 * 
 * 
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
class Solution1 {
public:
    /**
     * 观察特点：翻转奇数后，剩下的就是偶数 
     * 01 模型：
     *   原地翻转单链表，
     *   单链表特点______，
     *   因此需要三路指针
     * 02: 删除节点：删除节点编号
     *    对偶数节点采取策略是：遍历变量。
     *  
     * 03 插入节点： 在链表位节点插入
     *     1->3->5
     *    应当保持奇数节点和偶数节点的相对顺序 
     *   
     */
    ListNode* oddEvenList(ListNode* head)
    {
        if (nullptr == head || nullptr == head->next)
            return head;

        //定义辅助空间，并且初始化
        ListNode* ptail = head; //1:假设ptail有序节点最后一个元素。应当保持奇数节点和偶数节点的相对顺序
        ListNode* pcur = head->next; //2
        ListNode* ppre = head; //1
        int index = 0;

        //04 链表遍历 和翻转结合在一起了。 难度升级
        while (pcur) {

            //偶数移动
            if ((index & 1) == 0) {
                ppre = pcur;
                pcur = pcur->next;
            } else if ((index & 1) == 1) {
                //1(ptail)--2(ppre)--3(pcur)--4

                //在链表位节点插入 需要四个步骤

                // 删除 --删除--连接--移动
                ppre->next = pcur->next;
                pcur->next = ptail->next;
                ptail->next = pcur;
                ptail = pcur;

                pcur = ppre->next; //单链表需要记录下一个元素位置
            }
            index++;
        }

        return head;
    }
};

class Solution3 {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (head == NULL || head->next == NULL) {
            return head;
        } 
       
       int index =2;
       // step01- var 
       ListNode* ptail = head;

       ListNode* pcur = head->next;

       ListNode* ppre =head;

       while(pcur)
       {
          //基本操作2 遍历 
          if (0  == index % 2){
            ppre = pcur;
            pcur = pcur->next;
          }else {
              //奇数  基本操作 3 翻转 删除 插入

              ppre->next = pcur->next ; 
              
              pcur->next  = ptail->next; 

              ptail->next = pcur;

              ptail = pcur;

              pcur = ppre->next; //单链表需要记录下一个元素位置

          }

          index++;
       }

       return head; //相对顺序不变，head是第一个，翻转后还是第一个。基本操作1：在链表尾节点插入一个元素。


    }
};

//Go
/****************************
func oddEvenList(head *ListNode) *ListNode {
    if head == nil  || head.Next == nil {
        return head
    }
     
    //space:
    index :=2
    pcur :=head.Next
    ppre :=head
    ptail:=head
    //for condition { } while
    for pcur !=nil {
        if index%2 == 0 {
            ppre = pcur
            pcur =pcur.Next
        }else {
            ppre.Next = pcur.Next
            pcur.Next = ptail.Next
            ptail.Next = pcur
            ptail = pcur

            pcur = ppre.Next
        }
        index ++
    }
    return head

}
**/
// @lc code=end
