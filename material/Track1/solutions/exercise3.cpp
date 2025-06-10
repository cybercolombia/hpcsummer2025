#include <cstdio>
#include <string>

void foo(int *p) {
  *p += 2;
}

int main(int argc, char *argv[]) {
  int a = std::stoi(std::string(argv[1]));

  foo(&a);
  printf("a: %d\n",a);
  
  return 0;
}
