// unsigned long printf_vaddr = 0;
// 根据地址调用 printf() 这种形式的函数
typedef int (*MY_PRINTF)(const char *__restrict __format, ...);
// unsigned long test_var = 0x100;
// MY_PRINTF my_print = 0;
void init_symbol_vaddr(unsigned long printf_vaddr)
{
        MY_PRINTF my_print = 0;
        my_print = (MY_PRINTF)printf_vaddr;
        char *str = "Hello world! init_symbol";
        // my_print("%s ----------- %ld\n", str, test_var);
        my_print("%s\n", str);
}

void call_printf()
{
        // MY_PRINTF my_print = 0;
        // my_print = (MY_PRINTF)printf_vaddr;
        char *str = "Hello world! init_symbol";
        my_print("%s\n", str);
}

int func1(int p1)
{
        return (p1 + 10);
}

int hook_func2(int p1, int p2)
{
        return (p1 + p2);
}

int hook_func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
{
        return 0x99;
}

int hook_func20(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20)
{
        // call_printf();
        return 0x999;
}

void read_mem(unsigned long vaddr, unsigned long len)
{
        //    printf("Hello, this is read_mem ...\n");
        // uint8_t * tmp_addr = (uint8_t *)vaddr;
        // for(unsigned long i = 0; i < len; i++) {
        //        printf("%02x ",*(tmp_addr + i));
        //}
        // puts("");
}
