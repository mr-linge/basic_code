# ELF 文件格式

段(segment)/节(section)单位存储到elf文件中
代码段(code section): .code 或者 .text
数据段(data section): .data

.bbs         存储未初始化的全局变量和局部静态变量
.data        存储已初始化全局变量和已初始化局部静态变量的位置。
.text        存储指令(局部变量存储在栈空间)

## static

static 修饰的符号作用域都是本地的(local).如果在方法内则符号加前缀,前缀就是方法名.
static 修饰的变量存储位置和全局变量一样, 有值的放在 .data, 未初始化的放在 .bss
