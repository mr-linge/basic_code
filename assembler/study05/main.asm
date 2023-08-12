.section	.rodata
msg:
	.asciz	"add(%lx,%lx) = %lx\n"
str1:
	.asciz	"This is a test string\n"

.section 	.text
.global 	main
main:                                   // @main
	mov	x0, #0x23
	mov	x1, #0x34

	sub sp, sp, #-0x10
	stp x0, x1, [sp]

	bl	add_test

	ldp x1, x2, [sp]
	mov x3, x0
	adrp	x0, msg
	add	x0, x0, :lo12:msg
	bl	printf

	add sp, sp, #0x10

	mov x0,0
	bl exit
