.section	.rodata
msg:
	.asciz	"add(%ld,%ld) = %ld\n"
str1:
	.asciz	"This is a test string\n"

.section 	.text
.global 	main
main:
    adrp    x0, msg
	add		x0, x0, :lo12:msg		        // :lo12:msg 取 msg 低12位 即页内偏移offset,  msg vaddr = page + offset
	mov 	x1, #12
	mov 	x2, #13
	mov		x3, #25
    bl 		printf					        // bl 跳转到 printf 子程序

	adrp    x0,str1
	add		x0, x0, :lo12:str1
	bl 		printf

    mov 	x0, #0
    bl 		exit							// bl 跳转到 exit 子程序

add:
	sub	sp, sp, #16
	str	x0, [sp, #8]
	str	x1, [sp]
	ldr	x8, [sp, #8]
	ldr	x9, [sp]
	add	x0, x8, x9
	add	sp, sp, #16
	ret