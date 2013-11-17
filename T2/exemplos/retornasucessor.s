.globl retornasucessor

retornasucessor:
  push %ebp
  mov %esp, %ebp
  add $1, 8(%ebp)
  mov 8(%ebp), %eax
  mov %ebp, %esp
  pop %ebp
  ret
