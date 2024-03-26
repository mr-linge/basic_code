# memory

内存读写

## read

memory read 简写 x

Command Options Usage:

size指定内存块(block/item)的大小,默认为1byte
-s(--size):     The size in bytes to use when displaying with the selected format

count指定内存块(block/item)的个数,可配合起始地址使用
-c(--count):    The number of total items to display

format指定内容显示格式(不指定默认是 16进制数据),格式符同print:c-char,s-string,d-decimal,x-hex
-f(--format):   Specify a format to be used for display

--outfile:      把读取到的数据以文件形式输出

--force:        取消读写数据大小限制

### 起始地址到结束地址

memory read [start vmaddr] [end vmaddr]
如:
memory read 0x104e7d620 0x104e7d680

## 起始地址 + count

memory read --force [start vmaddr] -c [count]
如:
memory read --force 0x104e7d620 -c 0x60

### 起始地址 + size + count + format

memory read --force [start vmaddr] -s [block size] -c [block count]
如:
memory read --force 0x1008ad620 -s 0x10 -c 0x6 -f x

### --outfile 输出读取到的数据

memory read --force [start vmaddr] -c [byte count] --outfile [path]
如:
memory read --force 0x1008ad620 -c 0x60 --outfile /tmp/mem.txt

## write

### 起始地址 + 内存块size + 数据

memory write [start vmaddr] -s [block size] [data]
如:
memory write 0x10015d620 -s 0x4 0x10203040
