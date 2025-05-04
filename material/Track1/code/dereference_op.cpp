#include <cstdio>

int main(int argc, char *argv[]) {
  int a = 3;
  int *p = &a;
  printf("Address %p stores the value %d\n",p,*p);
  
  return 0;
}
