#include<iostream>
#include<vector>
using namespace std;
class Solution {
public:
    vector<vector<int> > imageSmoother(vector<vector<int> >& img) {
          vector<vector<int> > res;
          return res;
    }

    int getMaxGreyness(vector<vector<int> > &input)
    {
        int count =0;
        int rows = input.size();
        int cols = input[0].size();
        vector<int> myrows(rows,0); 
        vector<int> mycols(cols,0);

        for(int i =0;i<rows;i++)
        {
            for(int j =0;j<cols;j++)
            {
                if(1 ==input[i][j] )
                {
                    myrows[i]+=1;
                }else
                {
                    myrows[i]-=1;
                }
            }
        }
        for(int j =0;j<cols;j++)
        {
            for(int i =0;i<rows;i++)
            {
                if(1 ==input[i][j] )
                {
                    mycols[j]+=1;
                }else
                {
                    mycols[j]-=1;
                }
            }
        }

        for(int i =0;i<rows;i++)
        {
            for(int j =0;j<cols;j++)
            {

                count =max(count,myrows[i] +mycols[i]);
            }
           
        }

        return count;
    }
};