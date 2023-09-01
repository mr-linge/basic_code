.section	.rodata
msg:
	.asciz	"add(%lx,%lx) = %lx\n"
str1:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10
    stp     x29, x30, [sp]

    adrp    x0, msg
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	mov 	x1, 0x30
	mov 	x2, 0x70
	add		x3, x1, x2
    bl 		printf					        // bl 跳转到 printf 子程序

	adrp    x0,str1
	add		x0, x0, :lo12:str1
	bl 		printf

    ldp     x29, x30, [sp] 					// 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret										// 函数返回退出
