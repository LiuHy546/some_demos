#!/bin/bash

# 清理旧文件
rm -f *.o *.so *.dll *.exe normal_consumer_app

echo "========================================================="
echo "第一步：构建核心库 (libmain.so) - 无插件模式"
echo "========================================================="
# 1. 编译 main.c。关键点：这一次我们【没有】定义 -DUSE_SPECIAL_PLUGIN 宏。
#    因此，main.c 内部的 #else 分支会被激活，
#    它会自己定义 special_variable 并赋值为 100。
gcc -fPIC -c main.c -o main.o

# 2. 将 main.o 链接成动态库 libmain.so。
#    这个版本的 libmain.so 是完全自给自足的，它不依赖任何外部
#    符号来提供 special_variable 的值。
gcc -shared -o libmain.so main.o
echo "生成了 'libmain.so'。这是一个自包含、可独立工作的库。"
echo ""


echo "========================================================="
echo "第二步：构建并链接"正常"的消费者程序"
echo "========================================================="
# 编译我们的消费者程序，并告诉它去链接我们刚才创建的 libmain.so。
#
# 链接器的工作流程：
# 1. 发现 normal_consumer.c 调用了 print_variable。
# 2. 在 libmain.so 中找到了 print_variable 的定义。
# 3. 发现 print_variable 引用了 special_variable。
# 4. 在 libmain.so 内部就找到了 special_variable 的定义（值为100）。
#
# 所有依赖都在 libmain.so 内部解决，链接成功！
# 注意：我们完全没有用到 special.c 或 special.o。
gcc normal_consumer.c -L. -lmain -o normal_consumer_app
echo "生成了最终可执行文件 'normal_consumer_app'。"
echo ""


echo "========================================================="
echo "第三步：运行最终程序"
echo "========================================================="
# 设置库的搜索路径，然后运行
export LD_LIBRARY_PATH=.
./normal_consumer_app

echo ""
echo "模拟完成！库在没有插件的情况下独立工作成功。" 