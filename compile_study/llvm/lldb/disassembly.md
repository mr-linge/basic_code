# 反汇编

disassemble                                                 # 默认显示当前函数的汇编代码
disassemble --start-address 0x1eb8 --count 0x20             # 从 address 开始反汇编, 反汇编的代码行数 0x20
disassemble --start-address 0x1eb8 --end-address 0x1ec3     # 指定开始和结束地址, 显示这段区间的汇编代码
