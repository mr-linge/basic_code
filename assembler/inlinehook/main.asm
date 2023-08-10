	.text
	.file	"main.c"
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             // 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	x8, x1
	adrp	x9, :got:c_test_func
	ldr	x9, [x9, :got_lo12:c_test_func]
	str	x9, [sp, #16]                   // 8-byte Folded Spill
	mov	w1, wzr
	stur	wzr, [x29, #-4]
	stur	w0, [x29, #-8]
	stur	x8, [x29, #-16]
	adrp	x8, .L.str
	add	x8, x8, :lo12:.L.str
	stur	x8, [x29, #-24]
	ldur	x0, [x29, #-24]
	bl	access
	cbnz	w0, .LBB0_4
	b	.LBB0_1
.LBB0_1:
	ldur	x0, [x29, #-24]
	mov	w1, #2
	bl	dlopen
	str	x0, [sp, #32]
	ldr	x8, [sp, #32]
	cbnz	x8, .LBB0_3
	b	.LBB0_2
.LBB0_2:
	adrp	x8, :got:stderr
	ldr	x8, [x8, :got_lo12:stderr]
	ldr	x8, [x8]
	str	x8, [sp, #8]                    // 8-byte Folded Spill
	bl	dlerror
	mov	x2, x0
	ldr	x0, [sp, #8]                    // 8-byte Folded Reload
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	bl	fprintf
	b	.LBB0_3
.LBB0_3:
	b	.LBB0_5
.LBB0_4:
	ldur	x1, [x29, #-24]
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	bl	printf
	b	.LBB0_5
.LBB0_5:
	bl	getchar
	ldr	x1, [sp, #16]                   // 8-byte Folded Reload
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
	mov	w0, #10
	bl	c_test_func
	str	w0, [sp, #28]
	ldr	w1, [sp, #28]
	adrp	x0, .L.str.4
	add	x0, x0, :lo12:.L.str.4
	bl	printf
	bl	getchar
	mov	w0, wzr
	ldp	x29, x30, [sp, #64]             // 16-byte Folded Reload
	add	sp, sp, #80
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"./libhook.so"
	.size	.L.str, 13

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"%s\n"
	.size	.L.str.1, 4

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"%s is not exist\n"
	.size	.L.str.2, 17

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"c_test_func vaddr:0x%lx\n"
	.size	.L.str.3, 25

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"c_test_func ret:%d\n"
	.size	.L.str.4, 20

	.ident	"Android (9352603, based on r450784d1) clang version 14.0.7 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)"
	.section	".note.GNU-stack","",@progbits
