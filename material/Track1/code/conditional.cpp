#include <cstdio>
#include <string>

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
  int x = std::stoi(std::string(argv[1]));
  int y = std::stoi(std::string(argv[2]));
  int z = std::stoi(std::string(argv[3]));

  int gr = greatest(x, y, z);
  printf("The greatest number is %d\n",gr);
  
  return 0;
}
