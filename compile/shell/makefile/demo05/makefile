
name=Dio
param=

func1:
	@# 不能定义局部变量
	@key="application-identifier"
	@echo "key: ${key}"
	@echo "name: ${name}"

func2:$(name)
	@${name}=JOJO

func3:
	@echo "name: ${name}"

all: func1 func2 func3
