BITS 64

SECTION .text
    global _start

_start:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg]
    mov rdx, msg_len
    syscall

    mov rax, [rel orig_entry]
    jmp rax

SECTION .data
msg db '....WOODY....', 10
msg_len equ $ - msg
align 8
orig_entry:
    times 8 db 0