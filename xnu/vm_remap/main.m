#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#import <mach-o/dyld.h>
#import <mach/mach.h>

//因为新分配的虚拟内存是以页为单位的,所以要被映射的内存也要页对齐,所以这里的函数起始地址是以页为单位对齐的。
int __attribute__ ((aligned (PAGE_MAX_SIZE))) testfn(int a, int b)
{
    int c = a + b;
    return c;
}

int main(int argc, char *argv[])
{
    //通过vm_alloc以页为单位分配出一块虚拟内存。
    vm_size_t page_size = 0;
    host_page_size(mach_host_self(), &page_size);  //获取一页虚拟内存的尺寸
    printf("page_size:0x%lx\n",page_size);
    vm_address_t addr = 0;
    //在当前进程内的空闲区域中分配出一页虚拟内存出来,addr指向虚拟内存的开始位置。
    kern_return_t ret = vm_allocate(mach_task_self(), &addr, page_size, VM_FLAGS_ANYWHERE);
    if (ret == KERN_SUCCESS)
    {
        //addr被分配出来后,我们可以对这块内存进行读写操作
        memcpy((void*)addr, "Hello World!\n", 14);
        printf("%s\n",(const char*)addr);
        //执行上述代码后,这时候内存addr的内容除了最开始有“Hello World!\n“其他区域是一篇空白,而且并不是可执行的代码区域。
        
        //虚拟内存的remap重映射。执行完vm_remap函数后addr的内存将被重新映射到testfn函数所在的内存页中,这时候addr所指的内容将不在是Hello world!了,而是和函数testfn的代码保持一致。
        vm_prot_t cur,max;
        ret = vm_remap(mach_task_self(), &addr, page_size, 0, VM_FLAGS_FIXED | VM_FLAGS_OVERWRITE, mach_task_self(), (vm_address_t)testfn, false, &cur, &max, VM_INHERIT_SHARE);
        // ret = vm_remap(mach_task_self(), (unsigned long *)testfn, page_size, 0, VM_FLAGS_FIXED | VM_FLAGS_OVERWRITE, mach_task_self(), (vm_address_t)&addr, false, &cur, &max, VM_INHERIT_SHARE);
        if (ret == KERN_SUCCESS)
        {
           int c1 = testfn(10, 20);    //执行testfn函数
           int c2 = ((int (*)(int,int))addr)(10,20); //addr重新映射后将和testfn函数具有相同内容,所以这里可以将addr当做是testfn函数一样被调用。
           if(c1 == c2) {
                printf("call addr return: %d\n", c2);
           }
        }
 
       vm_deallocate(mach_task_self(), addr, page_size);
    }
 
   return 0;
}
