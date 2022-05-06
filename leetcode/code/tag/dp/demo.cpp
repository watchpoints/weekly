#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:
    int countSubstrings(string s)
    {
        int count = 0;
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
        // dp[start][end] =false;

        for (int end = 0; end < s.size(); end++)
        {
            for (int start = 0; start <= end; start++)
            {
                if (s[start] == s[end])
                {
                    if(start == end || start +1 == end)
                    {
                        dp[start][end] =true;
                    }else
                    {
                         dp[start][end] = dp[start+1][end-1];// if 
                    }
                }
                if (true == dp[start][end])
                {
                    count++;
                }
            }
        }

        return count;
    }
};
int main()
{
    Solution temp;
    string input("aaaa");
    cout<< "11111";
    cout << "input = " << input << "output = " << temp.countSubstrings(input) << endl;
}

/*
给你一个字符串 s ，请你统计并返回这个字符串中 回文子串 的数目。
回文字符串 是正着读和倒过来读一样的字符串。
子字符串 是字符串中的由连续字符组成的一个序列。
具有不同开始位置或结束位置的子串，即使是由相同的字符组成，也会被视作不同的子串。

示例 1：
输入：s = "abc"
输出：3
解释：三个回文子串: "a", "b", "c"

示例 2：
输入：s = "aaa"
输出：6
解释：6个回文子串: "a", "a", "a", "aa", "aa", "aaa"

提示：
1 <= s.length <= 1000
s 由小写英文字母组成

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/palindromic-substrings
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/