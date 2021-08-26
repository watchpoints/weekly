/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 翻转字符串里的单词
 */

// @lc code=start
class Solution {
public:
    //字符串特点：删除一个位置，整体迁移，
    //因此利用双指针重新构造一个新字符串

    //遇到空格 我就知道left单词结束位置，right单词开始位置
    //最后一个单词没有空格

    string reverseWords(string s) {

        //翻转后单词间应当仅用一个空格分隔
        //翻转后的字符串中不应包含额外的空格
        //步骤1 去掉多余空格 ：2边的空格去掉，中间空格保留一个
        // s = "  hello        world  "
        // s = "hello world"
        //字符串特点：删除一个位置，整体迁移，
        //因此利用双指针重新构造一个新字符串
        string input = delSpace();
            //步骤2：翻转每个单词:[begin,end)
            //the sky is blue
            //begin=0 end=3
            //begin=4 end=9
            //begin=10,end=14 【后面没有空格了】
            //寻找每个单词开始位置结束位置
            //遇到空格 我就知道left单词结束位置，right单词开始位置
            //最后一个单词没有空格
         int begin =0;
         int end =0;

         while (end <input.size())
         {
             //这里也分为2个情况
             if (' ' == input[end])
             {
                //这里情况情况相反，遇到null 一个单词结束 需要翻转

                reverse(input.begin() + begin, input.begin() + end);
                begin =end+1;//why not check 
             }

             end++;
         }
         //[abc]
         reverse(input.begin() + begin, input.begin() + end);

         reverse(input.begin(), input.end());

         //翻转整个句子
         return s;
    }
    //遇到空格（2个情况） 和字符串怎么办？
    string delSpace(string input)
    {
        int end = 0; //重新创新新单词的解释位置
        int spaceCount =0;
         //s = "  hello        world  "
        // s = "hello world|     world   "
        for(char key:input)
        {
            if (' ' == key)
            {
              
                if (0 == end) 
                {
                    continue; // //case1 前缀 替换，不保留
                }else
                {
                    spaceCount++; //case2  多个空格保留一个位置
                }
            }else
            {  //case3  单词之前有空格
                if (spaceCount >0)
                {
                    input[end++] =' ';
                    spaceCount =0;
                }

                input[end++] = key;
            }
        }

        return input.substr(0, end);
    }
};
// @lc code=end

