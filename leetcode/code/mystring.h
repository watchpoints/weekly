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
      cout<< "step1="<<input<<endl;
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
      
      cout<< "step2="<<input<<endl;
      //reverse(input.begin(),input.end());
      myreverse(input,0,input.size());

      cout<< "step2="<<input<<endl;
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


/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 翻转字符串里的单词
 */

// @lc code=start
class Solution12 {
public:
   ///核心思想： (A⁻¹B⁻¹) ⁻¹ = BA 
    string reverseWords(string s) {

      //青铜：你知道别人也知道的 信息

     //请尝试使用 O(1) 额外空间复杂度的原地解法
     //观察： s = "  hello world  "  s = "  hello world  "
     //核心思想： (A⁻¹B⁻¹) ⁻¹ = BA [后面单词移动前面，并且顺序不变] 
     // 字符串的旋转 旋转字符串
      
      //删除前后端空格，中间空格保留一个。

      string input =delSpace(s);
cout<< "step3="<<input<<endl;
      //two pointer 

      int start =0;//workd being index
      int end =0; //word end

      //s = "hello world"
      while (end <input.size())
      {
         char temp =input[end];//

         if (' ' == temp)
         {
             reverse(input.begin()+start,input.begin()+end);

             start =end+1;
         }
         //最后一个单词没有空格

         end++;
      }
       reverse(input.begin()+start,input.begin()+end);
       
       //(A⁻¹B⁻¹) ⁻¹ = BA
       cout<< "step3="<<input<<endl;
       reverse(input.begin(),input.end());
       cout<< "step3="<<input<<endl;
       return input;

    }

    string delSpace(string input)
    {
        //s = "  hello    world  "  
        //s = "hello world      "

        //two pointer
        int end =0;//new end index 
        int begin =0; //old string begin index
        char temp =0;
        int  spaceCount =0;

        while ( begin <input.size())
        {  
             temp =input[begin];
             
             if( ' ' == temp)
             {
                 if(0 == end)
                 {  
                     begin++;
                    continue; //不统计：个数
                 }else
                 {
                     spaceCount++;//统计：分割字符的空格
                 }
                 
             }else
             {
                 if(spaceCount >0)
                 {
                    input[end++] =' '; //分割字符的空格 保留

                    spaceCount = 0;// 判断字符串前排是否有空格 
                 }//else
                 //{
                input[end++] =input[begin];
                 //}
                 
             }
             begin++;

        }
        
    return input.substr(0,end);//[0,end)

    }
};



