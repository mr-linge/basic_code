	.text
	.globl	add_test
add_test:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]                  // 保存 fp lr
    add	    x29, sp, #0                     // fp 保存上一个 fp 的位置

    add     x0, x0, x1

    ldp     x29, x30, [sp]                  // 恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret