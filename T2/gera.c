/* Guilherme Berger 1210518 3WA */
/* Leonardo Giroto  1210817 3WA */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera.h"
#define LINE_SIZE 32
#define LINE_MAX 100

static char * parseLine(char *codSB, int line);

void gera(FILE *f, void ** code, funcp * entry){
  char *bufferSB = (char*)malloc(sizeof(char) * LINE_SIZE);
  int numLinhas = 0;

  if(!bufferSB) {
    exit(EXIT_FAILURE);
  }

  while( fscanf(f, " %[^\n]", bufferSB) == 1 && numLinhas<LINE_MAX){
    numLinhas++;
    
    printf("%d: %s\n", numLinhas, bufferSB);
    printf("%s\n", parseLine(bufferSB, numLinhas));
    
  }
}

static char * parseLine(char *codSB, int line){
  char * codigoAssembly = (char*)malloc(sizeof(char)*LINE_SIZE);

  strcpy(codigoAssembly, "oi");

  if (codSB[0] == 'f'){
    //function
    if(strcmp(codSB, "function") != 0){
      printf("Comando invalido na linha %d: %s\n", line, codSB);
      exit(EXIT_FAILURE);
    }
    
  }
  else if (codSB[0] == 'e'){
    //end
    if(strcmp(codSB, "end") != 0){
      printf("Comando invalido na linha %d: %s\n", line, codSB);
      exit(EXIT_FAILURE);
    }
  }
  else if (codSB[0] == 'v' || codSB[0] == 'p'){
    //atribuicao
  }
  else if (codSB[0] == 'r'){
    //ret
  }
  else {
    printf("Comando invalido na linha %d: %s\n", line, codSB);
    exit(EXIT_FAILURE);
  }

  return codigoAssembly;
}
