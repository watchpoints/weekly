/*
 * @lc app=leetcode.cn id=329 lang=cpp
 *
 * [329] 矩阵中的最长递增路径
 *
 * https://leetcode-cn.com/problems/longest-increasing-path-in-a-matrix/description/
 *
 * algorithms
 * Hard (38.92%)
 * Likes:    88
 * Dislikes: 0
 * Total Accepted:    6.5K
 * Total Submissions: 16.9K
 * Testcase Example:  '[[9,9,4],[6,6,8],[2,1,1]]'
 *
 * 给定一个整数矩阵，找出最长递增路径的长度。
 *
 * 对于每个单元格，你可以往上，下，左，右四个方向移动。
 *  你不能在对角线方向上移动或移动到边界外（即不允许环绕）。

 
 * 
 * 示例 1:
 *
 * 输入: nums = 
 * [
 * ⁠ [9,9,4],
 * ⁠ [6,6,8],
 * ⁠ [2,1,1]
 * ]
 * 输出: 4 
 * 解释: 最长递增路径为 [1, 2, 6, 9]。
 *
 * 示例 2:
 *
 * 输入: nums = 
 * [
 * ⁠ [3,4,5],
 * ⁠ [3,2,6],
 * ⁠ [2,2,1]
 * ]
 * 输出: 4 
 * 解释: 最长递增路径是 [3, 4, 5, 6]。注意不允许在对角线方向上移动。
 *
 *
 *
 * 题目理解：
 * 约束条件：递增路径
 * 步骤：回溯递归
 *
 *
 */

// @lc code=start

class Solution2 {
public:
    //[329] 矩阵中的最长递增路径
    int longestIncreasingPath(vector<vector<int>>& matrix)
    {
        int path = 0; //最长递增路径的长度
        int rows = matrix.size();
        if (rows == 0) {
            return 0;
            // testcase [] core
            //if the requested position is out of range by throwing an out_of_range exception
        }
        int cols = matrix[0].size();

        vector<vector<int>> dp(rows, vector(cols, 1)); //减少回溯递归次数，记录<i,j>为开始节点最长递增路径的长度.

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                //dfs遍历每一个节点<i,j>。类比N查树
                int temp = dfs(matrix, dp, i, j, INT_MIN);
                path = max(path, temp);
            }
        }
        return path;
    }

    int dfs(vector<vector<int>>& matrix, vector<vector<int>>& dp, int row, int col, int pre)
    {
        //叶子节点
        if (row < 0 || col < 0 || row >= matrix.size() || col >= matrix[0].size()) {
            return 0; // 最简单的case
        }
        // 8-->4
        if (pre >= matrix[row][col]) {
            return 0; //不是递增了，肯定是0
        }
        //dp[row][col] 默认为1
        if (dp[row][col] > 1) {
            return dp[row][col]; //避免重复计算
        }

        //为什么可以 递归调用？结构重复
        int cur = matrix[row][col];
        int left = dfs(matrix, dp, row, col - 1, cur);
        int right = dfs(matrix, dp, row, col + 1, cur);
        int up = dfs(matrix, dp, row - 1, col, cur);
        int down = dfs(matrix, dp, row + 1, col, cur);

        dp[row][col] = max(max(left, right), max(up, down)) + 1;
        return dp[row][col];
    }
};

class Solution1 {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix)
    {
        int row = matrix.size();
        if (0 == row)
            return 0;
        int col = matrix[0].size();

        vector<vector<int>> dp(row, vector(col, 1)); //记录从每个点开始最长的路径
        int path = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                //任意坐标开始都有可能
                int temp = dfs(matrix, dp, i, j, INT_MIN);
                path = max(temp, path);
            }
        }

        return path;
    }
    // row ，cols 开始坐标位置
    int dfs(vector<vector<int>>& matrix, vector<vector<int>>& dp, int row,
        int col, int pre)
    {
        if (row < 0 || col < 0 || row >= matrix.size() || col >= matrix[0].size()) {
            return 0; // 最简单的case
        }
        if (pre >= matrix[row][col]) {
            return 0; //不是递增了，肯定是0
        }

        if (dp[row][col] > 1) {
            return dp[row][col]; //避免重复计算
        }

        int left = dfs(matrix, dp, row, col - 1, matrix[row][col]);
        int right = dfs(matrix, dp, row, col + 1, matrix[row][col]);
        int up = dfs(matrix, dp, row - 1, col, matrix[row][col]);
        int down = dfs(matrix, dp, row + 1, col, matrix[row][col]);

        dp[row][col] = max(max(left, right), max(up, down)) + 1; //变化地方
        return dp[row][col];
    }
};
/**
[329] 矩阵中的最长递增路径
**/
class Solution {
private:
    //step1 定义数据结构
    int m_rows;
    int m_cols;
    vector<vector<int>> visited;
    //01 避免重复计算,记录每个node的最长递增路径的长度
    //02 0 标记为访问 1标记长度是 >1 长度是大于1

    vector<vector<int>> direction = {
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
        { -1, 0 }
    };
    //对于每个单元格，你可以往上，下，左，右四个方向移动
    //图的遍历：考虑存在环问题，但是这里不存在环。递增路
    //依然需要visited减少递归回溯
    //顺时针选择

    vector<vector<int>> input; // 对vecotr使用指针,我这里不会 &&语法报错

public:
    int longestIncreasingPath(vector<vector<int>>& matrix)
    {
        //step2 初始化数据结构
        m_rows = matrix.size();
        m_cols = matrix[0].size();
        visited.resize(m_rows, vecotr<int>(m_cols, 0)); //path
        input = matrix; //值拷贝
        int path = 0;

        //step3 一边遍历边一边计算路径。
        //顺时针遍历四个方向，符合dfs遍历特点
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                path = max(path, dfs(i, j, INT_MIN));
            }
        } //哈哈哈 就是图的遍历 1 vs 多

        return path;
    };
    //找出(x,y)最长递增路径的长度
    int dfs(int x, int y, int pre)
    {
        //顺时针选择回溯三个条件：
        if (x < 0 || x > = m_rows || y < 0 || y >= m_cols)
            return 0; // 越界

        if (pre >= input[x][y])
            return 0; //障碍物

        if (visited[x][y] > 0)
            return visited; // 重复访问,这里是对图的每个节点进行遍历，非环的重复

        int path = 0;

        //根据当前节点，获取邻居节点 ！！！，这里有四个
        for (int i = 0; i < 4; i++) {
            int nx = x + direction[i][0];
            int ny = y + direction[i][1];
            int next = dfs(nx, ny, input[x][y]) + 1;
            path = max(path, next); //四选一
        }

        visited[x][y] = path;
        //visited 不是解决环问题，因此没有开始标记为1
        // 解决是重复计算：遍历 每个节点至少访问 4*n次。

        reurn path;
    }

    // @lc code=end
