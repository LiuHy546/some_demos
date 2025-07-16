CC=gcc
CFLAGS=-fPIC -Wall -g -std=c99
LDFLAGS=-ldl

# 简单版本
all: main_simple plugin_value_plugin.so production_demo

# 简单主程序
main_simple: main_simple.c
	$(CC) $(CFLAGS) -o main_simple main_simple.c $(LDFLAGS)

# 简单插件
plugin_value_plugin.so: plugin_value_plugin.c
	$(CC) $(CFLAGS) -shared -o plugin_value_plugin.so plugin_value_plugin.c

plugin_value_plugin.dll: plugin_value_plugin.c
	$(CC) $(CFLAGS) -shared -o plugin_value_plugin.dll plugin_value_plugin.c

# 生产级版本
production_demo: main_production plugin_manager.o production_plugin.so

main_production: main_production.c plugin_manager.o
	$(CC) $(CFLAGS) -o main_production main_production.c plugin_manager.o $(LDFLAGS)

plugin_manager.o: plugin_manager.c plugin_manager.h plugin_interface.h
	$(CC) $(CFLAGS) -c plugin_manager.c

production_plugin.so: production_plugin.c plugin_interface.h
	$(CC) $(CFLAGS) -shared -o production_plugin.so production_plugin.c

production_plugin.dll: production_plugin.c plugin_interface.h
	$(CC) $(CFLAGS) -shared -o production_plugin.dll production_plugin.c

# 清理
clean:
	rm -f main_simple plugin_value_plugin.so plugin_value_plugin.dll
	rm -f main_production plugin_manager.o production_plugin.so production_plugin.dll
	rm -f *.o

# 测试目标
test_simple: main_simple plugin_value_plugin.so
	@echo "Testing simple plugin system..."
	./main_simple

test_production: production_demo
	@echo "Testing production plugin system..."
	./main_production

.PHONY: all clean test_simple test_production 