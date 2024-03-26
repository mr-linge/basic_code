# 模块

列出所有加载的模块
image list

列出指定模块信息
image list -o -f [module]

导出模块的所有符号表
image dump symtab

image lookup --type [symbol]                        查看成员
如:
image lookup --type ViewController

image lookup --name [symbol]                        查找方法来源
如:
image lookup --name main

查找崩溃位置.遇见数组崩溃又没有找到崩溃的位置,只有一堆报错信息,这时候image lookup来可以定位崩溃信息
image lookup --address [module start vaddr]
如: image lookup --address 0x0000000103110000
