# lldb 命令

命令                                        缩写                     功能
process continue                            c                       从当前断点处开始执行程序
thread step-over                            next, n                 下一步(不进入子程序执行流)
thread step-in                              step, s                 下一步(进入子程序执行流)
thread step-out                             finish, f               直接执行完当前函数的所有代码，返回到调用的地方
thread step-inst-over                       nexti, ni               汇编级别的单步下一步
thread step-inst                            stepi, si               汇编级别的进入下一个函数
memory read                                 x                       读取内存数据
thread backtrace                            bt                      查看当前调用栈
breakpoint                                  br                      符号断点
expression                                  exp                     执行一个表达式,并将表达式返回的结果输出
expression -o  --                           po                      打印信息(输出值)
expression --                               p                       打印信息(输出值+值类型+引用名+内存地址)
disassemble                                 dis                     显示汇编代码
