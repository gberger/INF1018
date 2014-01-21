INF1018 - Software Básico - Basic Software/Computer Systems
===========================================================

Exercises and projects for the INF1018 course at PUC-Rio.

Course program
--------------

> Arquitetura básica de um computador; CPU, memória, periféricos. Armazenamento e representação de tipos primitivos; base binária, complemento a dois, ponto flutuante, código ASCII. Conjunto de instruções típico de uma CPU; modos de endereçamento, interrupções. Linguagem de montagem. Serviços e chamadas ao Sistema Operacional; Sistema de Arquivos. Modelo de Execução de Uma Linguagem de Alto Nível; implementação de instruções e dados. Organização de memória em um programa; área estática, área de alocação dinâmica, registros de ativação. Ligação e relocação de programas objeto.

> Basic architecture of a computer; CPU, memmory, peripherals. Storage and representation of primitive types, binary; two's complement, floating point, ASCII code. CPU instructions; addresses, interrupts. Assembly language. Operating System calls; file systems. Execution model of a high level language; implementation of instructions and data. Memmory layout; static area, dynamic allocation area, activation registers. Linking of object files.


Projects
--------

The projects are in folders that start with "T" (for "trabalho").

### T1

The first project is an implementation of 16-bit integers in C. Basic operations are provided, such as addition, subtraction, multiplication and bit shifting, as well as comparator functions.

### T2

The second project is an implementation in C of a very simple programming language. The code is read from a file and the corresponding machine code is written to `malloc`-ed memory. A function pointer points to the program's entry point, allowing the user of the library to run the generated code.