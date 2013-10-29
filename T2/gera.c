/* Guilherme Berger 1210518 3WA */
/* Leonardo Giroto  1210817 3WA */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera.h"
#define LINE_SIZE 32
#define LINE_MAX 100

static char * toAssembly(char *codSB, int line);
static char * strip(char *str);

void gera(FILE *f){
  char *bufferSB = (char*)malloc(sizeof(char) * LINE_SIZE);
  //char codigoAssembly[LINE_SIZE];
  int numLinhas = 0;

  if(!bufferSB) {
    exit(EXIT_FAILURE);
  }

  while( fscanf(f, " %[^\n]", bufferSB) == 1 && numLinhas<LINE_MAX){
    numLinhas++;
    
    bufferSB = strip(bufferSB);
    printf("%d: %s\n", numLinhas, bufferSB);
    printf("%s\n", toAssembly(bufferSB, numLinhas));
    /*codigoAssembly = toAssembly(bufferSB);
    codigoMaquina = toMachine(codigoAssembly);*/
    
  }
}

static char * strip(char *str){
  char *ref = (char*)malloc(sizeof(char) * LINE_SIZE);
  int i, len = strlen(str);
  int j = 0;

  if(!ref) {
    exit(EXIT_FAILURE);
  }

  for(i=0; i<LINE_SIZE; i++){
    ref[i] = 0;
  }

  for(i=0; i<len; i++){
    //se não estiver no último caractere da linha
    //OU se não [ o atual e o próximo forem um espaço ]
    if(i == len-1 || !(str[i] == ' ' && str[i+1] == ' ')){
      ref[j] = str[i];
      j++;
    }
  }

  return ref;
}

static char * toAssembly(char *codSB, int line){
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
