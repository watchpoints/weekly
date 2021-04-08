class Solution
{
 private:
     vector<vector<int>> direction = {{1,  0}, {-1, 0}, {0,  1}, {0,  -1}};
public:
    //岛屿的数量 --必然要遍历-->遍历要求不重复不遗留
    int numIslands(vector<vector<char>> &grid)
    {
        if (grid.size() ==0 ||grid[0].size() ==0 )
        {
            return 0;
        }
        //01 定义数据结构
        int rows = grid.size();
        int cols = grid[0].size();
        //0 障碍物 1 可以通行  3 已经访问过
        vector<vector<int>> visited(rows, vector<int>(cols, 0));
        //不破坏原来结构，假如该数据别其他业务访问

        //02 遍历统计1次个数。
        int total = 0;
        // 图中存在孤立节点，dfs遍历 需要每个节点都判断一下。
        // 同类题目： 课程表统计。
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (grid[i][j] == '1' && visited[i][j] == 0 )
                {
                    total++;
                    dfs(i, j, grid, visited);
                }
            }
        }
        return total;
    }

    void dfs(int i, int j, vector<vector<char>> &grid, vector<vector<int>> &visited)
    {
        //约束条件：
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size())
        {
            return; //这个大家都想到，不然程序crash呀
        }
        //https://leetcode-cn.com/problems/number-of-islands/solution/dao-yu-lei-wen-ti-de-tong-yong-jie-fa-dfs-bian-li-/
        //想关联的 1只统计一次
        if (visited[i][j] == 1|| grid[i][j] == '0')
        {   
            visited[i][j] = 1;
            return; //
        }

        visited[i][j] = 1;

        for (int index = 0; index < 4; index++)
        {
            //int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            int nextx = i + direction[index][0];
            int nexty = j + direction[index][1];
            dfs(nextx, nexty, grid, visited);
            //why  visited
        }
     }
    };