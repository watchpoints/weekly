/*
 * @lc app=leetcode.cn id=54 lang=cpp
 *
 * [54] 螺旋矩阵
 */
// @lc code=start
struct Point
{
    int x;
    int y;
    Point(int a, int b)
    {
        x = a;
        y = b;
    }
};

class Solution
{
public:
    //采取深度优先搜索：递归回溯
    //这里重点是如何回溯
    //因为路径只有一个，采取非递归实现
    //定义三个位置
    //当前位置，下一个位置，下个位置包括（错误需要回溯，正确位置）
    //约束条件：
    vector<int> spiralOrder(vector<vector<int>> &matrix)
    {
        //01 定义数据结构
        vector<int> path;

        int rows = matrix.size();
        if (0 == rows)
        {
            return path;
        }
        int cols = matrix[0].size();
        int total = rows * cols;
        path.resize(total);
        
        vector<vector<bool>> visited(rows, vector<bool>(cols, false)); //!!!


        //02 开始位置 0,0,结束位置 ,全部元素遍历一遍（不重复遍历）结束
        int x = 0;
        int y = 0;
        int position = 0; // 0 right  down 1 left 2 up 3

        for (int i = 0; i < total; i++)
        {
            //step01
            visited[x][y] = true;
            path[i] = matrix[x][y];

            //规律：一个节点 最多2个方向
            Point wrongNext = move(position, x, y);
            //!!!
            if (wrongNext.x < 0 || wrongNext.x >= rows || wrongNext.y < 0 || wrongNext.y <= cols || visited[x][y] == true)
            {
                position = (position + 1) % 4;
            }

            Point rightNext = move(position, x, y);
            x = rightNext.x;
            y = rightNext.y;
        }

        return path;

        //规律：一个坐标其实只有2个方向
        //一个是合法的，一个非法的。
    }

    //right ,down ,left ,up
    // static constexpr int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    Point move(int direction, int x, int y)
    {
        Point next(x, y);
        switch (direction)
        {
        case 0:
            /* code */
            next.y++;
            break;

        case 1:
            /* code */
            next.x++;
            break;
        case 2:
            /* code */
            next.y--;
            break;
        case 3:
            next.x--;
            break;
        }

        return next;
    }
};
// @lc code=end
