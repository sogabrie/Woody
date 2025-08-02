BITS 32

section .text
    global _decrypt_xor_rol_32

_decrypt_xor_rol_32:
    push    ebp
    mov     ebp, esp
    push    ebx             
    push    esi
    push    edi

    jmp     woody_call_stub

end_code:
    mov     ecx, 43         ; text_size: 
    mov     esi, 32          ; key_size: 

    call    get_text_data_offset
get_text_data_offset:
    mov     edx, [esp]
    sub     edx, 0x10000 ; text
    ret

    xor     ebx, ebx

    jmp     key_call_stub

back_key:
    pop     ebx

.loop_start:
    cmp     ebx, ecx
    jae     .loop_end

    push    eax
    push    edx
    
    mov     eax, ebx
    add     eax, esi
    
    xor     edx, edx
    mov     edi, 31
    div     edi
    
    mov     cl, dl
    
    pop     edx
    pop     eax

    movzx   eax, byte [edx + ebx]
    ror     al, cl

    push    eax
    push    edx
    
    mov     eax, ebx
    xor     edx, edx
    div     ecx

    movzx   edi, byte [ebx + edx]
    
    pop     edx
    pop     eax

    xor     al, dl

    mov     byte [edx + ebx], al

    inc     ebx
    jmp     .loop_start

.loop_end:
    pop     edi             
    pop     esi
    pop     ebx
    mov     esp, ebp
    pop     ebp
    
    jmp     42                  

woody_call_stub:
    push    eax
    push    ebx
    push    ecx
    push    edx

    call    get_eip_woody_str
get_eip_woody_str:
    pop     ecx

    mov     eax, 4
    mov     ebx, 1
    mov     edx, 14
    int     0x80

    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

    jmp     end_code

woody_str: db "....WOODY....", 10

key_call_stub:
    call back_key
key_str: db ''


; section .text
;     global _decrypt_xor_rol_32
; ; void decrypt_xor_rol_asm(uint8_t *data, size_t len, const uint8_t *key, size_t keylen)
; ; [ebp+8]: data (uint8_t *)
; ; [ebp+12]: len (size_t)
; ; [ebp+16]: key (const uint8_t *)
; ; [ebp+20]: keylen (size_t)
; _decrypt_xor_rol_32:
;     push    ebp
;     mov     ebp, esp
;     push    ebx             
;     push    esi
;     push    edi

;     mov     edi, [ebp+8]
;     mov     esi, [ebp+12]
;     mov     ebx, [ebp+16]

;     push    dword [ebp+20]

;     xor     eax, eax

; .loop_start:
;     cmp     eax, esi
;     jae     .loop_end

;     push    eax
;     push    ecx

;     mov     ecx, [ebp+20]
;     add     ecx, eax

;     mov     eax, ecx
;     xor     edx, edx
;     mov     ecx, 31
;     div     ecx

;     mov     cl, dl
    
;     pop     ecx
;     pop     eax
    
;     movzx   eax, byte [edi + eax]
;     ror     al, cl

;     push    eax
;     push    ecx
    
;     mov     eax, eax
;     xor     edx, edx
;     mov     ecx, [ebp+20]
;     div     ecx

;     movzx   ebx, byte [ebx + edx]
    
;     pop     ecx
;     pop     eax

;     xor     al, bl

;     mov     byte [edi + eax], al

;     inc     eax
;     jmp     .loop_start

; .loop_end:
;     add     esp, 4

;     pop     edi             
;     pop     esi
;     pop     ebx
;     mov     esp, ebp
;     pop     ebp
;     ret

