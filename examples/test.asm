global _main

section .data
m_test: db "test", 10

section .text
_main:
mov rax, 0x2000004 ; write
mov rdi, 1 ; 1 stdout
mov rsi, m_test
mov rdx, 5 ; message length
syscall

mov rax, 0x2000001 ; exit
mov rdi, 0 ; 0 exit code
syscall
