#include <cstdio>

int main(int argc, char *argv[]) {
  int a = 3;
  int *p = &a;
  printf("Value of 'a': %d\nMemory address stored in 'p': %p\n", a, p);
  
  return 0;
}
