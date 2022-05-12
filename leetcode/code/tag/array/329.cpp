#include<iostream>
#include<vector>

using namespace std;
class Solution {
public:
    int longestIncreasingPath(vector<vector<int> >& matrix) {
 
        int rows =matrix.size();
        int cols =matrix[0].size();
        int path =0;
        vector<vector<int> > dp(rows,vector<int>(cols,1)); //dp[i][j] 为开始开始节点的递增路径。root 
        for(int i =0;i<rows;i++)
        {
            //for(int j =0;i<cols;j++) 死循环了
            for(int j =0;j<cols;j++)
            {   
                int temp =dfs(matrix,i,j,-9999,dp);
                if(temp> path)
                {
                    path =temp;
                }
            }
        }

        return path;
    }
    //找出其中 最长递增路径 的长度。 -->path--连续大于
    int dfs(vector<vector<int> >& matrix,int row,int col,int last,vector<vector<int> >& dp)
    {
        if (row < 0 || row >=matrix.size() || col <0 || col >=matrix[0].size())
        {
            return 0;
        }
        
        if (matrix[row][col] <=last)
        {
            return 0;
        }

        if( dp[row][col] >1)
        {
            return dp[row][col] ;
        }

        int temp =0;
        temp =max(temp,dfs(matrix,row+1,col,matrix[row][col],dp)+1);
        temp =max(temp,dfs(matrix,row-1,col,matrix[row][col],dp)+1);
        temp =max(temp,dfs(matrix,row,col+1,matrix[row][col],dp)+1);
        temp =max(temp,dfs(matrix,row,col-1,matrix[row][col],dp)+1); //忘记+1了，不

        dp[row][col] =temp;
        
        return  dp[row][col] ;
    }

    int minSteps(vector<vector<int> >& matrix)
    {
        
    }
};

int main()
{
    vector<vector<char> >input = {{'O', 'O', 'O', 'O'},
                         {'D', 'O', 'D', 'O'},
                         {'O', 'O', 'O', 'O'},
                         {'X', 'D', 'D', 'O'}};
   Solution test;
    
}