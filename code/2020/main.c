// main.c, from CS:APP
void multstore(long x, long y, long* dest);

int main() {
    long d;
    multstore(2, 3, &d);
    return 0;
}

long mult2(long x, long y) {
    long s = x * y;
    return s;
}
