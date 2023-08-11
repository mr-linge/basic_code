	.globl	c_test_func
c_test_func:
	NOP
	adrp	x9, :got:new_c_test_func
	ldr	x9, [x9, :got_lo12:new_c_test_func]
	blr x9
	sub	sp, sp, #0x10
	mov x2, #0x1234
	cmp x1, x2
	b.ne l1
	add	x0, x0, #100
l1:
	add	x0, x0, #10
	add	sp, sp, #0x10
	ret
