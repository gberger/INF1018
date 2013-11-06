#include <stdio.h>
#include <stdlib.h>
#include "gera.h"

#define DEBUG 1

int main(int argc, char **argv){
  if (argc != 2){
    printf("Uso: %s <arquivo>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *f = fopen(argv[1], "r");
  void *code;
  funcp entry;

  if(f == NULL){
    fprintf (stderr, "nao conseguiu abrir arquivo!\n");
    exit(EXIT_FAILURE);
  }

  gera(f, &code, &entry);

  fclose(f);
  return 0;
}
