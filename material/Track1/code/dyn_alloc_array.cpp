#include <cstdio>

int main(int argc, char *argv[]) {
  int N = 10;
  int *a = new int[N];
  for(int i = 0; i < N; i++){
    a[i] = 1;
  }

  for(int i = 0;  i < N; i++){
    printf("%d ", a[i]);
  }
  printf("\n");

  delete[] a;
  
  return 0;
}
