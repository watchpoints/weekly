#include <iostream>

using namespace std;
/**
 * @brief Get the Number object
 * 
 * @param n 
 * @return int 
 * 

有这样一道智力题：

某商店规定：三个空汽水瓶可以换一瓶汽水。(至少三个,前提我换多个)
如果小张手上有n个空汽水瓶，最多可以换多少瓶汽水喝？

假如 ：小张手上有十个空汽水瓶，她最多可以换多少瓶汽水喝？
答案是5瓶，

方法如下：

先用9个空瓶子换3瓶汽水，喝掉3瓶满的，喝完以后4个空瓶子，

用3个再换一瓶，喝掉这瓶满的，

这时候剩2个空瓶子。然后你让老板先借给你一瓶汽水，喝掉这瓶满的，
喝完以后用3个空瓶子换一瓶满的还给老板

 */
int getNumber(int n)
{
    int leftEmpty =n; //空汽水瓶
    int total =0; //汽水瓶
    
    //三个空汽水瓶可以换一瓶汽水
    while(leftEmpty >=3 )
    {   
        total +=leftEmpty/3;
        //先用9个空瓶子换3瓶汽水，喝掉3瓶满的，喝完以后4个空瓶子
        leftEmpty =(leftEmpty/3+leftEmpty%3);     
    }
    //这时候剩2个空瓶子。然后你让老板先借给你一瓶汽水
    if(leftEmpty ==2 )
    {
        total++;
    }
    return total;
}
int main()
{   
    int n;
    while(cin>>n){
        cout <<getNumber(n)<<endl;
    }   
}


int getNumber1(int n)
{   
    //循环退出的条件 n
    if (n < 2)
    {
        return 0;//不够3瓶
    }
    if (n == 2)  
    {
        return 1;//借一换一
    }
    // 每次递归都是都是一次循环。变化的是什么 每次循环变化的是n
    return n/3+getNumber1(n/3+n%3);//从剩余空瓶继续计算
}



