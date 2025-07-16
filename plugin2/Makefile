CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC
LDFLAGS = -ldl

# 目标文件
TARGETS = universal_demo test_system test_with_wrapper
LIBS = mock_system_library.so

# 默认目标
all: $(TARGETS) $(LIBS)

# 编译universal wrapper库
universal_wrapper.o: universal_wrapper.c universal_wrapper.h
	$(CC) $(CFLAGS) -c universal_wrapper.c

# 编译模拟系统库
mock_system_library.o: mock_system_library.c mock_system_library.h
	$(CC) $(CFLAGS) -c mock_system_library.c

mock_system_library.so: mock_system_library.o
	$(CC) -shared -o mock_system_library.so mock_system_library.o

# 编译universal demo程序
universal_demo.o: universal_demo.c universal_wrapper.h
	$(CC) $(CFLAGS) -c universal_demo.c

universal_demo: universal_demo.o universal_wrapper.o
	$(CC) -o universal_demo universal_demo.o universal_wrapper.o $(LDFLAGS)

# 编译测试系统程序
test_system.o: test_system.c mock_system_library.h
	$(CC) $(CFLAGS) -c test_system.c

test_system: test_system.o mock_system_library.o
	$(CC) -o test_system test_system.o mock_system_library.o

# 编译带wrapper的测试程序
test_with_wrapper.o: test_with_wrapper.c universal_wrapper.h
	$(CC) $(CFLAGS) -c test_with_wrapper.c

test_with_wrapper: test_with_wrapper.o universal_wrapper.o
	$(CC) -o test_with_wrapper test_with_wrapper.o universal_wrapper.o $(LDFLAGS)

# 清理
clean:
	rm -f *.o *.so $(TARGETS)

# 运行测试
test: all
	@echo "=== 运行模拟系统库测试 ==="
	./test_system
	@echo ""
	@echo "=== 运行Universal Wrapper测试 ==="
	./test_with_wrapper

.PHONY: all clean test 