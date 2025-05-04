#include <cstdio>

void foo(int a) {
  a++;
  printf("a: %d\n",a);
}

int main(int argc, char *argv[]) {
  int b = 3;
  foo(b);
  printf("b: %d\n",b);
  
  return 0;
}
