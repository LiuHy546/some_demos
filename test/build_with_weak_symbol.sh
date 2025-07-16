#!/bin/bash

# 清理旧文件
rm -f *.o *.so *.dll *.exe app_with_plugin app_without_plugin

echo "========================================================="
echo "第一步：构建【唯一版本】的核心库 (libmain.so)"
echo "========================================================="
# 我们现在只用一种方式编译 main.c。
# 编译出来的 main.o 包含了一个 special_variable 的"弱"定义。
gcc -fPIC -c main.c -o main.o

# 将 main.o 链接成动态库。
# 这个 libmain.so 是100%自包含的，可以直接使用，没有任何外部依赖。
gcc -shared -o libmain.so main.o
echo "生成了 'libmain.so'。这是一个独立的、带有默认值的库。"
echo ""


echo "========================================================="
echo "场景A：没有插件的普通程序链接库"
echo "========================================================="
# 链接时，链接器在 libmain.so 中找到了 print_variable。
# print_variable 需要 special_variable，链接器也在 libmain.so 中
# 找到了它的"弱"定义。因为没有其他定义，所以就用它了。
gcc normal_consumer.c -L. -lmain -o app_without_plugin
echo "生成了 'app_without_plugin'。"
echo ""


echo "========================================================="
echo "场景B：带插件的程序链接库"
echo "========================================================="
# 编译插件，它包含一个 special_variable 的"强"定义。
gcc -fPIC -c special.c -o special.o

# 链接时，链接器发现有两个 special_variable 的定义：
#  - 一个来自 special.o (强)
#  - 一个来自 libmain.so (弱)
# 链接器选择强的，丢弃弱的。依赖完美解决！
gcc test_app.c special.o -L. -lmain -o app_with_plugin
echo "生成了 'app_with_plugin'。"
echo ""


echo "========================================================="
echo "运行结果对比"
echo "========================================================="
export LD_LIBRARY_PATH=.

echo "运行无插件版本 'app_without_plugin':"
./app_without_plugin

echo ""
echo "运行有插件版本 'app_with_plugin':"
./app_with_plugin

echo ""
echo "模拟完成！" 