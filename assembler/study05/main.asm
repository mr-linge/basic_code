.section	.rodata
msg:
	.asciz	"add(%lx,%lx) = %lx\n"
str1:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]

	mov	x0, #0x70
	mov	x1, #0x60

	sub sp, sp, #0x10
	stp x0, x1, [sp]

	bl	add_test

	ldp x1, x2, [sp]
	add     sp, sp, #0x10
	mov x3, x0
	adrp	x0, msg
	add	x0, x0, :lo12:msg
	bl	printf

    ldp     x29, x30, [sp]                  // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
