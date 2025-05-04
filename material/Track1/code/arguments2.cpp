#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
  int an_integer = atoi(argv[1]);
  double a_real = atof(argv[2]);

  printf("The sum of the arguments is %f\n", an_integer+a_real);
  
  return 0;
}
