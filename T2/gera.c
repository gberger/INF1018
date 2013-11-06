/* Guilherme Berger 1210518 3WA */
/* Leonardo Giroto  1210817 3WA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gera.h"
#define LINE_SIZE 32
#define LINE_MAX 100

//#ifdef DEBUG
  #define debug_printf(x, y) (printf(x, y))
//#else
  //#define debug_printf(x, y) 
//#endif

static char * parseLine(char *bufferSB, int *size);
static char * getFunc(int *size);
static char * getEnd(int *size);
static char * getAttr(int *size);
static char * getCall(int *size);
static char * getRet(int *size);


void gera(FILE *f, void ** code, funcp * entry){
  char bufferSB[LINE_SIZE] = {0};
  char * bufferM;
  int numLinhas = 0;
  int bufferMSize;  

  while( fscanf(f, " %[^\n]", bufferSB) == 1 && numLinhas<LINE_MAX){
    numLinhas++;
    
    debug_printf("%d\n", numLinhas);

    char bufferM = parseLine(bufferSB, &bufferMSize);
    //

    free(bufferM);
  }
}

static char * parseLine(char *bufferSB, int *size){
  if (bufferSB[0] == 'f'){
    //function
    if(strcmp(bufferSB, "function") != 0){
      printf("Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }
    
    return getFunc(size);
  }
  
  if (bufferSB[0] == 'e'){
    //end
    if(strcmp(bufferSB, "end") != 0){
      printf("Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }

    return getEnd(size);
  }
  
  if (bufferSB[0] == 'v' || bufferSB[0] == 'p'){
    //atribuicao

    return getAttr(size);
  }
  
  if (bufferSB[0] == 'c'){
    //call

    return getCall(size);
  }

  if (bufferSB[0] == 'r'){
    //ret

    return getRet(size);
  }

  printf("Comando invalido: %s\n", bufferSB);
  exit(EXIT_FAILURE);

}

static char * getFunc(int *size){
  return NULL;  
}

static char * getEnd(int *size){
  return NULL;
}

static char * getAttr(int *size){
  return NULL;
}

static char * getCall(int *size){
  return NULL;
}

static char * getRet(int *size){
  return NULL;
}
