	.text
	.file	"main.c"
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             // 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	wzr, [x29, #-4]
	bl	getpid
	mov	w1, w0
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	bl	printf
                                        // kill: def $x8 killed $xzr
	str	xzr, [sp, #16]
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
	mov	w1, #1
	bl	dlopen
	stur	x0, [x29, #-16]
	ldur	x1, [x29, #-16]
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	bl	printf
	ldur	x8, [x29, #-16]
	cbnz	x8, .LBB0_2
	b	.LBB0_1
.LBB0_1:
	adrp	x8, :got:stderr
	ldr	x8, [x8, :got_lo12:stderr]
	ldr	x8, [x8]
	str	x8, [sp]                        // 8-byte Folded Spill
	bl	dlerror
	mov	x2, x0
	ldr	x0, [sp]                        // 8-byte Folded Reload
	adrp	x1, .L.str.3
	add	x1, x1, :lo12:.L.str.3
	bl	fprintf
	mov	w0, #1
	bl	exit
.LBB0_2:
	bl	dlerror
	ldur	x0, [x29, #-16]
	adrp	x1, .L.str.4
	add	x1, x1, :lo12:.L.str.4
	bl	dlsym
	str	x0, [sp, #8]
	bl	dlerror
	str	x0, [sp, #24]
	cbz	x0, .LBB0_4
	b	.LBB0_3
.LBB0_3:
	adrp	x8, :got:stderr
	ldr	x8, [x8, :got_lo12:stderr]
	ldr	x0, [x8]
	ldr	x2, [sp, #24]
	adrp	x1, .L.str.3
	add	x1, x1, :lo12:.L.str.3
	bl	fprintf
	mov	w0, #1
	bl	exit
.LBB0_4:
	ldr	x8, [sp, #8]
	mov	w0, #2
	mov	w1, #7
	blr	x8
	mov	w1, w0
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	bl	printf
	ldur	x0, [x29, #-16]
	bl	dlclose
	mov	w0, wzr
	ldp	x29, x30, [sp, #48]             // 16-byte Folded Reload
	add	sp, sp, #64
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"****************** pid:%d *******************\n"
	.size	.L.str, 47

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"./libcaculate.so"
	.size	.L.str.1, 17

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"handle = %p\n"
	.size	.L.str.2, 13

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"%s\n"
	.size	.L.str.3, 4

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"add"
	.size	.L.str.4, 4

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"add: %d\n"
	.size	.L.str.5, 9

	.ident	"Android (9352603, based on r450784d1) clang version 14.0.7 (https://android.googlesource.com/toolchain/llvm-project 4c603efb0cca074e9238af8b4106c30add4418f6)"
	.section	".note.GNU-stack","",@progbits
