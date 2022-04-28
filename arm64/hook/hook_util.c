#include "hook_util.h"


typedef int (*CALLBACK)(char *func_name, unsigned long func_vaddr, long long *argv);

// 采用信号控制当前程序的流程
static int signal_command;

void sighandler(int signum)
{
    printf("捕获信号 %d, 等待子进程完成当前任务后,父进程会自动结束...\n", signum);
    signal_command = signum;
}

// 下断点并保存此位置原来数据 (aarch64 往目标地址写入异常指令)
int set_breakpoint(pid_t pid, unsigned long addr, union OneInstruction *oneInstruction)
{
    // save context 保存断点处数据
    getdata(pid, addr, oneInstruction->bytes, 4);

    // 下断点
    union OneInstruction illegal_instruction;
    // aarch64 illegal instruction: 0xe7fXXXfX
    illegal_instruction.instruction = 0xe7f000f0;
    putdata(pid, addr, illegal_instruction.bytes, 4);

    return 0;
}

// 等待程序运行到 指定地址的断点处,并返回断点地址
long long wait_breakpoint(pid_t pid, struct pt_regs *regs)
{
    int status;
    waitpid(pid, &status, WUNTRACED);
    // printf("status:%d\n", status);
    if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGILL)
    {
        // save context 保存断点处寄存器信息
        get_registers(pid, regs);

        return 0;
    }

    return -1;
}

// 获取到函数参数
int get_func_params(pid_t pid, struct pt_regs regs, long long *params, int num_params)
{
    int i;
    for (i = 0; i < num_params && i < 8; i++)
    {
        params[i] = regs.uregs[i];
    }
    if (i < num_params)
    {
        long long current_sp = regs.ARM_sp;
        // printf("current_sp = 0x%llx\n", current_sp);
        union
        {
            long long val[0x100];
            uint8_t bytes[0x100 * sizeof(long long)];
        } data;

        getdata(pid, current_sp, data.bytes, ((num_params - 8) * sizeof(long long)));
        for (int j = 0; j < (num_params - 8); j++)
        {
            params[i + j] = data.val[j];
        }
    }

    // for (int k = 0; k < num_params; k++)
    // {
    // 	printf("param[%d] = 0x%llx\n", k, params[k]);
    // }

    return 0;
}

// 重新设置函数参数
int set_func_params(pid_t pid, struct pt_regs *regs, long long *params, int num_params)
{
    int i = 0;
    // aarch64处理器，函数传递参数，将前 8 个参数放到r0-r7，剩下的参数压入栈中
    for (i = 0; i < num_params && i < 8; i++)
    {
        regs->uregs[i] = params[i];
    }
    set_registers(pid, regs); // 将更改后的参㶼写入寄存器
    if (i < num_params)
    {
        long long current_sp = regs->ARM_sp;
        union
        {
            long long val[0x100];
            uint8_t bytes[0x100 * sizeof(long long)];
        } data;
        for (int j = 0; j < (num_params - 8); j++)
        {
            data.val[j] = params[i + j];
        }

        putdata(pid, (unsigned long)current_sp, data.bytes, (num_params - i) * sizeof(long long));
    }
    return 0;
}

void handle_breakpoint(pid_t pid, struct Hook_point_info *hook_points, int hook_point_count, unsigned long enter_call_back_vaddr, unsigned long leave_call_back_vaddr)
{
    ptrace_attach(pid);

    for (size_t i = 0; i < hook_point_count; i++)
    {
        // 把函数入口处设成断点
        set_breakpoint(pid, hook_points[i].entry_addr, &(hook_points[i].entry_point_code));
    }

    // 运行程序
    ptrace_cont(pid);
    // 等待断点
    struct pt_regs regs;
loop:
    wait_breakpoint(pid, &regs);
    unsigned long pc = regs.ARM_pc;

    for (size_t i = 0; i < hook_point_count; i++)
    {
        // 如果断点是函数入口
        if (pc == hook_points[i].entry_addr)
        {
            // 获取函数传进来的参数
            long long old_params[hook_points[i].num_params];
            get_func_params(pid, regs, old_params, hook_points[i].num_params);
            // printf("%s %d\n",__FUNCTION__,__LINE__);
            // ONENTER onEnter = NULL;
            CALLBACK onEnter = (CALLBACK)enter_call_back_vaddr;
            onEnter(hook_points[i].func_name, hook_points[i].entry_addr, old_params);
            // 重新设置函数参数
            set_func_params(pid, &regs, old_params, hook_points[i].num_params);

            // 保存函数相关信息
            memcpy(&(hook_points[i].entry_regs), &regs, sizeof(struct pt_regs));
            hook_points[i].back_addr = regs.ARM_lr;

            // 移除断点，即恢复程序上下文信息
            putdata(pid, hook_points[i].entry_addr, hook_points[i].entry_point_code.bytes, 4);
            // set_registers(pid, &(hook_point->entry_regs));

            //并把函数返回地址设置成断点，便于获取函数返回值
            set_breakpoint(pid, hook_points[i].back_addr, &(hook_points[i].back_point_code));
        }

        if (pc == hook_points[i].back_addr)
        {
            get_registers(pid, &regs);
            long long func_return_value = regs.ARM_r0;
            CALLBACK onLeave = (CALLBACK)leave_call_back_vaddr;
            onLeave(hook_points[i].func_name, hook_points[i].entry_addr, &func_return_value);
            // 将更改后的返回值 写入寄存器
            regs.ARM_r0 = func_return_value;
            set_registers(pid, &regs);

            // 保存函数相关信息
            memcpy(&(hook_points[i].back_regs), &regs, sizeof(struct pt_regs));

            // 删除断点，即恢复程序上下文信息
            putdata(pid, hook_points[i].back_addr, hook_points[i].back_point_code.bytes, 4);

            // 并把函数入口处设成断点
            set_breakpoint(pid, hook_points[i].entry_addr, &(hook_points[i].entry_point_code));
        }
    }

    if (signal_command == 0)
    {
        // 运行程序
        ptrace_cont(pid);
        goto loop;
    }

    for (size_t i = 0; i < hook_point_count; i++)
    {
        // 移除断点，即恢复程序上下文信息
        putdata(pid, hook_points[i].entry_addr, hook_points[i].entry_point_code.bytes, 4);
        // 删除断点，即恢复程序上下文信息
        putdata(pid, hook_points[i].back_addr, hook_points[i].back_point_code.bytes, 4);
    }

    // 运行程序
    ptrace_cont(pid);
    ptrace_detach(pid);
}

int hook(pid_t pid, struct Hooked_func *funcs, int func_count, unsigned long enter_call_back_vaddr, unsigned long leave_call_back_vaddr)
{
    struct Hook_point_info hook_points[0x100];

    for (int i = 0; i < func_count; i++)
    {
        struct Hooked_func func = funcs[i];

        struct Hook_point_info hook_point;
        memcpy(hook_point.func_name, func.func_name, strlen(func.func_name)); // name
        hook_point.num_params = func.num_params;                              // param count

        if (func.func_vaddr != 0)
        {
            hook_point.entry_addr = func.func_vaddr;
        }
        else
        {
            int bind = STB_GLOBAL;
            int type = STT_FUNC;
            unsigned long vaddr = get_vaddr(pid, func.func_name, func.module_path, bind, type);
            if (vaddr != 0)
            {
                hook_point.entry_addr = vaddr;
            }
            else
            {
                printf("%s 在模块 %s 中没能查找到对应的地址,如果想hook 这个函数需要自己去查找函数地址，然后输入对应的地址\n", func.func_name, func.module_path);
                exit(-1);
            }
        }

        hook_points[i] = hook_point;
    }

    handle_breakpoint(pid, hook_points, func_count, enter_call_back_vaddr, leave_call_back_vaddr);

    return 0;
}