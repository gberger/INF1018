/* Guilherme Berger 1210518 3WA */
/* Leonardo Giroto  1210817 3WA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gera.h"
#define LINE_SIZE 32
#define LINE_MAX 100

//#ifdef DEBUG
  #define debug_printf(x, y) (printf(x, y))
//#else
  //#define debug_printf(x, y) 
//#endif


/**************
 * Local types.
 **************/

/*
 * This type represents a varc.
 * A varc is either a number literal, a local variable or a function parameter variable. 
 * It can be used to hold a var, if the code is guaranteed to be safe.
 * (A var is either a local variable or a function parameter variable.)
 */
typedef struct varc_s {
  /*
   * This field represents what kind of varc it is.
   * 0 - number
   * 1 - local variable
   * 2 - function parameter variable
   */
  enum {
    NUMBER,
    LOCAL,
    PARAM
  } type;

  /*
   * This field's possible values depend on the type:
   * if number: any value
   * else: 0-9
   */
  int i;
} varc_t;

/*
 * This type represents an operator.
 * An operator can be '+', '-' or '*'.
 * It is simply a semantic typedef for char.
 */
typedef char op_t;

/*
 * This type represents the id of a function.
 * It can be a number from 0 to 9.
 * It is simply a semantic typedef for int.
 */
typedef int funcid_t;

/******************************
 * Local functions definitions.
 ******************************/
static char * parseLine(char *bufferSB, int *size);
static char * getFunc(int *size);
static char * getEnd(int *size);
static char * getOp(int *size, varc_t v1, varc_t v2, op_t op, varc_t v3);
static char * getCall(int *size, varc_t v1, funcid_t n, varc_t v2);
static char * getRet(int *size, varc_t cond, varc_t retVal);
static varc_t varc_parse(char * str, int *length);
static int varc_length(varc_t v);

void varc_debug_print(varc_t v){
  char c = (v.type == NUMBER ? '$' : v.type == LOCAL  ? 'v' : 'p');
  debug_printf("%c", c);
  debug_printf("%d", v.i);
}

/*********************
 * Exported functions.
 *********************/
void gera(FILE *f, void ** code, funcp * entry){
  char bufferSB[LINE_SIZE] = {0};
  char * bufferM;
  int readLines = 0;
  int bufferMSize;

  // todo refactor malloc size
  *code = malloc(sizeof(char) * LINE_MAX  * 10);

  while( fscanf(f, " %[^\n]", bufferSB) == 1 && readLines<LINE_MAX){
    readLines++;
    
    debug_printf("< %d: ", readLines);
    debug_printf("%s\n", bufferSB);

    char bufferM = parseLine(bufferSB, &bufferMSize);
    
    //copy bufferM to code


    free(bufferM);
  }
}

void libera(void *code){
  free(code);
}

/**********************************
 * Local functions implementations.
 **********************************/

/* 
 * Parameters:
 *   bufferSB: a line of SB code
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to the given SB code
 */
static char * parseLine(char *bufferSB, int *size){
  //we can have up to three varc in a command
  varc_t v1, v2, v3;
  op_t op;
  funcid_t n;
  int offset;


  if (bufferSB[0] == 'f'){
    //function
    if(strcmp(bufferSB, "function") != 0){
      printf("Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }

    debug_printf("   > function\n", 0);
    
    return getFunc(size);
  }
  
  if (bufferSB[0] == 'e'){
    //end
    if(strcmp(bufferSB, "end") != 0){
      printf("Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }
    
    debug_printf("   > end\n", 0);

    return getEnd(size);
  }
  
  if (bufferSB[0] == 'v' || bufferSB[0] == 'p'){
    //attribution

    v1 = varc_parse(bufferSB, &offset);

    debug_printf("   > ", 0);
    varc_debug_print(v1);
    debug_printf(" = ", 0);

    if(bufferSB[6] == 'c'){
      //var = call num varc
      n = (funcid_t)atoi(bufferSB + 10);
      v2 = varc_parse(bufferSB + 12, &offset);

      debug_printf("call ", 0);
      debug_printf(" %d ", (int)n);
      debug_printf("\n", 0);

      return getCall(size, v1, n, v2);
    } else {
      //var = varc op varc

      v2 = varc_parse(bufferSB + 5, &offset);
      op = bufferSB[5 + offset + 1];
      v3 = varc_parse(bufferSB + 5 + offset + 3, &offset);

      varc_debug_print(v2);
      debug_printf(" %c ", op);
      varc_debug_print(v3);
      debug_printf(" \n", 0);
      return getOp(size, v1, v2, op, v3);
    }
  }

  if (bufferSB[0] == 'r'){
    //ret
    //ret? varc varc
    debug_printf("   > ret? ", 0);

    v1 = varc_parse(bufferSB + 5, &offset);
    v2 = varc_parse(bufferSB + 5 + offset + 1, &offset);

    varc_debug_print(v1);
    debug_printf(" ", 0);
    varc_debug_print(v2);
    debug_printf("\n", 0);

    return getRet(size, v1, v2);
  }

  printf("Comando invalido: %s\n", bufferSB);
  exit(EXIT_FAILURE);

}

/* 
 * Parameters:
 *   str: a string representing a varc in its first values. It can contain anything afterwards.
 *   length: pointer that will be updated to be the number of characters of the varc.
 * Returns:
 *   the varc_t corresponding to the given string.
 */
static varc_t varc_parse(char * str, int *length){
  varc_t varc;

  if(str[0] == '$')
    varc.type = NUMBER;
  else if(str[0] == 'v')
    varc.type = LOCAL;
  else
    varc.type = PARAM;

  varc.i = atoi(str + 1);
  
  *length = 0;
  while(*str != ' ' && *str != '\0'){
    str++;
    (*length)++;
  }

  return varc;
}

/* 
 * Parameters:
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to the beginning of a function
 */
static char * getFunc(int *size){
  return NULL;  
}

/* 
 * Parameters:
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to the end of a function
 */
static char * getEnd(int *size){
  return NULL;
}

/* 
 * Parameters:
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to the atribution of an operation
 */
static char * getOp(int *size, varc_t v1, varc_t v2, op_t op, varc_t v3){
  return NULL;
}

/* 
 * Parameters:
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to the attribution of a function call
 */
static char * getCall(int *size, varc_t v1, funcid_t n, varc_t v2){
  return NULL;
}

/* 
 * Parameters:
 *   size: a pointer to where we'll store the size, in bytes, of the resulting machine code
 * Returns:
 *   an array of machine code, corresponding to a conditional return
 */
static char * getRet(int *size, varc_t cond, varc_t retVal){
  return NULL;
}

