	.text
	.globl	add_test
add_test:
	sub	sp, sp, #0x10
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	ldr	w8, [sp, #12]
	ldr	w9, [sp, #8]
	add	w0, w8, w9
	add	sp, sp, #0x10
	ret