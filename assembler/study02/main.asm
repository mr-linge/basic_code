.section	.text
.globl main
main:
    mov     x0, #1      			// fd := STDOUT_FILENO

	adrp	x1, str					// 计算出 page
	add		x1, x1, :lo12:str		// :lo12:str 取 str 低12位 即页内偏移offset,  string vaddr = page + offset
	
    mov     x2, #len    				// string length

    mov     w8, #64     			// write is syscall #64 
    svc     #0          			// invoke syscall 

    // syscall exit(int status) 
    mov     x0, #0      			// status := 0 
    mov     w8, #93     			// exit is syscall #93 
    svc     #0          			// invoke syscall 

.section .data
str:
    .ascii        "hello world!\n"
	len = 14
