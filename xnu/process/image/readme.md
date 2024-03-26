# 进程的模块信息

开启了ASLR 模块地址(vmaddr_slide)计算时会加上 ASLR

可执行文件都有一个__PAGEZERO段，和检查运行时错误有关，64位程序的__PAGEZERO段大小0x100000000，恰好是32位整数的最大值，防止int做指针的错误
动态库没有 __PAGEZERO段
