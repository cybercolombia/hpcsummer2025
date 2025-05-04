#include <cstdio>

int* element(int *array, int i, int j) {
  int *p = &array[i*4+j];
  return p;
}

int main(int argc, char *argv[]) {
  int *a = new int[20];

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 4; j++){
      *element(a,i,j) = 4*i+j;
    }
  }

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 4; j++){
      printf("%d ",*element(a,i,j));
    }
    printf("\n");
  }
  
  delete[] a;
  
  return 0;
}
