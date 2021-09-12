#include<stdio.h>

typedef enum {Upward, Downward} Stack_Dir_t;

Stack_Dir_t detect_stack_dir(char * p) {
    char local = 0;
    if (p) {
        return &local > p ? Upward : Downward;
    } else {
        return detect_stack_dir(&local);
    }
}

int main() {
    if (detect_stack_dir(NULL) == Upward)
        printf("Stack Growth Upward!\n");
    else
        printf("Stack Growth Downward!\n");

    return 0;
}

int main1()
{   
int a;  
 int b;  
 printf("%d\n%d\n",&a,&b);   
 return 0;
}

