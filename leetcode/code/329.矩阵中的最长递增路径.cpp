class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int path =0;
        int rows =matrix.size();
        int cols =matrix[0].size();

        vector<vector<int>> dp(rows,vector<int>(cols,1));// 起点。

        vector<vector<int>> directions ={
            {0,1},
            {1,0},
            {0,-1},
            {-1,0}

        };

        for(int i =0;i< rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                int temp =dfs(i,j,INT_MIN,matrix,dp,directions);

                path =max(temp,path);
            }
        }


        return path;

    }

    int dfs(int row,int col,int pre,vector<vector<int>>& matrix,vector<vector<int>> &dp,vector<vector<int>>& directions)
    {
        //edge

        if ( row < 0 || row >=matrix.size() || col <0 || col >=matrix[0].size())
        {
            return 0;
        }

        if (pre  >= matrix[row][col])
        {
            return 0;
        }

        if (dp[row][col] >1)
        {
            return dp[row][col];
        }

        for(int i =0;i<4;i++)
        {
            int nx =row+directions[i][0];
            int ny =col+directions[i][1];

            int temp =dfs(nx,ny,matrix[row][col],matrix,dp,directions);

            dp[row][col] =max(dp[row][col],temp+1);
        }

        return dp[row][col];
    }
};