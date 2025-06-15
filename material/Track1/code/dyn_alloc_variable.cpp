#include <cstdio>

int main(int argc, char *argv[]) {
  int *p = new int;
  *p = 5;
  
  printf("Value stored in p: %d\n", *p);

  delete p;
  
  return 0;
}
