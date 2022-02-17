#include <iostream>
using namespace std;
//memcached.h文件
#define HASHPOWER_DEFAULT 16
 
//assoc.h文件
unsigned int hashpower = HASHPOWER_DEFAULT;
 /* how many powers of 2's worth of buckets we use */
//hashsize(n)为2的幂，所以hashmask的值的二进制形式就是后面全为1的数。
//这就很像位操作里面的 & 
//value & hashmask(n)的结果肯定是比hashsize(n)小的一个数字.
//即结果在hash表里面
//hashmask(n)也可以称为哈希掩码

//所以memcached利用哈希表的长度是2的幂的性质，采用位操作进行优化，即: hv & hashmask(hashpower)

#define hashsize(n) ((unsigned long int)1<<(n))
#define hashmask(n) (hashsize(n)-1)
int main()
{
    for(int i=0;i<1000;i++)
    {  
       //65535
      cout<< hashmask(hashpower) << ":"<<(i & hashmask(hashpower)) << endl;
    }
}