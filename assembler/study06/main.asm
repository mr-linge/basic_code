.section	.rodata
msg:
	.asciz	"adrp = %lx, lo12 = %lx, add = %lx\n"
str1:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]

    adrp    x0, msg
	mov 	x1, x0
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	sub 	x2, x0, x1
	mov		x3, x0
    bl 		printf					        // bl 跳转到 printf 子程序

    ldp     x29, x30, [sp]                  // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
