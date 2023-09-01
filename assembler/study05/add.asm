	.text
	.globl	add_test
add_test:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]
    
    add     x0, x0, x1

    ldp     x29, x30, [sp]                  // 从start函数栈恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret