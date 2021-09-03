#include <string>
using namespace std;
// 翻转单词顺序
/***
 * 第一个拦路虎：有空格怎么办？
 * 第二个拦路虎：都是字符怎么区分，前面一个字符，后面一个字符 【回到问题1】
 * 第三个拦路虎：翻转后的字符串中不应包含额外的空格
 * 
 *  前缀空格怎么办？这个需要删除
 *  中间多余的空格怎么办？
 * 
 *  
 *  https://leetcode-cn.com/problems/reverse-words-in-a-string/solution/tu-wen-bing-mao-cchao-xiang-xi-ti-jie-by-ary7/
 **/
class Solution1 {
public:
    string reverseWords(string s) {
      //1. 删除多余空格
      string input =delSpace(s);
      //翻转每个单词
      int begin =0; //单词开始位置
      int end =0;//单词结束位置
      //"a good example"
      while (end <input.size())
      {    
          //第一个拦路虎：有空格怎么办？
          //第二个拦路虎：都是字符怎么区分，前面一个字符，后面一个字符 【回到问题1】
          if(input[end] ==' ')
          {
             //前闭后开【begin,end)
            //reverse(input.begin()+begin,input.begin()+end); //[a]
            myreverse(input,begin,end);
             
             //reverses the order of the elements in the range [first,last).
             begin =end+1;  ////单词开始位置
          }   

          end++;
      }
      //[abc] 最后一个单词
      //reverse(input.begin()+begin,input.begin()+end);
      myreverse(input,begin,end);

      //reverse(input.begin(),input.end());
      myreverse(input,0,input.size());
      return input;
    }
    
    //input:"    Hello my    word!    "
    //outout:"Hello my word!"
    //数组特点：删除一个字符，整体都要移动。
    //如果做到移动一次呢。利用数组空间大小不变假设，然后双指针
    //原地构造一个新的字符串
    string delSpace(string input)
    {
        int end =0;// 新字符串结束位置。
        int count =0;
        for(char key :input)
        {
            if(key == ' ')
            {
                if(end ==0) //[   Hello]
                {
                    continue;
                }else
                {
                    count++;
                }
                //细节2:空格分为2个情况：中间空格 和首位空格
            }else
            {   
                if(count >0)
                {
                    input[end++] =' ';
                    count =0;
                } //最重要地方：这里不能else 

                input[end++] =key;
                
            }
            
        }
        return input.substr(0,end);
    }
    //https://leetcode-cn.com/problems/ti-huan-kong-ge-lcof/solution/mian-shi-ti-05-ti-huan-kong-ge-ji-jian-qing-xi-tu-/

    //输入：s = "We are happy."
   //输出："We%20are%20happy."

   string replaceSpace(string input)
   {
      //步骤01 计算新字符串长度 
       int n1 =input.size();
       int spaceCount =0;
       for(char key:input)
       {
           if(' ' == key)
           {
               spaceCount++;
           }
       }
       //空格1个字符串 %20三个字符 比原来多2个字符串

       input.resize(n1+2*spaceCount);

      //步骤02 
      //字符串特点：插入一个元素，回导致整体后移，覆盖后面的元素 怎么办呢？
      //倒叙遍历 构造新字符串 。假如是链表的 仿照stlclass insert_iterator;
      
      int oldIndex =n1-1;
      int newIndex =input.size()-1;
      //[We are happy.xxxx]
      //           |     |
      //    oldIndex  newIndex
      //"     "
      //while(oldIndex >0)
      //边界要相等这么简单 判断你忘记了 
      //【0 n);
      //最重要地方！！！！！！果然出错了 哈哈哈 
      while(oldIndex >=0) //循环条件是什么想清楚？？？？ 全部遍历一遍。【0，n）
      {    
          //最重要地方！！！！！！果然出错了 哈哈哈 
          if(input[oldIndex] == ' ')
          { 
              //%20
              input[newIndex--] = '0';
              input[newIndex--] = '2';
              input[newIndex] = '%';
              //这里放三个字符
          }else
          {   

             input[newIndex] =input[oldIndex];
          }
          //变化条件是什么？
          newIndex --;
          oldIndex --;
      }

      return input;
   }
   //前闭后开【begin,end)
   void myreverse(string &input,int begin,int end)
   {   
       if(begin>=end)
       {
           return ;
       }
       end --;
       //[abax]
       while (end >begin)
       {
           char temp =input[begin];
           input[begin++] =input[end];
           input[end--] =temp;

       }
       
   }

};
