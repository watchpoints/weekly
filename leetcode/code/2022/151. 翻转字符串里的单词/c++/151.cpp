// https://leetcode.com/problems/reverse-words-in-a-string/
// 151. Reverse Words in a String
/*
Input: s = "a good   example"
Output: "example good a"
Explanation: You need to reduce multiple spaces between two words to a single space in the reversed string
解释：如果两个单词间有多余的空格，将翻转后单词间的空格减少到只含一个。

*/
class Solution
{
public:
    string reverseWords(string s)
    {
        //步骤1 删除前后缀空格（全部删除）和单词之间多余空格 保留一个空格(剩余一个)
        int end = 0;         //新字符串结束位置。原地去空格。
        int spacesCount = 0; //单词之间空格，非前后缀
        for (int i = 0; i < s.length(); i++)
        {
            if (' ' == s[i])
            {
                //思考：
                //有空格的就要删除。
                //但是中间的多个空格 还要保留一个怎么办？
                if (end > 0)
                {
                    spacesCount++;
                    //跳过前缀空格
                }
            }
            else
            {
                //思考:非空格保留。如何保留单词之间一个空格呢
                if (spacesCount > 0)
                {
                    s[end++] = ' ';
                    spacesCount = 0;
                    //后缀空格 因为结束，不被执行
                }
                s[end++] = s[i]; //这个函数执行条件是什么
            }
        }

        string input = s.substr(0, end);

        //步骤2：翻转单词
        int start = 0; //单词开始位置
        end = 0;   //单词结束位置

        for (end = 0; end < input.size(); end++)
        {
            //移动到空格就开始翻转,然后寻找下一个节点位置
            if (' ' == input[end])
            {
                reverse(input.begin() + start, input.begin() + end); //翻转条件是什么
                start = end + 1;
            }
        }

        //步骤3:最后一个单词
        reverse(input.begin() + start, input.begin() + end);  //条件不满足 还有剩余单词怎么办

        reverse(input.begin(), input.end());

        return input;
    }
};