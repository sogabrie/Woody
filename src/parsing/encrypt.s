section .text
	global encrypt_xor_rol_64

; void encrypt_xor_rol_64(uint8_t *data, size_t len, uint8_t *key, size_t keylen)
; rdi = data, rsi = len, rdx = key, rcx = keylen

encrypt_xor_rol_64:
	xor r8, r8
.loop:
	cmp r8, rsi
	jge .done

	movzx r9, byte [rdx + r8 % rcx]
	movzx r10, byte [rdi + r8]
	xor r10b, r9b

	mov r11, r8
	add r11, rcx
	and r11, 31
	mov cl, r11b
	rol r10b, cl

	mov [rdi + r8], r10b
	inc r8
	jmp .loop

.done:
	ret


; cipher[i] = ROL(plain[i] ^ key[i % keylen], (i + keylen) % 31);

; #include <stdint.h>
; #include <stddef.h>

; static inline uint8_t rol8(uint8_t value, uint8_t shift) {
; 	return (value << shift) | (value >> (8 - shift));
; }

; void encrypt_xor_rol(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
; 	for (size_t i = 0; i < len; i++) {
; 		uint8_t k = key[i % keylen];
; 		uint8_t x = data[i] ^ k;
; 		uint8_t r = (i + keylen) % 31;
; 		data[i] = rol8(x, r);
; 	}
; }

; #include <stdint.h>
; #include <stddef.h>

; static inline uint8_t ror8(uint8_t value, uint8_t shift) {
; 	return (value >> shift) | (value << (8 - shift));
; }

; void decrypt_xor_rol(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
; 	for (size_t i = 0; i < len; i++) {
; 		uint8_t r = (i + keylen) % 31;
; 		uint8_t x = ror8(data[i], r);
; 		uint8_t k = key[i % keylen];
; 		data[i] = x ^ k;
; 	}
; }