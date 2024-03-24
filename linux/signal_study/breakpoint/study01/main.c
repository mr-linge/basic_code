#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>

#define PAGESIZE 0x1000
#define PAGE_START(addr) ((addr) & (~(PAGESIZE - 1)))

unsigned long breakpoint_vaddr = 0;

// 在内存中设置断点
void set_point()
{
   asm volatile(
       "NOP   \n"
       "NOP   \n"
       "NOP   \n"
       "NOP   \n"
       "ADD    %[vaddr_out], x29, #0  \n"
       "brk 	#0xf000  \n"
       "NOP   \n"
       "NOP   \n"
       "NOP   \n"
       : [vaddr_out] "=r"(breakpoint_vaddr)
       :
       :);
   printf("%s:%d finish\n", __FUNCTION__, __LINE__);
}

// 删除断点,恢复程序执行
void del_breakpoint(unsigned long addr)
{
   unsigned long target_vaddr_page_start = PAGE_START(addr);
   printf("target_vaddr_page_startt:0x%lx\n", target_vaddr_page_start);
   int err = mprotect((void *)target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }
   char machine_code[4] = {0x1F, 0x20, 0x03, 0xD5};
   memcpy((void *)breakpoint_vaddr, machine_code, 4);

   // 更改内存权限为r-x
   err = mprotect((void *)target_vaddr_page_start, PAGESIZE, PROT_READ | PROT_EXEC);
   if (err != 0)
   {
      fprintf(stderr, "%s:%d mprotect error: %s\n", __FILE__, __LINE__, strerror(errno));
      exit(1);
   }
}

// 捕获断点信号
void sighandler(int signum)
{
   printf("捕获到断点 %d\n", signum);
   del_breakpoint(breakpoint_vaddr);
}

// 在内存中搜索断点指令
void lookup_breakpoint_vaddr(unsigned long start_vaddr)
{
   char needle[4] = {0x00, 0x00, 0x3E, 0xD4};
   void *target_vaddr = memmem((void *)start_vaddr, BUFSIZ, needle, 4);
   breakpoint_vaddr = (unsigned long)target_vaddr;
   printf("%s:%d breakpoint_vaddr:0x%lx\n", __FUNCTION__, __LINE__, breakpoint_vaddr);
}

int main()
{
   signal(SIGTRAP, sighandler);
   lookup_breakpoint_vaddr((unsigned long)&set_point);
   set_point();
   return 0;
}
