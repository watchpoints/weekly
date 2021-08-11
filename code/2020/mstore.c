// gcc -Og -S mstore.c [-fno-asynchronous-unwind-tables]
// gcc -Og -c mstore.c ; objdump -d mstore.c

long mult2(long, long);

void multstore(long x, long y, long *dest) {
  long t = mult2(x, y);
  *dest = t;
}
