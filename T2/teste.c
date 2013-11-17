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
  int param = 0;

  if(argc >= 3){
    param = atoi(argv[2]);
  }

  if(fp == NULL){
    fprintf (stderr, "nao conseguiu abrir arquivo!\n");
    exit(EXIT_FAILURE);
  }

  gera(fp, &code, &entry);

  printf("output: %d\n", entry(param));

  fclose(fp);
  return 0;
}
