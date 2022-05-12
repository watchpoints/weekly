#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:
    // Solution1
    // data:
    // alg:
    // 01 什么情况才算 判断 s 为 t 的子序列？--->遍历s结束
    // 02 怎么比较：包含  不包含
    bool isSubsequence1(string s, string t)
    {
        return subsequence(s, t, 0, 0);
    }
    bool subsequence(string s, string t, int i, int j)
    {
        if (i == s.size())
        {
            return true;
        }
        if (j >= t.size())
        {
            return false;
        }
        if (s[i] == t[j])
        {
            return subsequence(s, t, i + 1, j + 1);
        }
        else
        {
            return subsequence(s, t, i, j + 1);
        }
    }
    bool isSubsequence2(string s, string t)
    {
        int i = 0;
        int j = 0;
        while (i < s.size() && j < t.size())
        {
            if (s[i] == t[j])
            {
                i++; //包含
            }// 02 怎么比较： 不包含 i保持不变。
            j++;
        }

        return i == s.size();    // 01 什么情况才算 判断 s 为 t 的子序列？--->遍历s结束
    }
    // Greedy algorithms never work! Use dynamic programming instead!
    // https://leetcode-cn.com/problems/is-subsequence/solution/dai-ma-sui-xiang-lu-dai-ni-xue-tou-dpzi-knntf/
    //确定遍历顺序
    bool isSubsequence(string s, string t)
    {
        // step01 data
        vector<vector<int> > dp(s.size() + 1, vector<int>(t.size() + 1, 0));
        // 1. s 为rows t为cols 2. dp[s.size()][t.size()]
        // dp[i][j] 表示以下标i-1为结尾的字符串s，和以下标j-1为结尾的字符串t，相同子序列的长度为dp[i][j]。
        // dp数组如何初始化

        // step2 from 小到大遍历
        // 4 确定遍历顺序
        for (int i = 1; i <= s.size(); i++)
        {
            for (int j = 1; j <= t.size(); j++)
            {
                // a
                //[a 0 b 1 c 2]
                // dp[1][1]
                if (s[i - 1] == t[j - 1])
                {
                    dp[i][j] =dp[i-1][j-1]+1; 
                }else
                {
                    dp[i][j] =dp[i][j-1];
                }
            }
        }

        return dp[s.size()][t.size()] ==s.size();//end
    }
    int findmin(string searchWord,string resultWord)
    {
        int sl = searchWord.size();
        int rl =resultWord.size();
        int i =0;
        int j =0;
        while(i<rl && j<sl)
        {
            if(resultWord[i] == searchWord[j])
            {
                i++;//inlcude
            }
            j++;
        }
        return sl -i; //392. 判断子序列  区别。
    }
};
int main()
{
    Solution test;

    cout << "out: =" << test.isSubsequence("abc", "ahbgdc") << endl;
     cout << "out: =" << test.findmin("Armaze", "Amazon") << endl;
    return 0;
}
/**

 输入：s = "axc", t = "ahbgdc"
 输出：false

输入：s = "abc", t = "ahbgdc"
输出：true

//https://leetcode.com/problems/number-of-matching-subsequences/
//https://leetcode-cn.com/problems/longest-common-subsequence/ss s s

给定字符串 s 和 t ，判断 s 是否为 t 的子序列。
字符串的一个子序列是原始字符串删除一些（也可以不删除）字符而不改变剩余字符相对位置形成的新字符串。（例如，"ace"是"abcde"的一个子序列，而"aec"不是）。
来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/is-subsequence
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 */