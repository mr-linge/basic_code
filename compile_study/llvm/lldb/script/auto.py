#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
import lldb
import re
import sys

"""
lldb.SBDebugger     : 代表调试器实例的类,拥有命令行解析和所有调试目标
lldb.SBTarget       : 代表当前调试目标
lldb.SBProcess      : 代表当前调试目标的执行进程,管理进程并且可访问全部内存空间
lldb.SBThread       : 代表当前选择的线程,并且管理全部栈帧。及时当前Target停止仍然可以被选择
lldb.SBFrame        : 代表某一栈帧,管理所属的局部变量和寄存器快照
"""


# 根据模块名获取 模块基址
def get_module_vaddr(module_name):
    interpreter = lldb.debugger.GetCommandInterpreter()
    return_object = lldb.SBCommandReturnObject()

    cmd1 = "image list -o -f " + module_name
    interpreter.HandleCommand(cmd1, return_object)  # 执行image list -o -f 命令
    output = return_object.GetOutput()  # 获取命令的返回值
    # print("output:" + output)
    match = re.match(r".+(0x[0-9a-fA-F]+)", output)  # 正则匹配(0x开头)
    if match:
        return match.group(1)
    else:
        return None


# 获取 主程序 在进程中的虚拟地址
def test(debugger, command, result, internal_dict):
    module_name = "WhatsApp"
    print(module_name + " vaddr:" + get_module_vaddr(module_name))


def study01(debugger, command, result, internal_dict):
    interpreter = lldb.debugger.GetCommandInterpreter()
    return_object = lldb.SBCommandReturnObject()  # 用来保存结果
    interpreter.HandleCommand("dis", return_object)  # 执行dis命令
    output = return_object.GetOutput()
    # 获取反汇编后的结果
    print("asm:\n" + output)

    debugger.HandleCommand("breakpoint set --name stat")  # 添加断点
    debugger.HandleCommand("breakpoint set --name open")  # 添加断点
    debugger.HandleCommand("continue")  # 运行
    # 执行 register read x0 并将返回结果保存在 return_object
    interpreter.HandleCommand("register read x0", return_object)
    print(sys._getframe().f_lineno, "x0 = " + return_object.GetOutput())
    debugger.HandleCommand("si")  # 单步步入
    interpreter.HandleCommand("register read x1", return_object)
    print(sys._getframe().f_lineno, "x1 = " + return_object.GetOutput())
    debugger.HandleCommand("continue")


def study02(debugger, command, result, internal_dict):
    interpreter = lldb.debugger.GetCommandInterpreter()
    return_object = lldb.SBCommandReturnObject()  # 用来保存结果
    debugger.HandleCommand("breakpoint set --name main")  # 添加断点
    interpreter.HandleCommand("thread backtrace", return_object)
    print(sys._getframe().f_lineno, "stop info = " + return_object.GetOutput())


# 加载 script.py 文件时的初始化方法
def __lldb_init_module(debugger, internal_dict):
    # 连接 debugserver
    debugger.HandleCommand("process connect connect://127.0.0.1:8090")
    # 输入  cmd    执行 test 方法
    debugger.HandleCommand("command script add -f auto.test cmd")
    # 输入  cmd1   执行 study01 方法
    debugger.HandleCommand("command script add -f auto.study01 cmd1")
    # 输入  cmd2   执行 study02 方法
    debugger.HandleCommand("command script add -f auto.study02 cmd2")
