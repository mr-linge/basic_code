# AArch64 汇编

## NDK llvm 编译器路径

mac 下NDK安装路径
export PATH=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/darwin-x86_64/bin:${PATH}
linux 下NDK安装路径
export PATH=~/Library/Android/Sdk/ndk/25.2.9519653/toolchains/llvm/prebuilt/linux-x86_64/bin:${PATH}

## 与段相关的伪操作

.section:表示接下来的汇编会链接到哪个段里,例如代码段,数据段等
每一个段以段名为开始, 以下一个段名或者文件结尾为结束
.section name, "flags" 后面可以添加flags,表示段的属性。

.text           代码段
.data           初始化数据段
.bss            未初始化数据段

## 数据定义伪指令

.byte:                  把8位数当成数据插入到汇编中
.hword:                 把16位数当成数据插入到汇编中
.long 和 .int:          把32位数当成数据插入到汇编中
.quad:                  把64位数当成数据插入到汇编中
.float:                 把浮点数当成数据插入到汇编中
.ascii "string" ->      把string当作数据插入到汇编中,ascii伪操作定义的字符串需要自行添加结尾字符'\0'。
.asciz "string" ->      类似ascii,在string后面插入一个结尾字符’\0’。
.rept:                  重复定义, 比如重复3次

## 对齐伪指令

.align 对齐,填充数据来实现对齐。可以填充0或者使用nop指令
告诉汇编程序, align后面的汇编必须从下一个能被 2^n 整除的地址开始分配

.align n        表示 2^n 大小

## 函数相关的伪操作

.global:            定义一个全局符号,通常是为ld使用
.include:           包含指定的头文件, 可以把一个汇编常量定义放在头文件中
