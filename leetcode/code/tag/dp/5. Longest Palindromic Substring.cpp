#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    string longestPalindrome(string s) {
     //是 和 不是很清楚
      vector<vector<bool> > dp(s.size(),vector<bool>(s.size(),false));
      //dp[start][end]
    
       for(int end =0; end <s.size();end++ )
       {
           for(int start =0;start <=end;start++)
           {
               if()
               {
                   
               }
           }
       }

    }
};

int main()
{
    return 0;
}

