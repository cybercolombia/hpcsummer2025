#include <cstdio>
#include <string>

int main(int argc, char *argv[]) {
  int an_integer = std::stoi(std::string(argv[1]));
  double a_real = std::stof(std::string(argv[2]));

  printf("The sum of the arguments is %f\n", an_integer+a_real);
  
  return 0;
}
