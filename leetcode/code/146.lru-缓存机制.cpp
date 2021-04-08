/*
 * @lc app=leetcode.cn id=146 lang=cpp
 *
 * [146] LRU 缓存机制
 */

// @lc code=start

struct Node
{
    int key;
    int value;
    Node* pre ;
    Node* next;
};
class LRUCache {
private:
  map<int,Node*> hash;
  Node* head;// 头节点插入
  Node* tail;//wei节点删除
  int size ;

public:
    LRUCache(int capacity) {
     
      size = capacity;
      
      //为了减少if 判断各种异常情况. 
      head = new Node();
      tail = new Node();

      // head  tail
      head->next = tail;
      tail->pre =head;

    }
    
    int get(int key) {

        //01 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1 。
        if (hash.count(key) == 0 )
        {
            return -1;
        }
        Node* ptemp = hash[key];
        //02 调整优先级, 翻转链表
        //head-1->2->tail 
        //head-2->1->tail 
         
         //a: move
          ptemp->pre->next = ptemp->next;
          ptemp->next->pre =ptemp->pre;//!!!

         //b: insert
          ptemp->next = head->next;
          head->next->pre = ptemp;

          head->next = ptemp;
          ptemp->pre= head;



       return ptemp->value;
    }
    //插入一个元素
    void put(int key, int value) 
    {
        //01 如果关键字已经存在，则变更其数据值
        if (hash.count(key) ==1 )
        {    
            // '[[2],[2,1],[2,2],[2],[1,1],[4,1],[2]]
            //head-->2 --tail 
            Node* ptemp = hash[key];
            ptemp->value =value;

            //a: move
            ptemp->pre->next = ptemp->next;
            ptemp->next->pre =ptemp->pre;//!!!

            //b: insert
            ptemp->next = head->next;
            head->next->pre = ptemp;

            head->next = ptemp;
            ptemp->pre= head;
            return ;
        }
        //02 不能存在，就需要插入一个元素
     
        //a:创建一个元素
        
        Node* ptemp = new Node();
        ptemp->key = key;
        ptemp->value =value;

        //b hash
        hash[key] = ptemp;

        //c:insert 新元素放前面

         // head--> tail
         //     1 
         ptemp->next = head->next; //why !!!
         head->next->pre = ptemp;

         head->next = ptemp;
         ptemp->pre = head;

         if (hash.size() > size)
         {
           // 如果超出容量，删除双向链表的尾部节点.从最后面删除
           //head-1->2->tail 
           
           Node* plast = tail->pre;
           hash.erase(plast->key);
           //1-->tail
            
            plast->pre->next = tail;
            tail->pre = plast->pre;


         }

    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
// @lc code=end

