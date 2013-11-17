#include <stdio.h>
#include <stdlib.h>
#include "gera.h"

#define DEBUG 1

int main(int argc, char **argv){
  if (argc < 2){
    printf("Uso: %s <arquivo> [<param>]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[1], "r");
  void *code;
  funcp entry;
  int i, param[10] = {0};

  for(i=2; i<argc; i++){
    param[i-2] = atoi(argv[i]);
  }

  if(fp == NULL){
    fprintf (stderr, "nao conseguiu abrir arquivo!\n");
    exit(EXIT_FAILURE);
  }

  gera(fp, &code, &entry);

  printf("output: %d\n", entry(param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], param[8], param[9]));

  fclose(fp);
  return 0;
}
