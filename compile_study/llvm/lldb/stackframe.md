# stack frame 栈帧

frame select 1                                      跳转栈帧, thread backtrace 所显示的帧中,跳到第1帧
frame variable                                      打印当前栈帧的所有变量
frame info                                          打印方法名和行数

如:
(lldb) frame variable
(ViewController *) self = 0x0000000102c107e0
(SEL) _cmd = "viewDidLoad"
