section .data
    format db "%d %c", 0
    i dd 5
    c db 'e'
section .text
    global main
    extern printf
main:
    push dword [i]
    push format
    call printf
    add esp, 8

    mov eax, 1
    xor ebx, ebx
    int 0x80