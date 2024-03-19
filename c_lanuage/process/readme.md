# 进程

在Linux系统中,进程常见的状态有下面一些:
S:          Interruptible Sleeping,即可中断睡眠
D:          Uninterruptible Sleeping,即不可中断睡眠
R:          Running or Runnable,即运行状态
Z:          Zombie,即僵尸状态
T:          Stopped or Traced,即中止状态(注意是"中止"而非"终止")

## 启动新进程

fork + exec
