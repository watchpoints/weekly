int add(int a, int b, int c, int d, int e, int f, int g, int h) { // 8 个参数
  int sum = a + b + c + d + e + f + g + h;  //相加求和
  return sum;
}
 
int main(void) {
  int i = 10;
  int j = 20;
  int k = i + j;
  int sum = add(11, 22,33, 44, 55, 66, 77, 88);
  int m = k; // 为了观察 %rax Caller Save 寄存器的恢复
 
  return 0;
}
