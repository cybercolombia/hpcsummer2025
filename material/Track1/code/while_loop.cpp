#include <cstdio>

void less_than_7() {
  int num = 0;
  while(num < 7){
    printf("%d ", num);
    num++;
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  less_than_7();
  
  return 0;
}
