# 汇编

.rodata 段定义字符串

.section .rodata
msg:
 .asciz "add(%ld,%ld) = %ld\n"
str1:
 .asciz "./libhook.so\n"

.asciz      声明字符串尾部自动加0, bl printf 打印字多个字符串时互不干扰
