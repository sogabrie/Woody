section .text
	global decrypt_xor_rol_32

; void decrypt_xor_rol_32(uint8_t *data, size_t len, uint8_t *key, size_t keylen)
; [esp+4]  = data
; [esp+8]  = len
; [esp+12] = key
; [esp+16] = keylen

decrypt_xor_rol_32:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov esi, [ebp + 8]       ; len
	mov edi, [ebp + 4]       ; data
	mov ebx, 0               ; i = 0
	mov edx, [ebp + 12]      ; key
	mov ecx, [ebp + 16]      ; keylen

.loop:
	cmp ebx, esi
	jge .done

	movzx eax, byte [edx + ebx % ecx]
	movzx edx, byte [edi + ebx]

	mov eax, ebx
	add eax, ecx
	and eax, 31
	mov cl, al
	ror dl, cl

	xor dl, byte [edx + ebx % ecx]
	mov [edi + ebx], dl

	inc ebx
	jmp .loop

.done:
	pop ebx
	pop edi
	pop esi
	pop ebp
	ret