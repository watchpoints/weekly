/*
 * @lc app=leetcode.cn id=79 lang=cpp
 *
 * [79] 单词搜索
 */
// @lc code=start
class Solution {
public:
     //深度优先搜索 暴力破解
    bool exist(vector<vector<char>>& board, string word) {
       
       //01 从board任意一个点开始，采用深度优先搜索。
       int rows = board.size();
       int cols = board[0].size();
       for(int i =0; i< rows;i++)
       {
          for(int j=0;j<cols;j++)
          {  
              if( dfs(i,j,0,word,board))
              {
                 return true;
              }

          }
       }
       return false;
    }

   bool dfs(int i,int j,int cur,string& word,vector<vector<char>>& board)
   {
      if (cur == word.size())
      {
          return true;
      }

      if ( i<0 ||j<0 || i>=board.size() || j>= board[0].size()  || board[i][j] != word[cur])
      {
          return false;
      }

      //最重要部分
      char temp =board[i][j];
      board[i][j] ='*';//死循环
      
      bool result = dfs(i, j - 1, cur + 1, word, board) || dfs(i, j + 1, cur + 1, word, board) || dfs(i - 1, j, cur + 1,word, board) || dfs(i + 1, j, cur + 1, word, board);

      board[i][j] =temp;

      return  result;
   }
};
// @lc code=end

