#include <cstdio>
#include <cstdlib>

int greatest(int a, int b, int c) {
  if(a > b && a > c){
    return a;
  }else if(b > a && b > c){  
    return b;
  }else{
    return c;
  }
}

int main(int argc, char *argv[]) {
  int x = atoi(argv[1]);
  int y = atoi(argv[2]);
  int z = atoi(argv[3]);

  int gr = greatest(x, y, z);
  printf("The greatest number is %d\n",gr);
  
  return 0;
}
