.section	.rodata
msg:
	.asciz	"adrp = %lx, lo12 = %lx,  vaddr = %lx\n"
str1:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]                  // 保存 fp lr
    add	    x29, sp, #0                     // fp 保存上一个 fp 的位置

    adrp    x1, msg
	add		x0, x1, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	sub		x2, x0, x1
	mov 	x3, x0
    bl 		printf					        // bl 跳转到 printf 子程序

    ldp     x29, x30, [sp]                  // 恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
