	.text
	.file	"main.c"
	.globl	should_not_be_called            // -- Begin function should_not_be_called
	.p2align	2
	.type	should_not_be_called,@function
should_not_be_called:                   // @should_not_be_called
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	adrp	x1, .L__FUNCTION__.should_not_be_called
	add	x1, x1, :lo12:.L__FUNCTION__.should_not_be_called
	bl	printf
	mov	w0, wzr
	bl	exit
.Lfunc_end0:
	.size	should_not_be_called, .Lfunc_end0-should_not_be_called
	.cfi_endproc
                                        // -- End function
	.globl	attack                          // -- Begin function attack
	.p2align	2
	.type	attack,@function
attack:                                 // @attack
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #112
	stp	x29, x30, [sp, #96]             // 16-byte Folded Spill
	add	x29, sp, #96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-8]
	mov	x0, #256
	str	x0, [sp, #8]                    // 8-byte Folded Spill
	bl	malloc
	ldr	x2, [sp, #8]                    // 8-byte Folded Reload
	sub	x8, x29, #16
	str	x8, [sp, #32]                   // 8-byte Folded Spill
	stur	x0, [x29, #-16]
	ldur	x0, [x29, #-16]
	mov	w1, wzr
	bl	memset
	ldur	x1, [x29, #-16]
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	bl	scanf
	ldr	x0, [sp, #32]                   // 8-byte Folded Reload
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	str	x1, [sp, #40]                   // 8-byte Folded Spill
	bl	strsep
	stur	x0, [x29, #-24]
	ldur	x8, [x29, #-8]
	str	x8, [sp, #24]                   // 8-byte Folded Spill
	ldur	x8, [x29, #-24]
	str	x8, [sp, #16]                   // 8-byte Folded Spill
	ldur	x0, [x29, #-24]
	bl	strlen
	ldr	x1, [sp, #16]                   // 8-byte Folded Reload
	mov	x2, x0
	ldr	x0, [sp, #24]                   // 8-byte Folded Reload
	bl	memcpy
	ldr	x0, [sp, #32]                   // 8-byte Folded Reload
	ldr	x1, [sp, #40]                   // 8-byte Folded Reload
	bl	strsep
	stur	x0, [x29, #-32]
	ldur	x8, [x29, #-32]
	cbz	x8, .LBB1_2
	b	.LBB1_1
.LBB1_1:
	stur	xzr, [x29, #-40]
	ldur	x0, [x29, #-32]
	mov	x1, xzr
	mov	w2, #16
	bl	strtoul
	stur	x0, [x29, #-40]
	ldur	x8, [x29, #-8]
	str	x8, [sp]                        // 8-byte Folded Spill
	ldur	x0, [x29, #-24]
	bl	strlen
	ldr	x9, [sp]                        // 8-byte Folded Reload
	ldur	x8, [x29, #-40]
	str	x8, [x9, x0]
	b	.LBB1_2
.LBB1_2:
	str	xzr, [sp, #48]
	b	.LBB1_3
.LBB1_3:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #48]
	subs	x8, x8, #256
	b.hs	.LBB1_10
	b	.LBB1_4
.LBB1_4:                                //   in Loop: Header=BB1_3 Depth=1
	ldur	x8, [x29, #-8]
	ldr	x9, [sp, #48]
	ldrb	w1, [x8, x9]
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
	ldr	x8, [sp, #48]
	add	x8, x8, #1
	mov	x10, #8
	udiv	x9, x8, x10
	mul	x9, x9, x10
	subs	x8, x8, x9
	cbnz	x8, .LBB1_6
	b	.LBB1_5
.LBB1_5:                                //   in Loop: Header=BB1_3 Depth=1
	adrp	x0, .L.str.4
	add	x0, x0, :lo12:.L.str.4
	bl	printf
	b	.LBB1_6
.LBB1_6:                                //   in Loop: Header=BB1_3 Depth=1
	ldr	x8, [sp, #48]
	add	x8, x8, #1
	mov	x10, #16
	udiv	x9, x8, x10
	mul	x9, x9, x10
	subs	x8, x8, x9
	cbnz	x8, .LBB1_8
	b	.LBB1_7
.LBB1_7:                                //   in Loop: Header=BB1_3 Depth=1
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	bl	puts
	b	.LBB1_8
.LBB1_8:                                //   in Loop: Header=BB1_3 Depth=1
	b	.LBB1_9
.LBB1_9:                                //   in Loop: Header=BB1_3 Depth=1
	ldr	x8, [sp, #48]
	add	x8, x8, #1
	str	x8, [sp, #48]
	b	.LBB1_3
.LBB1_10:
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	bl	puts
	ldur	x0, [x29, #-16]
	bl	free
	ldp	x29, x30, [sp, #96]             // 16-byte Folded Reload
	add	sp, sp, #112
	ret
.Lfunc_end1:
	.size	attack, .Lfunc_end1-attack
	.cfi_endproc
                                        // -- End function
	.globl	normal_call                     // -- Begin function normal_call
	.p2align	2
	.type	normal_call,@function
normal_call:                            // @normal_call
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             // 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	x0, sp
	str	xzr, [sp]
	str	xzr, [sp, #8]
	bl	attack
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	adrp	x1, .L__FUNCTION__.normal_call
	add	x1, x1, :lo12:.L__FUNCTION__.normal_call
	bl	printf
	ldp	x29, x30, [sp, #16]             // 16-byte Folded Reload
	add	sp, sp, #32
	ret
.Lfunc_end2:
	.size	normal_call, .Lfunc_end2-normal_call
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, wzr
	str	w8, [sp, #12]                   // 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	stur	w0, [x29, #-8]
	str	x1, [sp, #16]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	adrp	x1, main
	add	x1, x1, :lo12:main
	bl	printf
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	adrp	x1, should_not_be_called
	add	x1, x1, :lo12:should_not_be_called
	bl	printf
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	adrp	x1, normal_call
	add	x1, x1, :lo12:normal_call
	bl	printf
	bl	normal_call
	ldr	w0, [sp, #12]                   // 4-byte Folded Reload
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	add	sp, sp, #48
	ret
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%s is running ...\n"
	.size	.L.str, 19

	.type	.L__FUNCTION__.should_not_be_called,@object // @__FUNCTION__.should_not_be_called
.L__FUNCTION__.should_not_be_called:
	.asciz	"should_not_be_called"
	.size	.L__FUNCTION__.should_not_be_called, 21

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"%s"
	.size	.L.str.1, 3

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"|"
	.size	.L.str.2, 2

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"%02X "
	.size	.L.str.3, 6

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"\t"
	.size	.L.str.4, 2

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.zero	1
	.size	.L.str.5, 1

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"%s is over ...\n"
	.size	.L.str.6, 16

	.type	.L__FUNCTION__.normal_call,@object // @__FUNCTION__.normal_call
.L__FUNCTION__.normal_call:
	.asciz	"normal_call"
	.size	.L__FUNCTION__.normal_call, 12

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"main                    vaddr:%p\n"
	.size	.L.str.7, 34

	.type	.L.str.8,@object                // @.str.8
.L.str.8:
	.asciz	"should_not_be_called    vaddr:%p\n"
	.size	.L.str.8, 34

	.type	.L.str.9,@object                // @.str.9
.L.str.9:
	.asciz	"normal_call             vaddr:%p\n"
	.size	.L.str.9, 34

	.ident	"Android (9352603, based on r450784d1) clang version 14.0.7 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)"
	.section	".note.GNU-stack","",@progbits
