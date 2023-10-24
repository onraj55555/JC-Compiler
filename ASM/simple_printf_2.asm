section .data
    string db "Hello World!", 10, 0

section .text
    global _start

_start:
    mov esi, string

loop_start:
    mov al, [rsi]
    cmp al, 0
    je loop_end

    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall

    inc rsi

    jmp loop_start

loop_end:
    mov rax, 60
    xor rdi, rdi
    syscall