; nasm -f elf64 decrypt_64.s -o decrypt_64.o
; nasm -f elf32 decrypt_32.s -o decrypt_32.o
; objcopy -j .text -O binary decrypt_64.o /dev/stdout | xxd -p -c 256 | sed 's/\(..\)/\\x\1/g' | tr -d '\n' ; echo
; objcopy -j .text -O binary decrypt_32.o /dev/stdout | xxd -p -c 256 | sed 's/\(..\)/\\x\1/g' | tr -d '\n' ; echo
; objdump -d -M intel -j .text decrypt_64.o
; objdump -d -M intel -j .text decrypt_32.o


BITS 64

section .text
    global decrypt_xor_rol_64

decrypt_xor_rol_64:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    jmp     woody_call_stub

end_code:
    mov     r15, 43         ; text_size:
    lea     r14, [rel back_key] ; r14 text
    mov     r13, 32          ; key_size:
    xor     r8, r8

    jmp     key_call_stub

back_key:
    pop     r12

.loop_start:
    cmp     r8, r15
    jae     .loop_end

    mov     rax, r8
    add     rax, r13
    
    mov     r9, 31
    xor     rdx, rdx
    div     r9

    mov     cl, dl

    movzx   r10, byte [r14 + r8]
    ror     r10b, cl

    mov     rax, r8
    xor     rdx, rdx
    div     r13

    movzx   r9, byte [r12 + rdx]

    xor     r10b, r9b

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
    
    jmp     42          ; end        

woody_call_stub:
    push    rax 
    push    rdi 
    push    rsi 
    push    rdx 

    mov     rax, 1
    mov     rdi, 1
    lea     rsi, [rel woody_str]
    mov     rdx, 14
    syscall

    pop     rdx
    pop     rsi
    pop     rdi
    pop     rax

    jmp     end_code

woody_str: db "....WOODY....", 10

key_call_stub:
    call back_key
key_str: db ''


; section .text
;     global decrypt_xor_rol_64

; ; void decrypt_xor_rol_asm(uint8_t *data, size_t len, const uint8_t *key, size_t keylen)
; ; RDI: data (uint8_t *)
; ; RSI: len (size_t)
; ; RDX: key (const uint8_t *)
; ; RCX: keylen (size_t)
; decrypt_xor_rol_64:
;     push    rbp
;     mov     rbp, rsp
;     push    rbx
;     push    r12
;     push    r13
;     push    r14
;     push    r15

;     mov     r14, rdi
;     mov     r15, rsi
;     mov     r12, rdx
;     mov     r13, rcx

;     xor     r8, r8

; .loop_start:
;     cmp     r8, r15
;     jae     .loop_end

;     mov     rax, r8 
;     add     rax, r13
    
;     mov     r9, 31
;     xor     rdx, rdx
;     div     r9

;     mov     cl, dl

;     movzx   r10, byte [r14 + r8]
;     ror     r10b, cl

;     mov     rax, r8         
;     xor     rdx, rdx
;     div     r13

;     movzx   r9, byte [r12 + rdx]

;     xor     r10b, r9b

;     mov     byte [r14 + r8], r10b

;     inc     r8
;     jmp     .loop_start

; .loop_end:
;     pop     r15
;     pop     r14
;     pop     r13
;     pop     r12
;     pop     rbx
;     pop     rbp
;     ret

