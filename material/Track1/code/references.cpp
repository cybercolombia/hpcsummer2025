#include <cstdio>

int main(int argc, char *argv[]) {
  int a = 3;
  int &b = a;

  printf("Initially\na: %d, b: %d\n",a, b);

  b = 5;
  printf("After modifying b\na: %d, b: %d\n",a, b);
  
  return 0;
}
