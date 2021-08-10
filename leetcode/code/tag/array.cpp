//01 堆排序
//如何在O(n)的时间复杂度内查找一个无序数组中的第K个大元素？
//https://www.cnblogs.com/evenleee/p/11164019.html
#include<iostream>
#include<vector>
using namespace  std;
//什么情况下快速排序变成线程o(n)
//https://www.jianshu.com/p/716c363841b7
//https://blog.csdn.net/Qgwperfect/article/details/89881778
//https://blog.csdn.net/pathfinder1987/article/details/80717575?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_title~default-0.control&spm=1001.2101.3001.4242

int partition(vector<int> &input, int low, int high);
//findKthLargest
int KthElement(vector<int> &input, int low, int hight, int k)
{
    cout << low << ":" << hight << " K= " <<k <<endl;
    
    if (low > hight)
    {
        return -1;
    } //递归必要条件

    int index = partition(input,low,hight);
    cout << " index= " << index << endl; // index= 27271184 why????
    if (k == index )
    {
        return input[index];
    }
    //舍弃left，left 不需要排序，
    if (k > index)
    {
        return KthElement(input, index + 1, hight, k);
    }else
    {
        return KthElement(input, low, index - 1, k); // 这是一个递归调用  不是while循环处理，在这里被失败过一次。
    }
}
/** 如何在O(n)的时间复杂度内查找一个无序数组中的第K个大元素？
 *  第一次分区查找，我们需要对大小为 n 的数组执行分区操作，需要遍历 n 个元素。
 * 
 * 第二次分区查找，我们只需要对大小为 n/2 的数组执行分区操作，需要遍历 n/2 个元素。
 * 
 * 依次类推，分区遍历元素的个数分别为、n/2、n/4、n/8、n/16.
 * ……直到区间缩小为 1。
 *  如果我们把每次分区遍历的元素个数加起来，就是：n+n/2+n/4+n/8+…+1。这是一个等比数列求和，
 * 
 * 最后的和等于 2n-1。所以，上述解决思路的时间复杂度就为 O(n)。
https://zhuanlan.zhihu.com/p/102290441
 */
//从小到大排序
int partition(vector<int> &input, int low, int high)
{    //[1 ,2  3,  4, 5 ]
     // 5 4  3 2 1 
    int pivot = input[low];

    while (high > low)
    {
        // 因为默认基准是从左边开始，所以从右边开始比较
        // 当队尾的元素大于等于基准数据 时,就一直向前挪动 high 指针
        while (high > low && input[high] >= pivot) // 从右向左找第一个小于x的数
        {
            high--;
        }
        if (high > low)
        {
            //swap(input[low++], input[high]); 
            input[low++] = input[high];
            //low 不清楚 与pivot大小，因此需要high在比较一次
        }

        while(high > low && input[low] < pivot)
        {
            low++;
        }
        if (high > low)
        {
            //替换掉最右元素(已在最左元素中有备份）
            //最左元素一定被覆盖过，若没有，则表明右侧所有元素都>x，那么算法将终止
           // swap(input[high--], input[low]); //low 不清楚 与pivot大小，因此需要high在比较一次
            input[high--] = input[low];
        }
    }

    input[low] = pivot;

    return low; //一个函数 忘记返回值，结果返回一个很大数值。然后去访问数组，这个肯定是越界。
}


int main()
{

    vector<int>  case1 ={1,2,3,4,5,6,7,8,9,10};
    int k =3;

    cout << " top k is = " << 8 << " KthElement:" << KthElement(case1, 0, case1.size() - 1, case1.size()-k) << endl;

    vector<int> case2 = {10, 6, 3, 8, 5, 2, 7, 4, 9, 1};

    cout << " top k is = " << 8 << " KthElement:" << KthElement(case1, 0, case1.size() - 1, case1.size() - k) << endl;

}
/**
 *  Segmentation fault 
 *   
    [root@VM-0-10-centos demo]# ./a.out
    0:9K= 7
    index= 27271184
    0:27271183K= 7
    Segmentation fault

 **/