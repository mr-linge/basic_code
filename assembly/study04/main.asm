.section	.rodata
msg:
    .asciz	"add(%lx,%lx) = %lx\n"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x20                   // 获取栈空间
    stp     x29, x30, [sp, #0x10]           // 保存 fp lr
    add	    x29, sp, #0x10                  // fp 保存上一个 fp 的位置

    mov     x0, #0x70
    mov     x1, #0x30
    stp     x0, x1, [sp]

    bl      add_test                        // 跳转到子程序

    ldp     x1, x2, [sp]                    // printf 参数 1 2
    mov     x3, x0                          // printf 参数 3
	adrp	x0, msg		                    // printf 参数 0     要打印的字符串首地址
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
    bl 		printf					        // bl 跳转到 printf 子程序

    ldp     x29, x30, [sp, #0x10]           // 恢复x29,x30寄存器
    add     sp, sp, #0x20
	ret		                                // 函数返回退出

add_test:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]                  // 保存 fp lr
    add	    x29, sp, #0                     // fp 保存上一个 fp 的位置
    
    add     x0, x0, x1

    ldp     x29, x30, [sp]                  // 恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret