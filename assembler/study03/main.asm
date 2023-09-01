.section	.rodata     //  定义只读数据段
msg:
    .asciz  "%lx + %lx = %lx\n"
msg1:
    .asciz  "add(%lx + %lx) = %lx\n"

.section    .data       //  定义数据段
.align 3		        //  以 2^3 字节对齐
number:		            //  定义一个数据
	.quad   0x11
	.quad   0x22
	.quad   0x33
	.quad   0x44
	.quad   0x55
	.quad   0x66

.section 	.text
.global 	main
main:
    sub     sp, sp, #0x10
    stp     x29, x30, [sp]

    adrp    x7, number
    add	    x7, x7, :lo12:number

    ldr     x1, [x7]
    ldr     x2, [x7, #8]
    add     x3, x1, x2

    // 测试一下修改数据
    str     x3, [x7, #16]
    ldr     x3, [x7, #16]

    adrp    x0, msg
    add		x0, x0, :lo12:msg
    bl 		printf

    ldp     x29, x30, [sp]              // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
