CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC
LDFLAGS = -ldl

# 目标文件
TARGETS = test_variable_override
LIBS = mock_system_library.so variable_override.so

# 默认目标
all: $(TARGETS) $(LIBS)

# 编译模拟系统库
mock_system_library.o: mock_system_library.c mock_system_library.h
	$(CC) $(CFLAGS) -c mock_system_library.c

mock_system_library.so: mock_system_library.o
	$(CC) -shared -o mock_system_library.so mock_system_library.o

# 编译变量覆盖库
variable_override.o: variable_override.c
	$(CC) $(CFLAGS) -c variable_override.c

variable_override.so: variable_override.o
	$(CC) -shared -o variable_override.so variable_override.o

# 编译变量覆盖测试程序
test_variable_override.o: test_variable_override.c
	$(CC) $(CFLAGS) -c test_variable_override.c

test_variable_override: test_variable_override.o
	$(CC) -o test_variable_override test_variable_override.o

# 清理
clean:
	rm -f *.o *.so $(TARGETS)

# 运行测试
test: all
	@echo "=== 运行变量覆盖测试 ==="
	@echo "不使用LD_PRELOAD:"
	./test_variable_override
	@echo ""
	@echo "使用LD_PRELOAD:"
	LD_PRELOAD=./variable_override.so ./test_variable_override

.PHONY: all clean test 