# 查看调试信息

## expression

expression [cmd-options] -- [expr]
    [cmd-options]:      命令选项,一般情况下使用默认的即可,不需要特别标明
    --           :      命令选项结束符,表示所有的命令选项已经设置完毕,如果没有命令选项,--可以省略
    [expr]       :      要执行的表达式

等价命令
expression --               简写 expression，print，p
expression -o --            简写 po，实际是调用对象的description方法，输出返回值

例子:
expression -o --                打印一个对象
如: expression -o -- self

改变颜色
(lldb) expression -- self.view.backgroundColor = [UIColor redColor]
刷新界面
(lldb) expression -- [void](CATransaction flush)

x0 赋值
(lldb) expression -- self
(ViewController *) $0 = 0x0000000121d0fab0

赋值
expression self.view.hidden = 1

## 打印信息

打印相关的命令有:p、po
p 和 po 的区别在于使用 po 只会输出对应的值,而 p 则会返回值的类型以及命令结果的引用名。
po : 输出值
p  : 输出值 + 值类型 + 引用名 + 内存地址

(lldb) p (CGFloat)self.view.frame.size.width
(CGFloat) $3 = 414
(lldb) po (CGFloat)self.view.frame.size.width
414

(lldb) p self
(ViewController *) $2 = 0x0000000121d0fab0
(lldb) po self
<ViewController: 0x121d0fab0>

打印 Objective-C 参数
po $x0      以OC 对象的格式打印 x0 保存的参数
po $x1      以OC 对象的格式打印 x1 对应的参数

### 打印格式

默认打印为10进制
(lldb) p 100
(int) $7 = 100

转16进制
(lldb) p/x 100
(int) $8 = 0x00000064

转8进制
(lldb) p/o 100
(int) $9 = 0144

转二进制
(lldb) p/t 100
(int) $10 = 0b00000000000000000000000001100100

字符转10进制数字
(lldb) p/d 'a'
(char) $11 = 97

10进制数字转字符
(lldb) p/c 97
(int) $12 = a\0\0\0
