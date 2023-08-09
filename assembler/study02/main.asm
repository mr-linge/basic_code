.section	.rodata
msg:
    .ascii	"hello world!\n"

.section 	.text
.global 	main
main:
	adrp	x0, msg					// 计算出 page
	add		x0, x0, :lo12:msg		// :lo12:msg 取 msg 低12位 即页内偏移offset,  msging vaddr = page + offset
    bl 		printf					// bl 跳转到 printf 子程序
 
    mov 	x0, #0
    bl 		exit							// bl 跳转到 exit 子程序
