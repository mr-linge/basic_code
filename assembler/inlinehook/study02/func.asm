.section	.rodata
msg:
	.asciz	"x(%ld)=0x%lx\n"
str1:
	.asciz	"This is a test string\n"

.section 	.text
.globl		c_test_func
c_test_func:
	NOP
	NOP
	NOP
	NOP
	sub sp, sp, #0x20
	stp x29, x30, [sp,#0x10] // 保存 fp,lr
	stp x0, x1, [sp]

	adrp x0,	msg
	add  x0, x0, :lo12:msg
	mov  x1, #18
	mov  x2, x18
	bl printf

	ldp x0, x1, [sp]
	
	bl new2_c_test_func

	ldr x1, [sp, #8]

	cmp x1, #0x123
	b.ne l1
	add	x0, x0, #100
l1:
	add	x0, x0, #10
	ldp x29, x30, [sp,#0x10]
	add sp, sp, #0x20
	ret