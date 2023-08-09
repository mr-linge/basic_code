	.text
	.globl	main
main:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	mov	w8, wzr
	str	w8, [sp, #8]                    // 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	bl	printf
	ldr	w0, [sp, #8]                    // 4-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret