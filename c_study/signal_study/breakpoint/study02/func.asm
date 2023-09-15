.section	.rodata
msg:
	.ascii	"hello world!\n"
	len = 14

.section	.text
.globl	    test_func
test_func:
    sub     sp, sp, #0x10                   // 获取栈空间
    stp     x29, x30, [sp]                  // 保存 fp lr
    add	    x29, sp, #0                     // fp 保存上一个 fp 的位置

    mov     x0, #1      			        // fd := STDOUT_FILENO

	adrp	x1, msg					        // 计算出 page
	add		x1, x1, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset

    mov     x2, #len    			        // msg length

    mov     w8, #64     			        // write is syscall #64 
    svc     #0          			        // invoke syscall 

    ldp     x29, x30, [sp]                  // 恢复x29,x30寄存器
    add     sp, sp, #0x10
	ret
