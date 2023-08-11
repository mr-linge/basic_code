	.text
	.file	"caculate.c"
	.globl	add                             // -- Begin function add
	.p2align	2
	.type	add,@function
add:                                    // @add
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	ldr	w8, [sp, #12]
	ldr	w9, [sp, #8]
	add	w0, w8, w9
	add	sp, sp, #16
	ret
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
	.cfi_endproc
                                        // -- End function
	.globl	sub                             // -- Begin function sub
	.p2align	2
	.type	sub,@function
sub:                                    // @sub
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	ldr	w8, [sp, #12]
	ldr	w9, [sp, #8]
	subs	w0, w8, w9
	add	sp, sp, #16
	ret
.Lfunc_end1:
	.size	sub, .Lfunc_end1-sub
	.cfi_endproc
                                        // -- End function
	.globl	mul                             // -- Begin function mul
	.p2align	2
	.type	mul,@function
mul:                                    // @mul
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	ldr	w8, [sp, #12]
	ldr	w9, [sp, #8]
	mul	w0, w8, w9
	add	sp, sp, #16
	ret
.Lfunc_end2:
	.size	mul, .Lfunc_end2-mul
	.cfi_endproc
                                        // -- End function
	.globl	div                             // -- Begin function div
	.p2align	2
	.type	div,@function
div:                                    // @div
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	ldr	w8, [sp, #12]
	ldr	w9, [sp, #8]
	sdiv	w0, w8, w9
	add	sp, sp, #16
	ret
.Lfunc_end3:
	.size	div, .Lfunc_end3-div
	.cfi_endproc
                                        // -- End function
	.ident	"Android (9352603, based on r450784d1) clang version 14.0.7 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)"
	.section	".note.GNU-stack","",@progbits
