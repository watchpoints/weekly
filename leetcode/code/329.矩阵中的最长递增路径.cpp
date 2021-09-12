class Solution
{
public:
    int longestIncreasingPath1(vector<vector<int>> &matrix)
    {
        int path = 0;
        int rows = matrix.size();
        int cols = matrix[0].size();

        vector<vector<int>> dp(rows, vector<int>(cols, 1)); // 起点。

        vector<vector<int>> directions = {
            {0, 1},
            {1, 0},
            {0, -1},
            {-1, 0}

        };

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int temp = dfs(i, j, INT_MIN, matrix, dp, directions);

                path = max(temp, path);
            }
        }

        return path;
    }

    int dfs(int row, int col, int pre, vector<vector<int>> &matrix, vector<vector<int>> &dp, vector<vector<int>> &directions)
    {
        //edge

        if (row < 0 || row >= matrix.size() || col < 0 || col >= matrix[0].size())
        {
            return 0;
        }

        if (pre >= matrix[row][col])
        {
            return 0;
        }

        if (dp[row][col] > 1)
        {
            return dp[row][col];
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = row + directions[i][0];
            int ny = col + directions[i][1];

            int temp = dfs(nx, ny, matrix[row][col], matrix, dp, directions);

            dp[row][col] = max(dp[row][col], temp + 1);
        }

        return dp[row][col];
    }

    //longestIncreingPath
    int longestIncreasingPath(vector<vector<int>> &matrix)
    {
        //path where is begin ,where is end

        // root matrix[i][j]

        int rows = matrix.size();
        int cols = matrix[0].size();
        int longestPath = 0;
        //dp[i][j] =1;
        vecotr<vector<int>> dp(rows, vector<int>(cols, 1));

        vector<vector<int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        //death loop
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int temp = mydfs(i, j, INT_MIN, dp, matrix, directions);
                longestPath = max(longestPath, temp);
            }
        }

        return longestPath;
    }

    int mydfs(int i, int j, int pre, vector<vector<int>> &dp, vector<vector<int>> &matrix, vector<vector<int>> &directions)
    {
        //约束条件1
        if (i < 0 || i >= matrix.size() || j < 0 || j >= matrix[0].size())
        {
            return 0;
        }
        //约束条件2 3 <4
        if (pre >= matrix[i][j])
        {
            return 0;
        }

        //避免重复计算
        if (dp[i][j] > 1)
        {
            return dp[i][j];
        }

        //计算邻居
        int path = 0;
        for (int index = 0; index < 4; index++)
        {
            int ni = i + directions[index][0];
            int nj = j + directions[index][1];
            path = max(path, mydfs(ni, nj, matrix[i][j], dp, matrix, directions) + 1);
        }
        dp[i][j] = path;

        return dp[i][j];
    }
};