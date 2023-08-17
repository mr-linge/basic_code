.section	.rodata
msg:
    .ascii	"add(%lx,%lx) = %lx\n"

.section 	.text
.global 	main
main:
    mov     x0, #0x70
    mov     x1, #0x30

    sub     sp, sp, #0x10                   // 获取栈空间
	stp     x0, x1, [sp]                    // 入栈

    bl      add_test                        // 跳转到子程序

    mov     x3, x0                          // printf 参数 3
    ldp     x1, x2, [sp]                    // printf 参数 1 2
	adrp	x0, msg		                    // printf 参数 0     要打印的字符串首地址
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
    bl 		printf					        // bl 跳转到 printf 子程序

    add     sp, sp, #0x10                   // 恢复栈空间

    mov 	x0, #0
    bl 		exit							// bl 跳转到 exit 子程序

add_test:
	sub	sp, sp, #16
	str	x0, [sp, #8]
	str	x1, [sp]
	ldr	x8, [sp, #8]
	ldr	x9, [sp]
	add	x0, x8, x9
	add	sp, sp, #16
	ret