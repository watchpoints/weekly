/*
 * @lc app=leetcode.cn id=207 lang=cpp
 *
 * [207] 课程表
 */

// @lc code=start
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {

    }
};
// @lc code=end


class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {

        //01 定义存储结构
        map<int,vector<int>> graph; 
        //hash+list key 是node value 邻接，这里也可以自定义key。
        //map key是const ,是无法被修改的。

        vector<int> degree(numCourses,0); //判断是否被访问过 degree ==0 才可以不在访问。

        //02 构造数据

        for(int i=0;i<prerequisites.size();i++)
        {
            int cur = prerequisites[i][1];
            int next = prerequisites[i][0];
            
            graph[cur].push_back(next);
            degree[next]++;
        }
        //03 从degree =0 的开始
        int count =0;
        queue<int> bfsQueue;
        
        for (int i=0;i < degree.size();i++)
        {
            if (0 == degree[i])
            {
                bfsQueue.push(i);//
                count++;

            }
        }

        //04 bfs
        while (!bfsQueue.empty())
        {
            int node = bfsQueue.front();
            bfsQueue.pop();

            vector<int> neights = graph[node];

            for(int i=0;i < neights.size();i++)
            {
                int next = neights[i];
                degree[next] --;
                if (degree[next] == 0)
                {
                    bfsQueue.push(next);
                    count++;
                }

            }

        }
        return  count == numCourses;

    }
};

