.section	.rodata
msg:
	.asciz	"add(%lx,%lx) = %lx\n"
str1:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x20                   // 获取栈空间
    stp     x29, x30, [sp, #0x10]           // 保存 fp lr
    add	    x29, sp, #0x10                  // fp 保存上一个 fp 的位置

	mov		x0, #0x70
	mov		x1, #0x60

	stp 	x0, x1, [sp]					// 跳转到子程序前在栈中保存局部变量

	bl	add_test							// 跳转到子程序

	ldp 	x1, x2, [sp]					// 从子程序返回后还原局部变量
	mov 	x3, x0
	adrp	x0, msg
	add		x0, x0, :lo12:msg
	bl		printf

    ldp     x29, x30, [sp, #0x10]          // 恢复x29,x30寄存器
    add     sp, sp, #0x20
	ret
