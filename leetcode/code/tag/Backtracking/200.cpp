//https://leetcode.com/problems/number-of-islands/
//Amazon | OA 2019 | Treasure Island II
#include<iostream>
#include<vector>

class Solution {
public:
    int numIslands(vector<vector<char> >& grid) {
        int count =0;
        int rows =grid.size();
        int cols =grid[0].size();

        for(int i =0;i<rows;i++)
        {   
            for(int j =0;j<cols;j++)
            {
                 //为什么每个点都需要判断呢
                 if（gid[i][j] =='1'）
                 {
                     dfs(grid,i,j);
                     count++;
                 }
                 
            }
        }
        return count;
    }

    void dfs(vector<char>>& grid, int i, int j))
    {
        int m = grid.size(), n = grid[0].size();
        if (i < 0 || i == m || j < 0 || j == n || grid[i][j] == '0') {
            return;
        }

        grid[i][j] = '0';
        eraseIslands(grid, i - 1, j);
        eraseIslands(grid, i + 1, j);
        eraseIslands(grid, i, j - 1);
        eraseIslands(grid, i, j + 1);

    }
};