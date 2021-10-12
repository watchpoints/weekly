/*
 * @lc app=leetcode.cn id=54 lang=cpp
 *
 * [54] 螺旋矩阵
 */
// @lc code=start

class Solution1 {
public:
    //模型：1节点和任何节点都有关系，并且顺时针 因此这是有向图
    vector<int> spiralOrder1(vector<vector<int>>& matrix)
    {
        //1 定义图的数据结构
        //顺时针 虽然是4个方向，但是只有2个方向移动，其中一个合法的。
        int direction = 0;
        vector<vector<int>> directions = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

        //约束条件:障碍物 没有可访问的节点, 边界
        int rows = matrix.size();
        int cols = matrix.size();
        int total = rows * cols;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

        vecotr<int> path(total);

        // 02 遍历有向图 开始位置(0,0)
        int x = 0;
        int y = 0;
        //邻居节点
        int nx = 0;
        int ny = 0;

        //03 循环迭代(x,y)
        // 根据这个题目你可以看出，矩阵特点 跟具体数字没关系，跟位置有关系，变化的是位置

        for (int index = 0; index < total; index++) {

            //enter
            visited[x][y] = true;
            path[index] = matrix[x][y];

            //寻找下一个邻居节点，
            //链表 tree无法获取下一个节点怎么遍历，图也是如此

            nx = x + directions[direction][0];
            ny = y + directions[direction][1];
            //(0,2) -->(0,3) 不合法
            //(1,0) --->(0,0) 被访问过一次
            //(1,0)--->(1,1) OK
            //思考与行动： 外圈约束条件是边界，内圈约束是什么，这个边界不好计算 !!!!
            //围墙在第一圈完毕后才会出现
            if (nx < 0 || nx >= rows || ny < 0 || ny >= cosl || visited[nx][ny] == ture) {

                direction = (direction + 1) % 4;
            }

            x = x + directions[direction][0];
            y = y + directions[direction][1];
            //这里需要重重新计算一次，这个函数调动浪费是值得的，不靠重复这样写就不妙。

        } //手工对着case1进行模拟一遍

        return path;
    }







    vector<int> spiralOrder(vector<vector<int>>& matrix) {
      
      //思路：
      //难点1 返回矩阵中的所有元 --遍历 --递归回溯（非2个for循环）
      //从（0,0）到（？，？）只有一个路径

      //难点2 数组链表都是线性结构，图 非线性结果 ，需要计算邻居节点
      //通过遍历 邻接表法

      int rows =matrix.size();
      int cols =matrix[0].size();
      int total = rows*cols;
      vector<int> path(total);

      vector<vector<bool>> visited(rows,vector<bool>(cols,false));
      //visited[x][y] =false

      int direction = 0;
      //vector<vector<int>> directions={{1,0},{0,1},{-1,0},{0,-1}};

      int x =0;
      int y =0;
      int nx =0;
      int ny =0;
      

      // length of path (x,y)--(x,y)
      for(int i=0;i<total;i++)
      {
          visited[x][y] =true;
          path[i] =matrix[x][y];

          nx =x+directions[direction][0];
          ny =y+directions[direction][1];
          
          //for 循环版本递归回溯
          if(nx < 0 || nx >=rows || ny <0 || ny >=cols || visited[nx][ny] == true)
          {
              direction =(direction+1)%4;
              //约束： 重复访问怎么办？
          }

          x =x+directions[direction][0];
          y =y+directions[direction][1];
      }

      return path;
    }
};


class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
      
      vector<int> output;
      int rows =matrix.size();
      if(0 == rows)
      {
          return output;
      }
      int cols =matrix[0].size(); //数据结构：图 
      int total =rows*cols;
      output.resize(output);

      //螺旋遍历图的条件
      vector<vector<bool>> visited(rows,vector<bool>(cosl,false)); //是否重复访问
      int direction =0;
      
      //顺指针
      vector<vector<int>>

    }
};
// @lc code=end


