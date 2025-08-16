

; BITS 32

; segment .text
; 	global decrypt

; decrypt:
; 	pusha
; 	jmp woody
; end_code:
; 	pop ecx
; 	mov eax, 4
; 	mov ebx, 1
; 	mov edx, 14
; 	int 0x80
; 	mov esi, 43 ; text_size
; 	call get_eip ; text
; 	sub edi, 0x10000
; 	jmp key
; back_key:
; 	pop ebx ; key
;     push    dword 32

;     xor     eax, eax

; .loop_start:
;     cmp     eax, esi
;     jae     .loop_end

;     push    eax
;     push    ecx

;     mov     ecx, 32
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
;     mov     ecx, 32
;     div     ecx

;     movzx   ebx, byte [ebx + edx]
    
;     pop     ecx
;     pop     eax

;     xor     al, bl

;     mov     byte [edi + eax], al

;     inc     eax
;     jmp     .loop_start

; .loop_end:
; 	popa
; 	jmp 42
; get_eip:
; 	mov edi, [esp]
; 	ret
; woody:
; 	call end_code
; 	woody_str: db "....WOODY....", 10
; key:
; 	call back_key
; 	key_str: db ''

BITS 32

segment .text
    global decrypt

decrypt:
    pusha
    jmp woody
end_code:
    pop ecx
    mov eax, 4
    mov ebx, 1
    mov edx, 14
    int 0x80
    mov ecx, 43 ; text_size
    mov esi, 32 ; key_size 
    call get_eip ; text
    sub edx, 0x10000
    xor ebx, ebx ; key_index
    xor eax, eax ; key_offset
    jmp key
back_key:
    pop edi ; key
routine:
    xor al, al
    mov cl, bl
    add cl, 4
    movzx ebx, cl
    mov cl, bl
    and cl, 31
    mov ch, 8
    div ch
    mov cl, ah
    movzx ecx, cl
    mov al, byte [edx]
    rol al, cl
    mov cl, byte [edi + ebx]
    xor al, cl
    mov byte [edx], al
    inc edx
    inc ebx
    cmp ebx, esi
    jne loopinstr
    add ah, byte 42
    xor ebx, ebx
loopinstr:
    loop routine
    popa
    jmp 42
get_eip:
    mov edx, [esp]
    ret
woody:
    call end_code
    woody_str: db "....WOODY....", 10
key:
    call back_key
    key_str: db ''