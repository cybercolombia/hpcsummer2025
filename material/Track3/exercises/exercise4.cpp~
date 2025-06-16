#include <cstdio>

int* element(int *array, int j, int i) {
  int *p = &array[j*4+i];
  return p;
}

int main(int argc, char *argv[]) {
  int *a = new int[20];

  for(int j = 0; j < 5; j++){
    for(int i = 0; i < 4; i++){
      *element(a,j,i) = 4*j+i;
    }
  }

  for(int j = 0; j < 5; j++){
    for(int i = 0; i < 4; i++){
      printf("%d ",*element(a,j,i));
    }
    printf("\n");
  }
  
  delete[] a;
  
  return 0;
}
