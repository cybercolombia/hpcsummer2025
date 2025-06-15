#include <cstdio>

void print_hello() {
  printf("Hello!\n");
}

double add_reals(double a, double b) {
  return a + b;
}

int main(int argc, char *argv[]) {
  print_hello();

  double x = 3.14, y = 2.71;
  double res = add_reals(x, y);
  printf("Sum result: %f\n",res);
  
  return 0;
}
