.section .text
.globl _start
.type _start, @function

_start:
  cmp %rsi, %rdi   
  jge .end
  movb (%rdi), %al
  movb (%rsi), %cl
  movb %al, (%rsi)
  movb %cl, (%rdi)
  dec %rsi
  inc %rdi
  jmp _start
.end:
  ret
