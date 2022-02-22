#include <stdlib.h>
 
void create_destory(unsigned int size) {
    void *p = malloc(size);
    free(p);
}
 
int main(void) {
    const int loop = 4;
    char* a[loop];
    unsigned int kilo = 1024;
 
    for (int i = 0; i < loop; i++) {
        const unsigned int create_destory_size = 100 * kilo;
        create_destory(create_destory_size);
    }
 
    return 0;
}
