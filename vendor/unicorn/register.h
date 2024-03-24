// ARM64 寄存器结构体
struct registers
{
    long long uregs[34];
};
#define x0 uregs[0]  // 存储R0寄存器的值，函数调用后的返回值会存储在R0寄存器中.调用函数时保存第 1 个参数
#define x1 uregs[1]  // 调用函数时保存第 2 个参数
#define x2 uregs[2]  // 调用函数时保存第 3 个参数
#define x3 uregs[3]  // 调用函数时保存第 4 个参数
#define x4 uregs[4]  // 调用函数时保存第 5 个参数
#define x5 uregs[5]  // 调用函数时保存第 6 个参数
#define x6 uregs[6]  // 调用函数时保存第 7 个参数
#define x7 uregs[7]  // 调用函数时保存第 8 个参数
#define fp uregs[29] // X29 is the frame pointer register (FP). 用来定位有效的栈帧记录。帧指针寄存器，存放当前过程调用栈帧的起始地址，可使用FP别名引用
#define lr uregs[30] // 链接寄存器，用于保存过程调用的返回地址，可使用LR别名引用
/*
X30 is the link register (LR). The branch-and-link instructions that store a return address in the link register (BL and BLR), setting the register X30 to PC+4.
Calls to subroutines, where it is necessary for the return address to be stored in the link register(X30).
*/
#define sp uregs[31]   // 栈指针寄存器，指向当前堆栈的栈顶
#define pc uregs[32]   // 存储当前的执行地址
#define cpsr uregs[33] // 存储状态寄存器的值