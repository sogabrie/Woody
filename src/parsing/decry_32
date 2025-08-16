section .text
    global _decrypt_xor_rol_32
; void decrypt_xor_rol_asm(uint8_t *data, size_t len, const uint8_t *key, size_t keylen)
; [ebp+8]: data (uint8_t *)
; [ebp+12]: len (size_t)
; [ebp+16]: key (const uint8_t *)
; [ebp+20]: keylen (size_t)
_decrypt_xor_rol_32:
    push    ebp
    mov     ebp, esp
    push    ebx             
    push    esi
    push    edi

    mov     edi, [ebp+8]
    mov     esi, [ebp+12]
    mov     ebx, [ebp+16]

    push    dword [ebp+20]

    xor     eax, eax

.loop_start:
    cmp     eax, esi
    jae     .loop_end

    push    eax
    push    ecx

    mov     ecx, [ebp+20]
    add     ecx, eax

    mov     eax, ecx
    xor     edx, edx
    mov     ecx, 31
    div     ecx

    mov     cl, dl
    
    pop     ecx
    pop     eax
    
    movzx   eax, byte [edi + eax]
    ror     al, cl

    push    eax
    push    ecx
    
    mov     eax, eax
    xor     edx, edx
    mov     ecx, [ebp+20]
    div     ecx

    movzx   ebx, byte [ebx + edx]
    
    pop     ecx
    pop     eax

    xor     al, bl

    mov     byte [edi + eax], al

    inc     eax
    jmp     .loop_start

.loop_end:
    add     esp, 4

    pop     edi             
    pop     esi
    pop     ebx
    mov     esp, ebp
    pop     ebp
    ret
