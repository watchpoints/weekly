#include <vector>
#include <iostream>
using namespace std;
//十六进制转字符串化为十进制
 int hexStringtoInt(string input){
   unsigned int num = 0x0;
  for(char item:input)
  {
      int bit = item >='a'?item-'a'+10:item -'0';
      num = num*16+bit; //2个大数乘法有可能越界。
  }
  return (int)num;
}

//十进制转整数串化为十六制转字符串
//
 string toHex(int num)
 {   
     if ( 0 == num) return "0";
     string output;
     unsigned int input = num; //自动完成转换 int -- unsigned int
     //(gdb) p /x -1
     //$16 = 0xffffffff
     //(gdb) p  0xffffffff
     //$18 = 4294967295
     //(gdb) p /x 15
     //$19 = 0xf
     string key="0123456789abcdef";
     int index =0;
     while(input >0 )
     {
        index = input & 0x0000f; // 自动完成转换
        output.push_back(key[index]);
        input = input>>4;
     }
     reverse(output.begin(),output.end());

     return output;
 }
 
 class Solution223 {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
       //step1 两个矩形覆盖的总面积 = 第一个矩形面积 + 第二个矩形面积 - 两个矩形重叠的面积
       int total =(ax2-ax1)*(ay2-ay1)+(bx2-bx1)*(by2-by1);

       //step2:
       //两个矩形重叠的面积 = 两个矩形重叠的长(两个矩形在X轴的投影重叠的长度) * 两个矩形重叠的宽(两个矩形在Y轴的投影重叠的长度)。
        int overlapLong = max(0, min(ax2, bx2) - max(ax1, bx1));
        int overlapWidth = max(0, min(ay2, by2) - max(ay1, by1));
        
        return total - overlapLong * overlapWidth;

    }
};


class Solution1353 {
public:
    int maxEvents(vector<vector<int> >& events) 
    {
        //sort(events.begin(),events.end()); 错误思路 按照开始时间排序 
        //因为第一天，第二天 第三天是时间有序的

        int count =0;  // 记录参见会议的次数
        // 小顶堆队列
        priority_queue<int,vector<int>,greater<int>>pq; //调度策略：最早结束时间

        int days =0; //最晚开始时间 一共有多少天

        unordered_map<int, vector<int>> day2days; 
        // 构建一个【开始天】 和 【结束天】的映射 1:多关系 图的
         for(vector<int>event:events)
         {
            day2days[event[0]].push_back(event[1]);//
            if (event[1] > days)
            {
                days = event[1];
            }
         }
        //错误思路：上来遍历了events
        //有很多会议 这样无法处理--->当天有很多会议。
        for(int i =0;i<days;i++)
        {

           //第i天 有1个会议 2个会议 三个会议 
           //一句结束 ，当天结束，后面结束
            //对堆
            // vector<int> day =day2days[i]; 不存在创建上来不能这样写。
            
             //存在的话
            if (day2days.find(i) != day2days.end())
            {
                vector<int> curdyays =day2days[i];
                for（int day:curdyays）
                {
                    pq.push(day);
                }
            }
       
           while(!qp.empty() && qp.top()< i)
           {
               qp.pop();  //过期删除
           }

           if(!qp.empty())
           {
               qp.pop();//删除 马上结束的。 
               count++;
           }

        }
        return count; 
    }
};

class Solution5 {
public:
    //time：o（n2）time o()
    //最长回文子串
    //思路：暴力遍历 time：o（n3),字串个数：（1+n）*n/2 判断回文 
    //利用动态规划 判断会变成o（1）
    //优化的回文的判断。
    //细节：[aa][aba]
    //j>i= j-1>=i+1 
    //优化：子串判断 a aa aba 
    // dp[start][end] = dp[start+1][end-1]; 
    // dp[start+1][end-1] = dp[start+2][end-2]; 
    // n=2时候，例如 aa 是无法依赖前面情况 初始值
    //从时间复杂度说思路
      
    //s[start] == s[end] 判断【start end】
    //s[start] == s[end] 【aa] 
    string longestPalindrome(string s) {
    int n= s.size();
    vector<vector<bool>> dp(n,vector<bool>(n,false)); //dp[i][j] =true;
    int start =0; //子串开始位置
    int end =0; //子串结束位置
    int low =0;
    int high =0;
    for(end =0;end <n;end++)
    {
        for(start =0;start <= end;start++)
        {    //aa aa
             if (s[start] == s[end])
             {    
                 
                 if(end -start <=2)
                 {
                     dp[start][end] = true;
                 }else{
                     dp[start][end] = dp[start+1][end-1];
                 }
             }

             if(dp[start][end] == true && (end -start > high -low ))
             { 
                 high = end;
                 low = start;
             }
        }
    }
    return s.substr(low,high-low+1);
    }
};
class Solution1044 {
public:
     // 重复子串
     //小王：判断字符串 开始和end 这就是重复自从这里理解吗？

    string longestDupSubstring(string s) {

    }
};


 class ParentTreeNode {
  public:
      int val;
      ParentTreeNode *parent, *left, *right;
       }；
 


class Solution474 {
public:
    /*
     * @param root: The root of the tree
     * @param A: node in the tree
     * @param B: node in the tree
     * @return: The lowest common ancestor of A and B
     */
    ParentTreeNode * lowestCommonAncestorII(ParentTreeNode * root, ParentTreeNode * A, ParentTreeNode * B) {
        // write your code here
        if (nullptr == root) return nullptr;

        if (root == A || root == B) return root;
        
        ParentTreeNode* left = lowestCommonAncestorII(root->left,A,B);
        ParentTreeNode* right = lowestCommonAncestorII(root->right,A,B);

        if (left && right )
        {
            return root;
        }

        return left ==nullptr?right:left;

    }
};
//g++ -std=c++11 4-24.cpp 
int main()
{   
    //64 0ffffffff
    cout <<"0xf="<<hexStringtoInt("f") <<endl;
    cout <<"-1="<<hexStringtoInt("ffffffff") <<endl;
    cout <<"-1="<<toHex(-1) <<endl;
    cout <<"-1="<<toHex(4294967295) <<endl;

}