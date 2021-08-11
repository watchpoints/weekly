
#include "mempool.h"

using namespace  std;
Mempool::Mempool(){

}


Mempool::~Mempool() {}

/**
* 申请 pool_num 个 pool_size 大小的内存， 这些内存块组成内存池
* 后续每次申请内存，都是从内存池中分配block_size 大小的内存，
* @param pool_num 申请大内存的个数
* @param pool_size 申请大内存的大小
* @param block_size 用户每一次申请的block 大小
* @return ： 0 表示成功， -1 表示失败
*/
int Mempool::init(int pool_num, int pool_size, int block_size){
    
    m_block_size= block_size;
    m_total = pool_size*pool_num;
    m_left =m_total;
    m_number =m_total/block_size; 

    Node* ptr = static_cast<Node*>(malloc(m_total));
    if (nullptr ==ptr)
    {
        return -1;
    }
    
    m_head.next =ptr;
    m_pcur =ptr;
    
     //防止内存碎片,这里按照block_size进行分割。
    for(int i =0;i<m_number-1;i++)
    {
        ptr[i].next =&ptr[i+1];
    }

    ptr[m_number-1].next =nullptr;
    
    return 0;  

}

char * Mempool::alloc(size_t  size)
{    
     if(m_left == 0) 
     {   
         //这里没有扩容，直接报错
         return nullptr ;
     }
    if (size >m_block_size)
    {
        size =m_block_size; // block_size 用户每一次申请的block 大小
    }

    //第一遍分配顺序分配
    Node * ptr =nullptr;
    if (m_pcur)
    {   
        ptr = m_pcur;
        m_used[ptr] = true;
        m_pcur =m_pcur->next;
    }else
    {   
        //还有剩余情况 寻找释放过的位置
        Node * ptemp=m_head.next;
        while (ptemp)
        {   
            if(m_used[ptemp] ==false)
            {   
                ptr = ptemp;
                m_used[ptemp] = true;
                break;
            }
            ptemp =ptemp->next;
        }
    }

    m_left = m_left-m_block_size;
    cout<< "Mempool::alloc():=" <<ptr << " total:=" <<m_total << " left:=" << m_left <<std::endl;

    return (char*)ptr;
}
void Mempool::cleanup(){
    
    m_number =0 ;
    m_total =0;
    m_left =0;
    m_block_size =0;
    m_used.clear();
    Node* ptr =m_head.next;
    Node* ppre =nullptr;
    free((char*)m_head.next);

}

//这里是分配指定序号的 block
char * Mempool::alloc(const char* tag){
   
    Node* ptr =(Node*) tag;
    if (ptr)
    {   
        if( m_used[ptr] ==false)
        {
            m_used[ptr] =true;
            m_left = m_left-m_block_size;
        }
    }
    return (char*)ptr;
    
}

void Mempool::free(char * pointer){
     
    if (nullptr == pointer)
    {
        return ;
    }
    Node* ptr =reinterpret_cast<Node*>(pointer);
    
    if (m_used[ptr] == true)
    {
        m_left += m_block_size;
        m_used[ptr] =false;
        cout<< "Mempool::free() " <<ptr << " total:=" <<m_total << " left:=" << m_left <<std::endl;
    }

}

/**
* 显示当前内存池的状态， 这些状态包含
* 1. 总内存多少
* 2. 已经使用了多少内存， 还剩下多少没有被使用的内存
* 3. 列出每个在使用中内存的tag
* @return
*/
const std::string Mempool::to_string()
{
    string ret;

    cout<< "total " <<m_total << " left:=" << m_left <<std::endl;
    
    cout<< "tag=:"  <<std::endl;
    
    //遍历
    map<Node*,bool>::iterator iter; 
    for(iter = m_used.begin(); iter != m_used.end(); iter++)  
    {
       cout<<"tag = "<< iter->first<<" used = "<<iter->second<<endl; 
    }
    

    return ret;

}

//测试类

class TestMempool
{  
public:
    TestMempool(int i)
    {   
        m_data =i;
        //cout << "TestMempool() is ok " <<i << endl;
        
    }
  static void* operator new(size_t size)
  {     
      //cout << "TestMempool is newed  by gAlloc.alloc " << size << endl;
      return gAlloc.alloc(size);
  }

  static void operator delete(void * pointer)
  {    
      //cout << " TestMempool is free  gAlloc.free:" <<pointer << endl;
      char* ptr= static_cast<char*>(pointer);
      return gAlloc.free(ptr);
  }

private:
 int m_data;

public:
 static Mempool gAlloc;

};
Mempool TestMempool::gAlloc ;


//测试入口
int main(int argc, char **argv)
{   

    TestMempool::gAlloc.init(10,80,sizeof(TestMempool)); //申请10个大块内存，每个10个大块内存容纳 TestMempool类

    TestMempool* pf[10];

    for(int i =0;i<10;i++)
    {
       pf[i] =new TestMempool(i);
    }
    
    for(int i =0;i<9;i++)
    {
       delete pf[i];
    }

    TestMempool::gAlloc.to_string();
    TestMempool::gAlloc.cleanup();
  
}
