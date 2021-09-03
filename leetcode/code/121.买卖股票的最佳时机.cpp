class Solution
{
public:
    int maxProfit(vector<int> &prices)
    {
        int maxProfit = 0;
        //f(end) =p[end]-p[begin] 
        //p[end] >p[begin] 
        
        //都发生变化，改为只有一端
        //p[end] 保持不变的，  p[begin]选择最小的一个值
        
        int begin = 0; //最低价格

        for (int end = 1; end < prices.size(); end++)
        {
            //这个地方思路很模糊，也是隐藏大boss地方

            if (prices[end] > prices[begin])
            {
                maxProfit = max(maxProfit, prices[end] - prices[begin]); //这个函数执行条件是什么
            }
            else
            {
                begin = end;
            }

            // end++;//细节呀，忘记end++ while --for
        }

        return maxProfit;
    }
};