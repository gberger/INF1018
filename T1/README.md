## Manipulação de inteiros grandes

O objetivo deste trabalho é construir uma biblioteca para
manipulação de inteiros grandes de 128 bits. A biblioteca deverá prover
as operações aritméticas
básicas (somar, subtrair, multiplicar), comparação e operações de deslocamento de bits.

A biblioteca deve ser implementada em C, usando a seguinte definição
para o tipo do inteiro grande:

    #define NUM_BYTES 16
    typedef unsigned char BigInt[NUM_BYTES];

Ou seja, uma variável do tipo BigInt é representada por este array,
que deve ser interpretado como um único inteiro de 128 bits, seguindo a
ordem **little-endian**.

Dependendo do tipo de operação, o valor armazenado deve ser interpretado
como um valor sem sinal (_unsigned_) ou como um valor com sinal,
representado **em complemento a dois**.

Por exemplo, o array

    {0xFE, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF}
    
se interpretado como um valor inteiro **com sinal**, 
representa o inteiro **` 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE ` (-2)**. Este mesmo array, se interpretado como um valor inteiro **sem sinal**,
representa o inteiro **2^128 - 2**.

* * *

### Especificação da Biblioteca

A implementação das funções da biblioteca deve obedecer os protótipos a seguir:

    /* Atribuição */
    
    /* res = val (extensão **com sinal**) */
    void big_val (BigInt res, int val);
    
    /* res = uval (extensão **sem sinal**) */
    void big_uval (BigInt res, unsigned int uval);
    
    /* Operacoes aritméticas */
    
    /* res = a + b */
    void big_sum (BigInt res, BigInt a, BigInt b);
    
    /* res = a - b */
    void big_sub (BigInt res, BigInt a, BigInt b);
    
    /* res = a * b **(com sinal)** */
    void big_mul (BigInt res, BigInt a, BigInt b);
    
    /* res = a * b **(sem sinal)** */
    void big_umul (BigInt res, BigInt a, BigInt b);
    
    /* Operacoes de deslocamento */
    
    /* res = a &lt;&lt; n */
    void big_shl (BigInt res, BigInt a, int n);
    
    /* res = a &gt;&gt; n **(lógico)** */
    void big_shr (BigInt res, BigInt a, int n);
    
    /* Comparação: retorna -1 (a &lt; b), 0 (a == b), 1 (a &gt; b) */
    
    /* comparação com sinal */
    int big_cmp(BigInt a, BigInt b);
    
    /* comparação sem sinal */
    int big_ucmp(BigInt a, BigInt b);

Sua biblioteca deve ser implementada como um arquivo C com nome "bigint.c".
Este arquivo deve conter **apenas a implementação das funções acima**
(e, eventualmente, funções auxiliares). Ele deve incluir a linha `#include "bigint.h"` (arquivo de cabeçalho com os protótipos das funções da biblioteca)

* * *

## Testando as funções

Para testar as suas funções, crie **um outro arquivo**, `teste.c`,
contendo uma função `main`. Ele também deve incluir a linha `#include "bigint`.

Crie seu programa executável `teste` com a linha:
   
   gcc -Wall -o teste bigint.c teste.c
   
   
* * *

## Observações

* O trabalho pode ser feito em grupo de dois alunos.

* Implemente uma função de cada vez, e teste bem
sua implementação. É melhor entregar um
trabalho com apenas algumas funções funcionando
corretamente do que com todas as funções
funcionando incorretamente.

* Se você não implementar alguma das
funções, crie uma função _dummy_ com o nome da
função que faltou com corpo '{ return; }', para que o
programa de teste usado para correção do trabalho
possa ser gerado sem problemas.

* Alguns grupos poderão ser chamados para explicar ao professor,
no laboratório, o código do trabalho entregue. Será marcada uma data
específica para essa avaliação.
