#ifndef  BASE_H
#define  BASE_H
struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
};
//如果插入一个链表
ListNode* createList( vector<int>& input)
{
    ListNode head;
    ListNode* ptail=&head;

    for(int i =0;i<input.size();i++)
    {
        
        ListNode * ptemp  =new ListNode(input[i]);
        ptail->next =ptemp;
        ptail =ptemp;
    }
    return head.next;
}

void show(ListNode* phead )
{
    while(phead)
    {
      cout<< phead->val << " ";
      phead = phead->next;
    }
    cout <<endl;

}


void show(vector<int>& input)
{  
  for(int i =0;i<input.size();i++)
  {
      cout<< input[i] << " ";
  }
  cout <<endl;
}

#endif
