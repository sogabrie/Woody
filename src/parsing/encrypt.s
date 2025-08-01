section .text
    global encrypt_xor_rol_64

; void encrypt_xor_rol_asm(uint8_t *data, size_t len, const uint8_t *key, size_t keylen)
; RDI: data (uint8_t *)
; RSI: len (size_t)
; RDX: key (const uint8_t *)
; RCX: keylen (size_t)
encrypt_xor_rol_64:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    mov     r14, rdi
    mov     r15, rsi
    mov     r12, rdx
    mov     r13, rcx
    xor     r8, r8

.loop_start:
    cmp     r8, r15
    jae     .loop_end

    mov     rax, r8         
    xor     rdx, rdx
    div     r13

    movzx   r9, byte [r12 + rdx]

    movzx   r10, byte [r14 + r8]
    xor     r10b, r9b

    mov     rax, r8
    add     rax, r13
    
    mov     r9, 31
    xor     rdx, rdx
    div     r9

    mov     cl, dl

    rol     r10b, cl

    mov     byte [r14 + r8], r10b

    inc     r8
    jmp     .loop_start

.loop_end:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret


