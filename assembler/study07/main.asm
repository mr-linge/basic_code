.section	.rodata
msg:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]

    adrp    x0, msg
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	brk 	#0xf000							// 调试断点
    bl 		printf					        // bl 跳转到 printf 子程序

    ldp     x29, x30, [sp]                  // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
