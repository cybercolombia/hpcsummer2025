#include <cstdio>

int dot_product(int *v1, int *v2, int size) {
  int result = 0;
  for (int i = 0; i < size; i++) {
    result += v1[i] * v2[i];
  }
  return result;
}

int main(int argc, char *argv[]) {
  unsigned N = 20;
  int *v1 = new int[N];
  int *v2 = new int[N];

  for (int i = 0; i < N; i++) {
    v1[i] = 1;
    v2[i] = 1;
  }

  int res = dot_product(v1, v2, N);

  printf("Dot product: %d\n", res);
  
  delete[] v1;
  delete[] v2;
  
  return 0;
}
