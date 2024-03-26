# breakpoint 断点

## 设置断点

breakpoint set --address [vaddr]      对内存地址下断点
breakpoint set --name    [symbol]     给所有名为xx的函数设置一个断点

breakpoint set --func-regex regular-expression      # 给所有函数名正则匹配成功的函数设置一个断点
如:
breakpoint set --func-regex "-[NSUserDefaults *]"
breakpoint set --func-regex "-[NSFileManager *]"

### Objective-C

breakpoint set --name "-[objc msgSend:]"
给一个OC函数[objc msgSend:]设置一个断点

给所有名为xx的OC方法设置一个断点(如果没有名为xx的C或者C++函数)
breakpoint set —-selector xx
如:
breakpoint set --selector touchesBegan:withEvent:

### C++

breakpoint set —-method xxx
给所有名为xx的C++函数设置一个断点(如果没有同名的C函数)

## breakpoint list

breakpoint list                 # 查看断点列表

## 删除断点

breakpoint delete [index]       # index指明断点的序号,如果为空则删除所有断点
breakpoint delete xxx -f        # 强制删除(不需要确认)

## breakpoint disable/enable    禁用/启用断点

breakpoint disable 1            禁用 1 号断点
breakpoint enable  1            启用 1 号断点

## 断点命令

breakpoint command add 断点编号: 给断点预先设置需要执行的命令，当触发断点时候，就会执行
如:
(lldb) breakpoint command add 1
Enter your debugger command(s).  Type 'DONE' to end.
> print "breakpoint was hit"
> register read x0
> register read x1
> DONE

breakpoint command list 断点编号: 列出断点的绑定的命令

breakpoint command delete 断点编号: 删除断点绑定的命令

## 监控

监控变量:
watchpoint set variable 变量
如:
(lldb) watchpoint set variable self->age

监控地址:
watchpoint set expression 地址
(lldb) watchpoint set expression 0x0000000101b3eab3

条件监控:
watchpoint modify -c '(global==5)'

watchpoint list                 所有监控列表
watchpoint diable 断点编号
watchpoint enable 断点编号
watchpoint delete 断点编号
watchpoint command add 断点编号
watchpoint command list 断点编号
watchpoint command delete 断点编号
