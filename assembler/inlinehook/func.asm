	.globl	c_test_func
c_test_func:
	NOP
	//sub x3, x32, x18
//	sub sp, sp, #16
//	str x32, [sp]
//	ldr x4, [sp]
//	add sp, sp, #16
//	br  x18
	sub	sp, sp, #0x10
	adrp x4, c_test_func2
	add	x4, x4, :lo12:c_test_func2
	BR 	#12
	mov x2, #0x1234
	cmp x1, x2
	b.ne l1
	add	x0, x0, #100
l1:
	add	x0, x0, #10
	add	sp, sp, #0x10
	ret

c_test_func2:
	NOP
	NOP
	sub	sp, sp, #0x10
	add	x0, x0, #1000
	add	sp, sp, #0x10
	ret