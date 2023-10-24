section .data
    string db "Hello World!", 10, 0
section .text
    global _start
_start:
    mov rax, 1 ; syscall write
    mov rdi, 1 ; write to stdout (fd=1)
    mov rsi, string ; const char * to rsi
    mov rdx, 13
    syscall

    mov rax, 60
    xor rdi, rdi;
    syscall