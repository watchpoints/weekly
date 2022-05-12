class Solution
{
public:
    // 54. Spiral Matrix
    //旁白：1. 画图 2.想到出过程和细节，3. 然后写出来，4 然后朗读出来。
    //o(m*n)
    vector<int> spiralOrder(vector<vector<int>> &matrix)
    {
        int rows = matrix.size();
        int cols = matrix[0].size();
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        int direction = 0;
        vector<vector<int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int i = 0;
        int j = 0;
        int steps = rows * cols;
        vector<int> res;

        for (int index = 0; index < steps; index++)
        {   
            visited[i][j] = true;
            res.push_back(matrix[i][j]);
            //generate
            int ni = i + directions[direction][0];
            int nj = j + directions[direction][1];
            // check borders
            if (ni < 0 || ni >= rows || nj < 0 || nj >= cols || true == visited[ni][nj])
            {
                direction = (direction + 1) % 4;
                ni = i + directions[direction][0];
                nj = j + directions[direction][1];
            }
            i = ni;
            j = nj;
        }
          return res;
    }
  
};

//https://leetcode.com/problems/spiral-matrix-ii/