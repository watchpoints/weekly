#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:
    // https://leetcode-cn.com/problems/is-subsequence/solution/dai-ma-sui-xiang-lu-dai-ni-xue-tou-dpzi-knntf/
    //思路：必须用动态规划，因为 双指针变量的方式 适合 我是你的子序列。已经很清楚了。
    int longestCommonSubsequence1(string text1, string text2)
    {
        int n1 = text1.size();
        int n2 = text2.size();
        vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));
        // 1.dp定义：dp[i][j] 表示以下标i-1为结尾的字符串text1，和以下标j-1为结尾的字符text1，长公共子序列列的长度为 dp[n1][n2] 返回结果
        // 2.dp初始化： 这里dp[i][0]和dp[0][j]是没有含义的，仅仅是为了给递推公式做前期铺垫，所以初始化为0。 dp[0][j] dp[j][0] 已经初始化完毕。

        // 3.确定遍历顺序：依赖状态改变  j --->j-1   i -->i-1
        for (int i = 1; i <= n1; i++)

        {
            for (int j = 1; j <= n2; j++)
            {
                // 4. 状态改变：
                if (text1[i - 1] == text2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1; //递归 计算
                }
                else
                {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]); //等价于
                }
            }
        }

        return dp[n1][n2];
    }
    
    int longestCommonSubsequence(string text1, string text2)
    {  //g++ -std=c++11 1143.longest-common-subsequence.cpp
        vector<vector<int> > dp(text1.size() + 1, vector<int>(text2.size() + 1, 0));
        //  dp[i][j] =max(dp[i-1][j],dp[i][j-1]); //等价于 i-1>=0
        //  dp[i][j] =dp[i-1][j-1] +1;  i-1 >=0;
        //三个状态变化

        for (int i = 1; i <= text1.size(); i++)
        {
            for (int j = 1; j <= text2.size(); j++)
            {   //dp[1][1] s1[0] s2[0]
                if(text1[i-1] == text2[j-1])
                {
                    dp[i][j] = dp[i-1][j-1]+1; //前面 一个元素走到当前元素 +1     //三个状态变化
                }else
                {
                    dp[i][j]  = max(dp[i-1][j],dp[i][j-1]);//前面 一个元素走到当前元素.不相等没有 这是等价于。     //三个状态变化
                }
            }
        }

        return dp[text1.size()][text2.size()];
    }

}
;
int main()
{
    Solution test;
    cout << "out: =" << test.longestCommonSubsequence("abcde", "ace") << endl;
    return 0;
}