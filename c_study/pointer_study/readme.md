Pointer


指针与地址
type * p = (type *) malloc(n * sizeof(type)); 
p + i 的步长为 sizeof(type)
unsigned long step1 =  (p + 1);
unsigned long step2 =  (p + 2);
unsigned long step =  step2 - step1;
step 与 sizeof(type) 相等
直接 char * new_p =  (char *)p; (new_p + 1)的步长就为 1了


pointer and array
array 就是通过 pointer 实现的，数组名是一个地址（首元素地址），即是一个指针常量。（不是指针变量）
指针也可以通过下标管理数据
type * p = (type *) malloc(n * sizeof(type)); 
*(p + i) 等价 p[i]
区别:
数组存储在 Stack 栈上
指针存储在 Heap  堆上 


char *p And char p[] :
char* p
p的含义是指向常量字符的指针 (常量指针)
char* p="hello";等价于const char* p="hello"
常量字符：说明hello这个字符串是无法改变的，则不能通过更改指针指向的内容，来改变字符串hello。
*(p+2)='w';错误
常量指针：是可以改变自身的值，输出p的时候就不是hello了而是别的字符串
p="newstring";正确

char p[]
char p[] = "hello";等价于char const* p = "hello"；
p的含义是指向字符串的指针常量 (指针常量)
p是数组的引用，引用本身就是指针常量
指针常量定义：指针常量不能改变本身的值，但是可以改变指向的值
p="newstring";是不合法
*(p+2)='w';合法

char * p = "hello";
char p[] = "hello"
数据都是放在 Stack 上


指针的释放:
栈上的 数据会随着函数的释放而自动释放，由操作系统自动管理
全局 量 或 静态量 放在 Data Segment 里 不需要管理
只有存在 Heap 里的 变量才需要释放，也就是说只有 malloc、 calloc、ralloc 函数分配的 堆空间才需要释放
释放用 free

