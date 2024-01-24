# 静态链接库

archive options :
ar : archive 压缩, object file 压缩成 static library
-c : create 仓库 static library,一般与 -r 连用
-r : replace 替换,表示当前插入的模块名已经在库中存在,则替换同名的模块。如果若干模块中有一个模块在库中不存在,ar显示一个错误信息,并不替换其他同名的模块。默认的情况下,新的成员增加在静态库结尾处。
-t : 查看静态库中的目标文件
-x : 解压缩 static library 获取到所有的 object file

## 基本使用

压缩 object file 成 static library
ar -cr libmymath.a sub.o add.o

添加一个 object file 到 static library
ar -r libmymath.a new.o

删除库文件中指定的目标文件
ar -d libmymath.a new.o

解压缩 static library
ar -x libmymath.a

查看 static library 中的 object file
ar -t libmymath.a
