# 切换工作目录

dir=./directory1

t1:
	@echo $(shell pwd)

t2:
	@cd ${dir}
	@echo $(shell pwd)

t3:
	@# 一行命令启动一个sub shell,所以前一行命令导致环境的变化在下一行会被reset
	@cd ${dir} && echo $(shell pwd)

all: t1 t2 t3
