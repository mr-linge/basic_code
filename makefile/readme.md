# makefile
编译辅助工具

## 原理及功能
make是一个递归创建的过程:
1. 如果当前依赖关系中没有说明前提文件，那么直接执行操作。
2. 如果当前依赖关系说明了目标文件，而目标文件所需的前提文件已经存在，而且前提文件与上次make时没有发生改变(根据最近写入时间判断)，也直接执行该依赖关系的操作。
如果当前目标文件依赖关系所需的前提文件不存在，或者前提文件发生改变，那么以前提文件为新的目标文件，寻找依赖关系，创建目标文件。

内部宏       功能
$*          当前依赖关系中的目标文件名，不包括后缀。
$$          字符"$"

符号作用  
@           后的命令将不显示命令本身。   
-           后面的命令将忽略错误(比如删除不存在的文件)。   
'#'         号起始的行是注释行  

all:
如果make后没有跟随文件名，那么将执行 all 依赖关系

.PHONY : clean
表示 clean 是一个“伪目标”

clean:
常用于清理历史文件。


循环
LIST = one two three
all:
    for i in $(LIST); do \
        echo $$i; \
     done

等同于
all:
    for i in one two three; do \
        echo $i; \
    done

## 参数
-C <dir>, --directory=<dir>
指定读取makefile的目录。如果有多个“-C”参数，make的解释是后面的路径以前面的作为相对路径，并以最后的目录作为被指定目录。
如: "make -C ~hchen/test -C prog”等价于“make -C ~hchen/test/prog"

-e, --environment-overrides
指明环境变量的值覆盖makefile中定义的变量的值

-f=<file>, --file=<file>, --makefile=<file>
指定需要执行的makefile

-i , --ignore-errors
在执行时忽略所有的错误

-I <dir>, --include-dir=<dir>
指定一个被包含makefile的搜索目标。可以使用多个“-I”参数来指定多个目录。

-j [<jobsnum>], --jobs[=<jobsnum>]
指同时运行命令的个数。如果没有这个参数，make运行命令时能运行多少就运行多少。如果有一个以上的“-j”参数，那么仅最后一个“-j”才是有效的
