#include <string.h>
#include <stdio.h>

int main(void)
{
    char array[2] = {0};

    strcpy(array, "stackwilloverflow");

    return 0;
}
