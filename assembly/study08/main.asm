.section	.rodata
msg:
	.asciz	"This is a test string.\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]                  // 保存 fp lr
    add	    x29, sp, #0                     // fp 保存上一个 fp 的位置

    adrp    x0, msg
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	brk 	#0xf000							// 调试断点
    bl 		printf					        // bl 跳转到 printf 子程序

    ldp     x29, x30, [sp]                  // 恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
