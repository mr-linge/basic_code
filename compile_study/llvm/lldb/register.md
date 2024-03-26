# register

## 读取寄存器

register read x[n]                  读取 xn 的值
register read                       显示所有常用寄存器
register read --all                 显示当前线程所有寄存器的值

## 写入寄存器

register write x[n] 0x01

直接跳转到返回地址
register write pc `$lr`
