## Gerador de Código

O objetivo deste trabalho é desenvolver, em C, uma função `gera`
que implementa um pequeno gerador de código (um "micro-compilador") para uma
linguagem de programação bastante simples, chamada **SB**.
A função `gera` deverá ler um arquivo texto contendo o
código fonte de uma ou mais funções escritas em **SB** e retornar dois
ponteiros:

Além disso, deverá ser implementada uma função `libera`, que
libera a memória alocada para o código criado por `gera`.

Os trabalhos podem ser feitos em grupos de até dois alunos.
**Leia com atenção o enunciado do trabalho e as instruções para a entrega.
Em caso de dúvidas, não invente. Pergunte!**

* * *

### A Linguagem **SB**
Funções na linguagem **SB** contém apenas atribuições, 
operações aritméticas, chamadas de outras funções e retornos condicionais.
Todas as funções **SB** são delimitadas por uma marca de início 
(`function`) e uma marca de fim (`end`).

Uma _atribuição_ tem a forma
```
var '=' expr
```
onde `var` é uma variável local ou um parâmetro
e `expr` é uma operação aritmética ou uma chamada de 
função.

Uma _operação aritmética_ tem a forma
```
varc op varc
```
onde `varc` é uma variável local, um parâmetro 
ou uma constante inteira e `op` é um dos operadores:
+, - ou *.

Uma _chamada de função_ tem a forma
```
'call' num varc
```
onde `num` é um número que identifica a função **SB**
que será chamada, com parâmetro `varc`.
A primeira função no arquivo de entrada será a de número **0**, a
segunda a de número **1** e assim por diante. 
Uma função só pode chamar a si mesma ou funções que apareçam antes dela
no arquivo de entrada.
**A última função do arquivo de entrada é a que será chamada pelo
programa principal.**

Um _retorno condicional _ tem a forma
```
'ret?' varc varc
```
Seu significado é que, se o primeiro operando tiver valor **igual a zero**,
a função corrente deve retornar e o valor de retorno é o segundo operando.

Em **SB** as variáveis locais são da forma `vi`, 
sendo o índice **_i_** utilizado para identificar a variável
(ex. `v0, v1`, etc...).
Em **SB** cada função usa no máximo 10 variáveis locais.

Constantes em **SB** são escritas na forma
`$i`, onde `i` é um valor inteiro
(com sinal opcional).
Por exemplo, `$10` representa o valor **10**
e `$-10` representa o valor **-10**.

Parâmetros são da forma `pi`,
sendo p0 o primeiro parâmetro, p1 o segundo, e assim sucessivamente.
Contudo, é importante notar que **apenas a última função definida
no arquivo** (que é chamada externamente) poderá receber mais que
um parâmetro (no máximo 10), pois a sintaxe de **SB** só permite 
a passagem de um parâmetro em uma chamada de função.

A sintaxe da linguagem **SB** pode ser definida um
pouco mais formalmente como abaixo:

```
pgm :: func | func pgm 
func :: header cmds endf
header :: 'function\n'
endf :: 'end\n'
cmds :: cmd '\n' | cmd '\n' cmds
cmd :: att | ret
att :: var ' = ' expr
expr :: oper | call 
oper :: varc op varc 
call :: 'call ' num ' ' varc 
ret :: 'ret? ' varc ' ' varc
var :: 'v' digito  | 'p' digito 
varc :: var | '$' snum 
op :: ' + ' | ' - ' | ' * '
num :: digito | digito num
snum :: [-] num
digito :: 0' | '1' | '2' | '3' | '4' | '5' | '6' | '7'|  '8' |  '9'
```

* * *

### Alguns exemplos

Um exemplo trivial é uma função **SB** que retorna uma constante:

```
function
ret? $0 $1
end
```

O próximo exemplo implementa em **SB** uma função _f(x) = x + 1_.

```
function
p0 = p0 + $1
ret? $0 p0
end
```

Uma função **SB** que calcula o fatorial de seu parâmetro:

```
function
ret? p0 $1
v0 = p0 - $1
v1 = call 0 v0
v0 = p0 * v1
ret? $0 v0
end
```

E, finalmente, uma função que calcula a soma dos quadrados de 1 até o seu
parâmetro, usando uma função auxiliar para calcular o quadrado de um
número:

```
function
p0 = p0 * p0
ret? $0 p0
end
function
ret? p0 $0
v0 = call 0 p0
p0 = p0 - $1
v1 = call 1 p0
v0 = v0 + v1
ret? $0 v0
end
```

* * *

###  O que fazer 

Desenvolva, em C, uma função chamada `gera` que leia um arquivo de entrada contendo o código fonte de **uma ou mais** funções **SB**, gere o código de máquina IA32 correspondente e retorne um ponteiro para a região de memória que contém o código gerado e um ponteiro para o início do código da última função definida no arquivo de entrada.

O código gerado deverá seguir as convenções de C/Linux para a passagem de parâmetros, retorno de valores e salvamento de registradores.

O protótipo de `gera` é o seguinte:
```
typedef int (*funcp) ();
void gera(FILE *f, void **code, funcp *entry);
```

- O parâmetro `f` é o descritor de um arquivo texto já aberto para leitura, de onde deve ser lido o código fonte **SB**.

- O parâmetro `code` é o endereço de uma variável onde deve ser armazenado o ponteiro para a área que contém o código gerado.

- O parâmetro `entry` é o endereço de uma variável onde deve ser armazenado o ponteiro (endereço) do código da função a ser chamada.

O arquivo de entrada terá no máximo 100 linhas, com um comando **SB** por linha.

Desenvolva também uma função que libere a área alocada por  `gera`, com o protótipo:

```
void libera(void *p);
```

Você deverá incluir no arquivo que contém as implementações de `gera` e `libera` o arquivo de
cabeçalho `gera.h`.

* * *

### Execução

Você deve criar um arquivo contendo as funções `gera` e `libera` e **outro arquivo** com uma 
função `main` para testá-las. 

Sua função _main_ deverá abrir o arquivo texto que contém uma ou mais funções **SB** e chamar a função _gera_, passando o arquivo aberto como argumento e os endereços dasvariáveis que deverão receber o endereço do início do código gerado (a tradução das funções **SB** e o endereço da função a ser chamada (a última função do arquivo com o código **SB**.

Após chamar _gera_, sua função _main_ deverá chamar a função retornada, passando os parâmetros apropriados, eimprimir o valor de retorno dessa função. Esse retorno é um valor inteiro, que pode ser exibido com código de formação (`"%d\n"`). A função chamada é a última função do arquivo com o código **SB**.

* * *

### Implementação

A função `gera` deve alocar um bloco de memória para 
escrever o código gerado.
Os valores de retorno de `gera` serão um ponteiro para o início dessa
área e um ponteiro para o início do código da última função (dentro dessa
área, obviamente). 

O código gerado por `gera` deverá ser um 
**_código de máquina IA-32_**, e não um código fonte
assembly. Ou seja, você deverá descobrir o código de máquina que
corresponde às instruções de assembly que implementam cada uma das 
instruções de nossa linguagem, usando o
programa `objdump` e possivelmente a documentação
das instruções da Intel, disponível na página do curso.
Por exemplo, para descobrir o código gerado por  `movl %eax, %ecx`,
você pode criar um arquivo meuteste.s contendo apenas essa instrução,
traduzi-lo com o gcc para gerar um arquivo `meuteste.o`,
e usar `objdump -d meuteste.o` para ver o código de máquina
gerado.

Lembre-se que as instruções assembly ocupam um número variável de
bytes na memória! 

Não é necessário fazer o tratamento de erros do arquivo de entrada,
você pode supor que o código nele estará sempre correto.
Vale a pena colocar alguns testes (ver programa exemplo abaixo)
só para facilitar a própria depuração do seu código, mas **as
entradas usadas como testes na correção do trabalho sempre estarão corretas**.

Para ler e interpretar cada linha da linguagem **SB**, teste
se a linha contém cada um dos formatos possíveis.

IMPORTANTE: Esse programa não é trivial. Implemente sua solução passo a passo, **testando separadamente cada passo implementado!**
