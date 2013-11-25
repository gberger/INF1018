/* Guilherme Berger 1210518 3WA */
/* Leonardo Giroto  1210817 3WA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gera.h"
#define LINE_SIZE 32
#define LINE_MAX 100


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
static void parseLine(char *bufferSB, void *code, int *nextByte, int *functions, int *nextFunction);
static void addFunc(void *code, int *nextByte, int *functions, int *nextFunction);
static void addEnd(void *code, int *nextByte);
static void addOp(void *code, int *nextByte, varc_t var1, varc_t var2, op_t op, varc_t var3);
static void addCall(void *code, int *nextByte, varc_t var1, funcid_t n, varc_t var2, int *functions);
static void addRet(void *code, int *nextByte, varc_t cond, varc_t retVal);
static void addLittleEndianNumber(void *code, int *nextByte, int num);
static void addByte(void *code, int *nextByte, unsigned char mach);
static varc_t varc_parse(char * str, int *length);
static int varc_ebp_offset(varc_t v);


/*********************
 * Exported functions.
 *********************/
void gera(FILE *f, void ** code, funcp * entry){
  char bufferSB[LINE_SIZE] = {0};
  int readLines = 0, nextByte = 0, functions[10], nextFunction = 0;

  // the largest line has 21 bytes
  *code = malloc(sizeof(char) * LINE_MAX  * 21);

  if(*code == NULL){
    fprintf(stderr, "Falta de memoria ao alocar espaco para codigo SB.\n");
    exit(EXIT_FAILURE);
  }

  while( fscanf(f, " %[^\n]", bufferSB) == 1 && readLines<LINE_MAX){
    readLines++;
    
    parseLine(bufferSB, *code, &nextByte, functions, &nextFunction);
  }

  *entry = (funcp) ((*code) + functions[nextFunction-1]);

}

void libera(void *code){
  free(code);
}

/**********************************
 * Local functions implementations.
 **********************************/


/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to whatever is in bufferSB
 * Parameters:
 *   bufferSB: a line of SB code
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 *   functions: the array of function indexes
 *   nextFunction: pointer to the length of the array
 */
static void parseLine(char *bufferSB, void *code, int *nextByte, int *functions, int *nextFunction){
  //we can have up to three varc in a command
  varc_t var1, var2, var3;
  op_t op;
  funcid_t n;
  int offset;


  if (bufferSB[0] == 'f'){
    //function
    if(strcmp(bufferSB, "function") != 0){
      fprintf(stderr, "Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }

    addFunc(code, nextByte, functions, nextFunction);
    return;
  }
  
  if (bufferSB[0] == 'e'){
    //end
    if(strcmp(bufferSB, "end") != 0){
      fprintf(stderr, "Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }
    
    addEnd(code, nextByte);
    return;
  }
  
  if (bufferSB[0] == 'v' || bufferSB[0] == 'p'){
    //attribution

    var1 = varc_parse(bufferSB, &offset);

    if(var1.type == NUMBER){
      fprintf(stderr, "Comando invalido: %s\n", bufferSB);
      exit(EXIT_FAILURE);
    }

    if(bufferSB[5] == 'c'){
      //var = call num varc
      n = (funcid_t)atoi(bufferSB + 10);
      var2 = varc_parse(bufferSB + 12, &offset);

      addCall(code, nextByte, var1, n, var2, functions);
      return;
    } else {
      //var = varc op varc

      var2 = varc_parse(bufferSB + 5, &offset);
      op = bufferSB[5 + offset + 1];
      var3 = varc_parse(bufferSB + 5 + offset + 3, &offset);

      if(op != '+' && op != '-' && op != '*'){
        fprintf(stderr, "Comando invalido: %s\n", bufferSB);
        exit(EXIT_FAILURE);
      }

      addOp(code, nextByte, var1, var2, op, var3);
      return;
    }
  }

  if (bufferSB[0] == 'r'){
    //ret? varc varc

    var1 = varc_parse(bufferSB + 5, &offset);
    var2 = varc_parse(bufferSB + 5 + offset + 1, &offset);

    addRet(code, nextByte, var1, var2);
    return;
  }

  fprintf(stderr, "Comando invalido: %s\n", bufferSB);
  exit(EXIT_FAILURE);
}

/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to a 'function' command
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 *   functions: the array of function indexes
 *   nextFunction: the size of the array
 * Code:
 *   0x55           = push %ebp
 *   0x89 0xe5      = mov %esp, %ebp
 *   0x83 0xec 0x28 = sub $40, %esp
 */
static void addFunc(void *code, int *nextByte, int *functions, int *nextFunction){
  functions[(*nextFunction)++] = *nextByte;

  addByte(code, nextByte, 0x55);
  addByte(code, nextByte, 0x89);
  addByte(code, nextByte, 0xe5);
  addByte(code, nextByte, 0x83);
  addByte(code, nextByte, 0xec);
  addByte(code, nextByte, 0x28);
}

/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to an 'end' command
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 * Code:
 *   0x89 0xec = mov %ebp, %esp
 *   0x5d      = pop %ebp
 *   0xc3      = ret
 */
static void addEnd(void *code, int *nextByte){
  addByte(code, nextByte, 0x89);
  addByte(code, nextByte, 0xec);
  addByte(code, nextByte, 0x5d);
  addByte(code, nextByte, 0xc3);
}

/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to an 'operation' command (form: 'var = varc op varc')
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 */
static void addOp(void *code, int *nextByte, varc_t var1, varc_t var2, op_t op, varc_t var3){
  int offset;
  
  // mov var2, %ecx
  // mov var3, %edx
  // op %edx, %ecx
  // mov %edx, var1
  
  if(var2.type == NUMBER){
    // mov $x, %ecx
    addByte(code, nextByte, 0xb9);
    addLittleEndianNumber(code, nextByte, var2.i);
  } else {
    // mov offset(%ebp), %edx
    
    offset = varc_ebp_offset(var2);
    addByte(code, nextByte, 0x8b);
    addByte(code, nextByte, 0x4d);
    addByte(code, nextByte, offset);
  }

  if(var3.type == NUMBER){
    // mov $x, %edx
    addByte(code, nextByte, 0xba);
    addLittleEndianNumber(code, nextByte, var3.i);
  } else {
    // mov offset(%ebp), %edx
    
    offset = varc_ebp_offset(var3);
    addByte(code, nextByte, 0x8b);
    addByte(code, nextByte, 0x55);
    addByte(code, nextByte, offset);
  }

  if(op == '+'){
    // add %edx, %ecx
    addByte(code, nextByte, 0x01);
    addByte(code, nextByte, 0xd1);
  } else if(op == '-'){
    // sub %edx, %ecx
    addByte(code, nextByte, 0x29);
    addByte(code, nextByte, 0xd1);
  } else {
    // imul %edx, %ecx
    addByte(code, nextByte, 0x0f);
    addByte(code, nextByte, 0xaf);
    addByte(code, nextByte, 0xca);
  }

  // mov %ecx, offset(%ebp)
  offset = varc_ebp_offset(var1);
  addByte(code, nextByte, 0x89);
  addByte(code, nextByte, 0x4d);
  addByte(code, nextByte, offset);
}

/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to a 'call' command (form: 'var = call num varc')
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 */
static void addCall(void *code, int *nextByte, varc_t var1, funcid_t n, varc_t var2, int *functions){
  int offset;

  // mov var2, %ecx
  // push %ecx
  // call n
  // add $4, %esp
  // mov %eax, var1

  if(var2.type == NUMBER){
    // mov $x, %ecx
    addByte(code, nextByte, 0xb9);
    addLittleEndianNumber(code, nextByte, var2.i);
  } else {
    // mov offset(%ebp), %edx
    
    offset = varc_ebp_offset(var2);
    addByte(code, nextByte, 0x8b);
    addByte(code, nextByte, 0x4d);
    addByte(code, nextByte, offset);
  }

  // push %ecx
  addByte(code, nextByte, 0x51);

  // call 
  addByte(code, nextByte, 0xe8);
  offset = functions[(int)n] - (*nextByte + 4);
  addLittleEndianNumber(code, nextByte, offset);

  // add $4, %esp
  addByte(code, nextByte, 0x83);
  addByte(code, nextByte, 0xc4);
  addByte(code, nextByte, 0x04);

  // mov %eax, offset(%ebp)
  offset = varc_ebp_offset(var1);
  addByte(code, nextByte, 0x89);
  addByte(code, nextByte, 0x45);
  addByte(code, nextByte, offset); 

}

/* 
 * Description:
 *   adds machine code at the end of code and increments nextByte accordingly
 *   the machine code corresponds to a 'return' command (form: 'ret? varc varc')
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 */
static void addRet(void *code, int *nextByte, varc_t cond, varc_t retVal){

  int offset, toFix;

  // mov cond, %ecx
  // cmp $0, %ecx,
  // jne ...
  // mov retVal, %eax
  // (addEnd)
  // :...
  
  if(cond.type == NUMBER){
    // mov $x, %ecx
    addByte(code, nextByte, 0xb9);
    addLittleEndianNumber(code, nextByte, cond.i);
  } else {
    // mov offset(%ebp), %edx
    
    offset = varc_ebp_offset(cond);
    addByte(code, nextByte, 0x8b);
    addByte(code, nextByte, 0x4d);
    addByte(code, nextByte, offset);
  }

  // cmp $0, %ecx
  addByte(code, nextByte, 0x83);
  addByte(code, nextByte, 0xf9);
  addByte(code, nextByte, 0x00);

  // jne ...
  addByte(code, nextByte, 0x0f);  
  addByte(code, nextByte, 0x85);
  // come back later to fix it
  toFix = *nextByte;
  (*nextByte) += 4;
  
  if(retVal.type == NUMBER){
    // mov $i, %eax

    addByte(code, nextByte, 0xb8);
    addLittleEndianNumber(code, nextByte, retVal.i);
  } else {
    // mov offset(%ebp), %eax

    offset = varc_ebp_offset(retVal);
    addByte(code, nextByte, 0x8b);
    addByte(code, nextByte, 0x45);
    addByte(code, nextByte, offset);
  }

  // ret
  addEnd(code, nextByte);

  // fix the jump
  // the math is:
  // address_to_jump_to - address_after_jump_instruction
  offset = (int)(*nextByte) - (toFix + 4);
  addLittleEndianNumber(code, &toFix, offset);
}

/* 
 * Description:
 *   adds the given number to the code in little endian notation.
 *   useful when using "mov $num, ..." and "jump num"
 * Parameters:
 *   code: the generated machine code
 *   nextByte: the size of the machine code until now
 *   num: the number to be added
 */

static void addLittleEndianNumber(void *code, int *nextByte, int num){
  addByte(code, nextByte, num & 0xff);
  num >>= 8;
  addByte(code, nextByte, num & 0xff);
  num >>= 8;
  addByte(code, nextByte, num & 0xff);
  num >>= 8;
  addByte(code, nextByte, num & 0xff);
}

/*
 * Description:
 *   adds the given machine code to the code array
 *   also increments the value pointed by nextByte
 */
static void addByte(void *code, int *nextByte, unsigned char mach){
  ((unsigned char*)code)[(*nextByte)++] = mach;
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
 * Description
 *   gives the offset of the address of the varc, in relation to %ebp
 */
static int varc_ebp_offset(varc_t v){
  if(v.type == PARAM)
    return v.i * 4 + 8;
  if(v.type == LOCAL) 
    return -(v.i * 4) - 4;
  return 0;
}
