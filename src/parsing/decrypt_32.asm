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
    
    jmp     42              ; end   

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



