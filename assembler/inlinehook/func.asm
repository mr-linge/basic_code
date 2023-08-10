	.globl	c_test_func
c_test_func:
	NOP
	sub	sp, sp, #0x10
	mov x2, #0x1234
	cmp x1, x2
	b.ne l1
	add	x0, x0, #100
l1:
	add	x0, x0, #10
	add	sp, sp, #0x10
	ret
