#include <cstdio>

int add_all(int from, int to) {
  int sum = 0;
  for(int i = from; i <= to; i++){
    sum += i;
  }
  return sum;
}

int main(int argc, char *argv[]) {
  int ans = add_all(3, 10);
  printf("Result of adding integers from 3 to 10: %d\n", ans);
  
  return 0;
}
