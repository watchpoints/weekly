#include <iostream>
#include <vector>
using namespace std;
// 516. 最长回文子序列
/**
给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度。
子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列。
**/
//同类题目
// 5 最长回文子串
// 647.回文子串
/**
Input: s = "bbbab"
Output: 4
Explanation: One possible longest palindromic subsequence is "bbbb".
**/

//旁白：1. 画图 2.想到出过程和细节，3. 然后写出来，4 然后朗读出来。
// dp[][] 长度
//并返回该序列的长度。--->回文判断  Longest Palindromic Substring
//子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列 -->包含该字符 和不包含该字符2个情况。
//[stat end] 不相等 有也可以是回文。【
// start end-1】（(不包含end）
//[stat+1,end](不包含start)

//细节1 --- 删除某些字符或者不删除任何字符形 ---包含 不包含 -->
//细节2: 这里为什么不考虑同时不包含 start end。[start+1,end-1]属于历史记录 已经完成判断了。
//细节3:然后注意遍历顺序，i 从最后一个字符开始往前遍历，j 从 i + 1 开始往后遍历，这样可以保证每个子问题都已经算好了。
/**
 *
 * dp[i][j]: the longest palindromic subsequence's length of substring(i, j),
 * here i, j represent left, right indexes in the string
State transition:
dp[i][j] = dp[i+1][j-1] + 2 if s.charAt(i) == s.charAt(j)
otherwise, dp[i][j] = Math.max(dp[i+1][j], dp[i][j-1])
Initialization: dp[i][i] = 1
 *
 */
class Solution
{
public:
    //错误的思路，错误在哪里
    int longestPalindromeSubseq(string s)
    {
        int rows = s.size();
        vector<vector<int> > dp(rows, vector<int>(rows, 0)); // length
        for (int i = 0; i < rows; i++)
        {
            dp[i][i] = 1;
        }
        for (int end = 0; end < rows; end++)
        {
            for (int start = 0; start < end; start++)
            {
                // State transition
                if (s[start] == s[end])
                { // include [a] [aa] [aba]

                    if (end == start + 1)
                    {
                        dp[start][end] = 2;
                    }
                    else
                    {
                        dp[start][end] = dp[start + 1][end - 1] + 2; //  palindromic pre
                    }
                }
                else
                {
                    // bbba ,abbb  euqal
                    dp[start][end] = max(dp[start][end - 1], dp[start + 1][end]);
                }
            }
        }

        return dp[0][rows - 1]; // 516. Longest Palindromic Subsequence
    }
    int longestPalindromeSubseq2(string s)
    {
        int rows = s.size();
        vector<vector<int> > dp(rows, vector<int>(rows, 0)); // space: n*n //why bool -->jude palindromic
        
        for (int end = 0; end < rows; end++)
        {  
            //for:why  change form big to small
            for (int start = end; start >= 0; start--)
            {
                // State transition 回文判断  Longest Palindromic Substring
                if (s[start] == s[end])
                {   // include [a] [aa] [aba]
                    if(start == end)
                    {    
                         dp[start][end] = 1;
                    }else if (end == start + 1)
                    {
                        dp[start][end] = 2;
                    }
                    else
                    {
                        dp[start][end] = dp[start + 1][end - 1] + 2; //  palindromic pre  +2 why
                    }
                }
                else
                {
                      // State transition
                      //【abb】
                    dp[start][end] = max(dp[start][end - 1], dp[start + 1][end]);//why  not 
                }
            }
        }

        return dp[0][rows - 1]; // 516. Longest Palindromic Subsequence
    }
};
int main()
{
    // string input("bbbab");
    string input("cbbd");
    Solution test;
    cout << "input:"<<input<< " outout: " << test.longestPalindromeSubseq2(input) << endl;
}