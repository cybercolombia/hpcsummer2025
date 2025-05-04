#include <cstdio>

int add() {
  int sum = 0;
  for(int i = 0; i < 13; i++){
    if(i%2 == 0){
      sum += i;
    }
  }
  return sum;
}

int main(int argc, char *argv[]) {
  int ans = add();
  printf("Sum of the possitive, even integers lesser than 13: %d\n", ans);
  
  return 0;
}
