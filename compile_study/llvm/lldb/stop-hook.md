# target stop-hook

给所有断点添加命令的指令,在每次stop的时候去执行一些命令。类似breakpoint的断点执行命令,只不过breakpoint的断点执行命令需要指定某个断点,而这个命令是全局的,只要是断点来了就会执行后面的命令

## 添加命令

target stop-hook add

Enter your stop hook command(s).  Type 'DONE' to end.
> print "breakpoint was hit"
> register read x0
> register read x1
> DONE

## 查看 hook 列表

target stop-hook list

## 使 hook 失效

target stop-hook disable 1

## 使 hook 生效

target stop-hook enable 1

## 删除 hook

target stop-hook delete  1
