strip命令通过除去绑定程序和符号调试程序使用的信息，降低扩展公共对象文件格式（XCOFF）的对象文件的大小。

file xxx.so 可以查看文件是否经过 strip 处理。
经过strip 处理后的ELF文件 没有 section 信息、无法调试，而我自己写的那个根据符号名查找符号地址的方法，void lookup_symbol(handle_t *h);
恰恰是通过解析ELF文件来寻找symbol 所对应的 symbol offset。 所以 void lookup_symbol(handle_t *h) 是无法在 strip 后的文件里找到偏移的
