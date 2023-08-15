#include <unicorn/unicorn.h>
#include "register.h"

#define ADDRESS 0x10000                // 开始执行的地址
#define MEMORY_SIZE 0x400 * 0x400 * 2  // 内存 2M
#define STACK 0x80000000 + 0x10000 * 6 // 栈

int main()
{
   // 待运行的机器码
   unsigned char machine_code[] = {0x20, 0x00, 0x02, 0x8B, 0x00, 0x00, 0x03, 0xCB};

   struct registers regs;
   regs.x0 = 0x0;
   regs.x1 = 0x10;
   regs.x2 = 0x50;
   regs.x3 = 0x40;

   uc_engine *uc;
   uc_err err;

   /*
   Create new instance of unicorn engine.

   @arch: architecture type (UC_ARCH_*)
   @mode: hardware mode. This is combined of UC_MODE_*
   @uc: pointer to uc_engine, which will be updated at return time

   @return UC_ERR_OK on success, or other value on failure (refer to uc_err enum
   for detailed error).
   */
   err = uc_open(UC_ARCH_ARM64, UC_MODE_ARM, &uc);
   if (err != UC_ERR_OK)
   {
      printf("Failed to open with error returned:%u\n", err);
      return 0;
   }
   /*
    Map memory in for emulation.
    This API adds a memory region that can be used by emulation.

    @uc: handle returned by uc_open()
    @address: starting address of the new memory region to be mapped in.
       This address must be aligned to 4KB, or this will return with UC_ERR_ARG
    error.
    @size: size of the new memory region to be mapped in.
       This size must be a multiple of 4KB, or this will return with UC_ERR_ARG
    error.
    @perms: Permissions for the newly mapped region.
       This must be some combination of UC_PROT_READ | UC_PROT_WRITE |
    UC_PROT_EXEC, or this will return with UC_ERR_ARG error.

    @return UC_ERR_OK on success, or other value on failure (refer to uc_err enum
      for detailed error).
   */
   // UNICORN_EXPORT uc_err uc_mem_map(uc_engine *uc, uint64_t address, size_t size, uint32_t perms);
   uc_mem_map(uc, ADDRESS, MEMORY_SIZE, UC_PROT_ALL);

   /*
    Write to a range of bytes in memory.

    @uc: handle returned by uc_open()
    @address: starting memory address of bytes to set.
    @bytes:   pointer to a variable containing data to be written to memory.
    @size:   size of memory to write to.

    NOTE: @bytes must be big enough to contain @size bytes.

    @return UC_ERR_OK on success, or other value on failure (refer to uc_err enum
      for detailed error).
   */
   if (uc_mem_write(uc, ADDRESS, machine_code, sizeof(machine_code)))
   {
      printf("Failed to write code to memory\n");
      return 0;
   }

   // 往寄存器中写入数据
   uc_reg_write(uc, UC_ARM64_REG_X1, &regs.x1);
   uc_reg_write(uc, UC_ARM64_REG_X2, &regs.x2);
   uc_reg_write(uc, UC_ARM64_REG_X3, &regs.x3);

   /*
    Emulate machine code in a specific duration of time.

    @uc: handle returned by uc_open()
    @begin: address where emulation starts
    @until: address where emulation stops (i.e. when this address is hit)
    @timeout: duration to emulate the code (in microseconds). When this value is 0,
           we will emulate the code in infinite time, until the code is finished.
    @count: the number of instructions to be emulated. When this value is 0,
           we will emulate all the code available, until the code is finished.

    NOTE: The internal states of the engine is guranteed to be correct if and only
          if uc_emu_start returns without any errors or errors have been handled in
          the callbacks.

    @return UC_ERR_OK on success, or other value on failure (refer to uc_err enum
      for detailed error).
   */
   err = uc_emu_start(uc, ADDRESS, ADDRESS + sizeof(machine_code), 0, 0);
   if (err)
   {
      printf("Failed to uc_emu_start with error returned %u:%s\n", err, uc_strerror(err));
      return 0;
   }

   // 读取寄存器中的数据
   uc_reg_read(uc, UC_ARM64_REG_X0, &regs.x0);
   uc_reg_read(uc, UC_ARM64_REG_X1, &regs.x1);
   uc_reg_read(uc, UC_ARM64_REG_X2, &regs.x2);
   uc_reg_read(uc, UC_ARM64_REG_X3, &regs.x3);
   printf("x0 = 0x%llx\n", regs.x0);
   printf("x1 = 0x%llx\n", regs.x1);
   printf("x2 = 0x%llx\n", regs.x2);
   printf("x3 = 0x%llx\n", regs.x3);

   // 关闭uc类,申请了堆空间来运行的，所以一定要调用这个函数来关闭，否则会内存泄露
   uc_close(uc);

   return 0;
}
