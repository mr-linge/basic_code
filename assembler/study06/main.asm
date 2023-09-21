.section	.rodata
libpath:
	.asciz	"./a.so"

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x20                   // 获取栈空间
    stp     x29, x30, [sp, #0x10]           // 保存 fp lr
    add	    x29, sp, #0x10                  // fp 保存上一个 fp 的位置

//	bl		test1
//	bl		test2
//	bl		test3
	bl		test4

	bl 		getchar

    ldp     x29, x30, [sp, #0x10]          	// 恢复x29,x30寄存器
    add     sp, sp, #0x20
	ret

test1: // dlopen 加载动态库
	sub     sp, sp, #0x20                   // 获取栈空间
    stp     x29, x30, [sp, #0x10]           // 保存 fp lr
    add	    x29, sp, #0x10                  // fp 保存上一个 fp 的位置

	adrp  	x0, libpath
	add		x0, x0, :lo12:libpath
	mov		x1, #0x2
	bl		dlopen							// 跳转到 dlopen

	ldp     x29, x30, [sp, #0x10]          	// 恢复x29,x30寄存器
    add     sp, sp, #0x20
	ret

test2: // dlopen 加载动态库, 在指令中构建数据(在往目标进程注入代码时很有用)
	sub     sp, sp, 0x20                   // 获取栈空间
    stp     x29, x30, [sp, 0x10]           // 保存 fp lr
    add	    x29, sp, 0x10                  // fp 保存上一个 fp 的位置

	sub     sp, sp, 0x100
	ldr		x1, 0x8
	b 		0xc
	.BYTE 	0x2e, 0x2f, 0x61, 0x2e
	.BYTE 	0x73, 0x6f, 0x00, 0x00

	str 	x1, [sp]
	mov		x0, sp
	mov		x1, 0x2
	bl		dlopen
	add     sp, sp, 0x100

	ldp     x29, x30, [sp, #0x10]          	// 恢复x29,x30寄存器
    add     sp, sp, 0x20
	ret

test3: // blr 绝对地址跳转,实现调用 dlopen函数(在往目标进程注入代码时很有用)
	sub     sp, sp, 0x20                   // 获取栈空间
    stp     x29, x30, [sp, 0x10]           // 保存 fp lr
    add	    x29, sp, 0x10                  // fp 保存上一个 fp 的位置

	sub     sp, sp, 0x100
	ldr		x1, 0x8
	b 		0xc
	.BYTE 	0x2e, 0x2f, 0x61, 0x2e
	.BYTE 	0x73, 0x6f, 0x00, 0x00

	str 	x1, [sp]
	mov		x0, sp
	mov		x1, 0x2
	adrp	x3, dlopen
	add		x3, x3, :lo12:dlopen
	blr		x3
	add     sp, sp, 0x100

	ldp     x29, x30, [sp, #0x10]          	// 恢复x29,x30寄存器
    add     sp, sp, 0x20
	ret

test4: // blr 绝对地址跳转,实现调用 dlopen函数(在往目标进程注入代码时很有用)
	sub     sp, sp, 0x20                   // 获取栈空间
    stp     x29, x30, [sp, 0x10]           // 保存 fp lr
    add	    x29, sp, 0x10                  // fp 保存上一个 fp 的位置

	adrp	x3, printf
	add		x3, x3, :lo12:printf

	sub     sp, sp, 0x100
	ldr		x1, 0x8
	b 		0xc
	.BYTE 	0x2e, 0x2f, 0x61, 0x2e
	.BYTE 	0x73, 0x6f, 0x00, 0x00
	str 	x1, [sp]
	mov		x0, sp
	blr		x3
	add     sp, sp, 0x100

	ldp     x29, x30, [sp, #0x10]          	// 恢复x29,x30寄存器
    add     sp, sp, 0x20
	ret
