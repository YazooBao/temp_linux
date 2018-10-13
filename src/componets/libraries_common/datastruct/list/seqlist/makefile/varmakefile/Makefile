
#变量
TARGET=target
OBJS=main.o add.o sub.o mul.o

$(TARGET):$(OBJS)
	@gcc -o $@ $^ 
#$@取得目标文件  $^取得所有的依赖文件
#@规则:隐藏规则

clean:
	@rm *.o target



