#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#import <mach/mach.h>

#define PAGESIZE 0x4000
#define PAGE_START(addr) ((addr) & (~(PAGESIZE - 1)))

void log_matchine_code(void *vaddr, unsigned long len)
{
    unsigned char *machine_codes = (unsigned char *)vaddr;
    for (unsigned long i = 0; i < len; i++)
    {
        printf("%02x ", (int)machine_codes[i]);
        if ((i + 1) % 4 == 0)
        {
            puts("");
        }
    }
}

int c_test_func(int num)
{
    return num + 10;
}

kern_return_t mach_vm_remap(vm_map_t, mach_vm_address_t *, mach_vm_size_t, mach_vm_offset_t, int, vm_map_t, mach_vm_address_t, boolean_t, vm_prot_t *, vm_prot_t *, vm_inherit_t);

void modify_text_segment(unsigned long target_vaddr) {
    int err,ret;
    kern_return_t kr;

    unsigned long page_vaddr = PAGE_START(target_vaddr);   // 页地址
    unsigned long page_offset = target_vaddr - page_vaddr; // 页内偏移
    printf("page_vaddr: 0x%lx, page_offset:0x%lx\n", page_vaddr, page_offset);

    mach_port_t task_self = mach_task_self();
    printf("port got: %d\n", task_self);
    // 1. 在 heap 里申请一段内存 大小为 PAGESIZE
    void *new_page_vaddr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    printf("new_page_vaddr:%p\n",new_page_vaddr);
    if (new_page_vaddr == MAP_FAILED) /* 判断是否映射成功 */
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

    // 2. 把 目标页 数据复制到 新页
    // kern_return_t kr = vm_copy(task_self, page_vaddr, PAGESIZE, (vm_address_t) new_page_vaddr);
    // if (!kr) { 
    //     printf("vm_copy ok\n");
    // } else {
    //     fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
    //     exit(1);
    // }
    // 或
    memcpy(new_page_vaddr, (void *)page_vaddr, PAGESIZE);

    // 3. 更改机器码
    unsigned char jump_code[0x8] = {
        0x52, 0x00, 0x00, 0x58,
        0x40, 0x02, 0x1F, 0xD6};
    unsigned long new_target_vaddr = (unsigned long)new_page_vaddr + page_offset;
    printf("new_target_vaddr:0x%lx\n",new_target_vaddr);
    memcpy((void *)new_target_vaddr, jump_code, 8);

    // 4. 新页的权限改为 r-x
    err = mprotect(new_page_vaddr, PAGESIZE, PROT_READ | PROT_EXEC);
    if (err != 0)
	{
		fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}

    // 5. 用 新页 替换 旧页
    vm_prot_t c, m;
    vm_inherit_t inherit;
    mach_vm_address_t target = page_vaddr;
    kr = mach_vm_remap(mach_task_self(), &target, PAGESIZE, 0, VM_FLAGS_OVERWRITE, task_self, (mach_vm_address_t) new_page_vaddr, TRUE, &c, &m, inherit);
    if(kr != KERN_SUCCESS){
        fprintf(stderr, "%s:%d error: %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    log_matchine_code((void *)c_test_func,0x20);
    modify_text_segment((unsigned long)c_test_func);
    log_matchine_code((void *)c_test_func,0x20);

    return 0;
}
