#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp) (int x);

int main(int argc, char **argv){
  unsigned char *code;
  int bytes = argc -1;
  int i;
  funcp f;

  code = (unsigned char*)malloc(bytes);
  f = (funcp)code;

  for(i=0; i<bytes; i++){
    code[i] = (int)strtol(argv[i+1], NULL, 16);
  }

  printf("%d\n", f(0));

  return 0;
}