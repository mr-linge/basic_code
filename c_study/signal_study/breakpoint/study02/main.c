#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PAGESIZE 0x1000
#define PAGE_START(addr) ((addr) & (~(PAGESIZE - 1)))

// union Instruction
// {
//    uint32_t val;
//    uint8_t bytes[4];
// };

// 断点机器码
char break_machine_code[4] = {0x00, 0x00, 0x3E, 0xD4};

struct BreakPoint
{
   unsigned long vaddr;
   unsigned char machine_code[4];
};

// 所有断点信息保存在这里
struct BreakPoint breakpoints[0x1000];

// 定义未决信号集(pending)
sigset_t pset;
// 定义阻塞信号集(block)
sigset_t bset;
// unsigned long breakpoint_vaddr = 0;

// 在内存中设置断点
void test_func();

// 设置断点
void set_breakpoint(unsigned long addr)
{
   unsigned long page_start = PAGE_START(addr);
   printf("%s:%d 0x%lx page start 0x%lx\n", __FUNCTION__, __LINE__, addr, page_start);
   int err = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }

   // 保存断点处指令
   breakpoints[0].vaddr = addr;
   memcpy(breakpoints[0].machine_code, (void *)addr, 4);
   // 设置成断点指令
   memcpy((void *)addr, break_machine_code, 4);

   // 更改内存权限为r-x
   err = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }
}

// 删除断点,恢复程序执行
void del_breakpoint(unsigned long addr)
{
   unsigned long page_start = PAGE_START(addr);
   printf("%s:%d 0x%lx page start 0x%lx\n", __FUNCTION__, __LINE__, addr, page_start);
   int err = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }

   memcpy((void *)addr, breakpoints[0].machine_code, 4);

   // 更改内存权限为r-x
   err = mprotect((void *)page_start, PAGESIZE, PROT_READ | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }
}

// 捕获断点信号
void sighandler(int signum)
{
   // sigprocmask(SIG_SETMASK, &pset, NULL); // 取消阻塞

   printf("捕获到断点 %d\n", signum);
   del_breakpoint(breakpoints[0].vaddr);

   // // 进行位或操作，将信号集bset更新到进程控制块PCB结构中，阻塞信号SIGINT（即使用户按下ctrl+c，信号也不会递达）
   // sigprocmask(SIG_BLOCK, &bset, NULL);
}

int main()
{
   // 定义未决信号集(pending)
   // sigset_t pset;
   // // 定义阻塞信号集(block)
   // sigset_t bset;
   // // 清空信号集
   // sigemptyset(&bset);
   // // 将信号SIGTRAP加入到信号集中
   // sigaddset(&bset, SIGTRAP);

   // 清空信号集
   sigemptyset(&bset);
   // 将信号SIGTRAP加入到信号集中
   sigaddset(&bset, SIGTRAP);

   signal(SIGTRAP, sighandler);

   // 进行位或操作，将信号集bset更新到进程控制块PCB结构中，阻塞信号SIGINT（即使用户按下ctrl+c，信号也不会递达）
   // sigprocmask(SIG_BLOCK, &bset, NULL);

   // 在内存中搜索断点指令
   uint8_t target_code[4] = {0x01, 0x00, 0x00, 0xD4};
   void *target_vaddr = memmem((void *)test_func, 0x4000, target_code, 4);
   set_breakpoint((unsigned long)target_vaddr);

   test_func();
   return 0;
}
