section .text
	global decrypt_xor_rol_64

; void decrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen)
; rdi = data, rsi = len, rdx = key, rcx = keylen

decrypt_xor_rol_64:
	xor r8, r8
.loop:
	cmp r8, rsi
	jge .done

	movzx r9, byte [rdx + r8 % rcx]
	movzx r10, byte [rdi + r8]

	mov r11, r8
	add r11, rcx
	and r11, 31
	mov cl, r11b
	ror r10b, cl

	xor r10b, r9b
	mov [rdi + r8], r10b

	inc r8
	jmp .loop

.done:
	ret