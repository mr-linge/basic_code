	.text
	.file	"test1.c"
	.globl	func_example                    // -- Begin function func_example
	.p2align	2
	.type	func_example,@function
func_example:                           // @func_example
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	w0, [x29, #-4]
	ldur	w1, [x29, #-4]
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printf
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.Lfunc_end0:
	.size	func_example, .Lfunc_end0-func_example
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, wzr
	str	w8, [sp]                        // 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	mov	w8, #1
	str	w8, [sp, #8]
	adrp	x8, main.s_var_1
	ldr	w8, [x8, :lo12:main.s_var_1]
	adrp	x9, main.s_var_2
	ldr	w9, [x9, :lo12:main.s_var_2]
	add	w8, w8, w9
	ldr	w9, [sp, #8]
	add	w8, w8, w9
	ldr	w9, [sp, #4]
	add	w8, w8, w9
	adrp	x9, d_var
	ldr	w9, [x9, :lo12:d_var]
	add	w8, w8, w9
	adrp	x9, e_var
	ldr	w9, [x9, :lo12:e_var]
	add	w0, w8, w9
	bl	func_example
	ldr	w0, [sp]                        // 4-byte Folded Reload
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        // -- End function
	.type	a_var_global,@object            // @a_var_global
	.data
	.globl	a_var_global
	.p2align	2
a_var_global:
	.word	84                              // 0x54
	.size	a_var_global, 4

	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"example %d\n"
	.size	.L.str, 12

	.type	main.s_var_1,@object            // @main.s_var_1
	.data
	.p2align	2
main.s_var_1:
	.word	85                              // 0x55
	.size	main.s_var_1, 4

	.type	main.s_var_2,@object            // @main.s_var_2
	.local	main.s_var_2
	.comm	main.s_var_2,4,4
	.type	d_var,@object                   // @d_var
	.p2align	2
d_var:
	.word	86                              // 0x56
	.size	d_var, 4

	.type	e_var,@object                   // @e_var
	.local	e_var
	.comm	e_var,4,4
	.type	b_var_global,@object            // @b_var_global
	.bss
	.globl	b_var_global
	.p2align	2
b_var_global:
	.word	0                               // 0x0
	.size	b_var_global, 4

	.type	h_var_global,@object            // @h_var_global
	.globl	h_var_global
	.p2align	2
h_var_global:
	.zero	1024
	.size	h_var_global, 1024

	.ident	"Android (9352603, based on r450784d1) clang version 14.0.7 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)"
	.section	".note.GNU-stack","",@progbits
