GCC编译并运行c代码
参考文章:https://blog.csdn.net/qq_42475711/article/details/85224010

#mac上gcc关键字已经被占用,只能以gcc-9来运行gcc
#gcc在编译main.c文件时 要把所有引用到的文件一起编译,要不然编译时找不到对应的引用方法会报编译错误
gcc-9 main.c study01.c study02.c 
