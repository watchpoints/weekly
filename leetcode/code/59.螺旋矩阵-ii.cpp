/*
 * @lc app=leetcode.cn id=59 lang=cpp
 *
 * [59] 螺旋矩阵 II
 *
 * https://leetcode-cn.com/problems/spiral-matrix-ii/description/
 *
 * algorithms
 * Medium (78.44%)
 * Likes:    541
 * Dislikes: 0
 * Total Accepted:    141.2K
 * Total Submissions: 179.9K
 * Testcase Example:  '3'
 *
 * 给你一个正整数 n ，生成一个包含 1 到 n^2 所有元素，且元素按顺时针顺序螺旋排列的 n x n 正方形矩阵 matrix 。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：n = 3
 * 输出：[[1,2,3],[8,9,4],[7,6,5]]
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：n = 1
 * 输出：[[1]]
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 1 
 * 
 * 
 */

// @lc code=start
class Solution {
public:
    //time o(n) space o(n2)
    vector<vector<int>> generateMatrix(int n) {

        vector<vector<int>> path(n,vector<int>(n,0));
        int steps =n*n;
        //start 
        int x =0;
        int y =0;
        
        int direction =0;
        vector<vector<int>> directions ={{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        vector<vector<bool>> visited(n, vector<bool>(n,false));


        for(int i =1;i<=steps;i++)
        {
            path[x][y] =i;
            visited[x][y] =true;//why 

            //i++
            int nx =x+directions[direction][0];
            int ny =y+directions[direction][1];
             //i<=steps
            if (nx < 0 || nx >=n || ny <0  || ny >= n || true == visited[nx][ny] )
            {    
                 direction = (direction + 1) % 4;
                 nx =x+directions[direction][0];
                 ny =y+directions[direction][1];
            }

            x = nx;
            y = ny ;
        }

        return path;

    }
};
// @lc code=end

