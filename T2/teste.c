#include <stdio.h>
#include <stdlib.h>
#include "gera.h"

#define DEBUG 1

int main(int argc, char **argv){
  if (argc != 2){
    printf("Uso: %s <arquivo>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[1], "r");
  void *code;
  funcp entry;

  if(fp == NULL){
    fprintf (stderr, "nao conseguiu abrir arquivo!\n");
    exit(EXIT_FAILURE);
  }

  gera(fp, &code, &entry);

  printf("output: %d\n", entry());

  fclose(fp);
  return 0;
}
