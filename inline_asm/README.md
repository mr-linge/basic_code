# 内联汇编

## 语法
asm asm-qualifiers(
				Assembler Template;
				:outputOperands
				:inputOperands
				[:Clobbers]
            )

语句构成
asm                 关键字,表明是一个GNU扩展

修饰词(qualifiers)
volatile            关闭编译器优化
inline              内敛,减小汇编代码尺寸
goto                在内嵌汇编里会跳转到C语言的label

输出部:
用于描述在指令部中可以被修改的C语言变量以及约束条件；

输入部:
用来描述在指令部中，只读的C语言变量以及约束条件；
输入部描述的参数，是只读属性，不要修改参数内容，因为GCC编译器假定输入部参数，在内嵌汇编前后是一致的；
输入部中不能使用"=/+"约束条件，编译器会报错；
输入部可以是空的；

损坏部:
“memory”告诉GCC内嵌汇编指令改变了内存的值，强迫编译器在执行该汇编代码前，存储所有缓存的值，在执行完汇编代码之后重新加载该值，目的是防止编译乱序；
“cc”:表示内嵌汇编代码影响状态寄存器相关的标志位；

## 1. Assembler Template
所有汇编代码必须用双引号括起来，如果多行汇编代码，每一条语句都要用双引号括起来，代码后面加上换行符("\n"或"\n\t")，具体形式如下:
asm volatile ( "instruction 1\n\t" 
           "instruction 2\n\t"
           ......
           "last instruction"
           );

## 2. 输出/入参数列表
static void my_memcpy_asm_test(unsigned long src, unsigned long dst,unsigned long counter)
  {
      unsigned long tmp;
      unsigned long end = src+counter;
  
      asm volatile(
              "1: ldr %1, [%2], #8\n"
              "str %1, [%0], #8\n"
              "cmp %2, %3\n"
              "b.cc 1b"
              :"+r"(dst),"+r"(tmp),"+r"(src)   //output list, can write and read
              :"r"(end)                        //input list, readonly
              :"memory"
              );
  }


%0代表第一个操作数，dst变量
%1代表第二个操作数，tmp变量

## 2. 操作数格式
操作数最多由4部分组成:
[name]"modifier+constraint"(C expression)

name:别名，可选；
modifier:修改符，要用双引号括起来；
constraint:限定符，要用双引号括起来；
c表达式:用小括号括起来；

### 2.1 别名
在汇编模板中引用该变量可以使用别名增加可读性,例如:
int x=10, y;
asm volatile("mov %[in],%[out]"
   : [out]"=r"(y)
   : [in]"r"(x)
   :
);

### 2.2 修改符号
修改符	            含义
=	             表示被修饰的操作数具有可写属性
+	             表示被修饰的操作数具有可读可写属性
&	             寄存器只能用于输出(为了避免编译器优化输出和输入使用同一个寄存器，可在输出操作数中使用“&”修改符，明确告诉编译器，代表输出操作数的寄存器一定不能使用输入操作数已经使用过的寄存器)

### 2.3 限定符
操作数在这里的作用是将C语言定义的变量与汇编中使用的变量进行一一对应，但并不是所有的汇编指令都可以接受任何类型变量，因此汇编器需要知道这些变量到底用在什么地方，传递前做一些转换。
常用限定符如下表:
限定符	            ARM指令集含义
r	                通用寄存器
f	                浮点寄存器
m	                内存地址


## 3 修改寄存器列表
为保持寄存器，内存数据一致性，提供三个类型
| 类型| 作用 |
| :-----|:---- |
| r0...r15 | 告诉编译器汇编代码中修改了通用寄存器r0...r15 |
| cc | 告诉编译器汇编代码会导致CPU状态位的改变||
| memory | 告诉编译器汇编代码会读取或修改内存中某个地址存放的值 |
添加“memory”之后，编译器会在执行汇编代码之前，保证将保存在寄存器中，没有更新到内存中的值全部都写入到内存中。
此列表中的每一项都要用双引号（""）括起来，每项之间要用逗号（“,”）分割
