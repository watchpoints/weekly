#include<iostream>
#include<vector>

using namespace std;

int getMaxGreyness(vector<vector<int> > &input)
{
    int rows =input.size();
    int cols =input[0].size();
    
    vector<int> myrows(rows,0);
    vector<int> mycols(cols,0);

    int res =0 ;

    for(int i =0;i<rows;i++)
    {
        for(int j =0; j<cols;j++)
        {
            if(input[i][j] == 1)
            {
                myrows[i] +=1;
            }else
            {
                myrows[i] -=1;
            }
        }
    }

    for(int j =0;j<cols;j++)
    {
        for(int i =0; i<rows;i++)
        {
            if(input[i][j] == 1)
            {
                mycols[j] +=1;
            }else
            {
                mycols[j] -=1;
            }
        }
    }

    for(int i =0;i <rows;i++)
    {
        for(int j =0;j<cols;j++)
        {
            int temp =myrows[i]+mycols[j];
            if (temp >res)
            {
                res = temp;
            }
        }
    }

    return res;
}
int main()
{
    vector<vector<int> > input={{1,0,1,},{0,0,1},{1,1,0}};
    cout<< "out= " <<getMaxGreyness(input);

}