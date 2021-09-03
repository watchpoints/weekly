/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 翻转字符串里的单词
 */

// @lc code=start
class Solution {
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

       reverse(input.begin(),input.end());

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
                    continue; //不统计：
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
// @lc code=end

