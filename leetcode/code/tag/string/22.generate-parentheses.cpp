#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

class Solution
{
public:
    // Solution
    // 1. tree +path , Space Complexity o(n)
    // 2. dfs, Time Complexity:o(2^N) 1 <= n <= 8
    // 3. count jude  well-formed parentheses.
    vector<string> generateParenthesis(int n)
    {
        vector<string> res;
        string path;
        dfs(path, n, n, res);
        return res;
    }

    void dfs(string &path, int left, int right, vector<string> &res)
    {   
        if (left <0 || right < 0) return ; // if (left > 0)
        if (left == 0 && right == 0)
        {
            res.push_back(path);
            // paht="(("  left = 0,right= 2
        }

        if (left > right)
        {
            return; // not match ())
        }

        if (left > 0)
        {
            path.push_back('(');
            dfs(path, left - 1, right, res);
            path.pop_back();
        }

        if (right > 0)
        {
            path.push_back(')');
            dfs(path, left, right-1, res);
            path.pop_back();
        }

    }
};