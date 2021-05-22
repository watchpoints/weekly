/*
 * @lc app=leetcode.cn id=332 lang=cpp
 *
 * [332] 重新安排行程
 */

// @lc code=start
class Solution {
public:
    //欧拉回路与欧拉路径之重新安排行程
    vector<string> findItinerary(vector<vector<string>>& tickets)
    {
        //01 定义辅助空间
        map<string, priority_queue<string, vector<string>>> graph;
        vector<string> path;

        //02 初始化邻居表
        for (auto& it : tickets) {
            graph[it]
        }
    }
};
class Solution1 {
public:
    unordered_map<string, priority_queue<string, vector<string>, std::greater<string>>> vec;

    vector<string> stk;

    void dfs(const string& curr)
    {
        while (vec[curr].size() > 0) {
            string tmp = vec[curr].top();
            vec[curr].pop();
            dfs(move(tmp));
        }
        //tree 的层次遍历 需要东西记录 是通过栈 和队里了记录的
        stk.emplace_back(curr);
    }

    vector<string> findItinerary(vector<vector<string>>& tickets)
    {
        for (auto& it : tickets) {
            vec[it[0]].emplace(it[1]);
        }

        dfs("JFK"); //我就是从这看i开始寻找

        //忘记这一样。这个不是思路重点
        reverse(stk.begin(), stk.end());
        return stk;
    }
};
//https://leetcode.com/problems/reconstruct-itinerary/discuss/78832/Short-C%2B%2B-DFS-iterative-44ms-solution-with-explanation.-No-recursive-calls-no-backtracking.

//https://leetcode-cn.com/problems/reconstruct-itinerary/solution/javadfsjie-fa-by-pwrliang/

//https://zhuanlan.zhihu.com/p/112013386
// @lc code=end
