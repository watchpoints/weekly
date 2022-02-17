#include <stdio.h>
#include <stdlib.h>

static const char const_data[16] = "I'm Const Data";

int main(int args,char ** argv)
{   
    // 注意这里用了跟上面一样的trick来修改
    char * pc = (char *)const_data;
    *pc = 'X';
    return 0;
}
//https://www.zhihu.com/question/23006995
//https://cedar-renjun.github.io/2015/10/31/elf-format-0/
