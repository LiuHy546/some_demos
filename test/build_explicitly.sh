 #!/bin/bash

# 清理旧文件，为模拟做准备
rm -f *.o *.so *.dll *.exe test_app_final

echo "========================================================="
echo "第一步：将插件编译为目标文件 (special.o)"
echo "========================================================="
# 我们只编译不链接，得到一个.o文件。
# 这个 special.o 文件里定义了 special_variable 和它的构造函数。
# 注意：此时它有一个未解决的依赖：get_base_value() 函数。
gcc -fPIC -c special.c -o special.o
echo "生成了 'special.o'。可以视其为'已打包的插件'。"
echo ""


echo "========================================================="
echo "第二步：将核心C文件打包成动态库 (libmain.so)"
echo "========================================================="
# 1. 编译 main.c，并启用插件宏。
#    这会告诉 main.c，special_variable 是一个外部变量，需要在别处寻找。
gcc -fPIC -DUSE_SPECIAL_PLUGIN -c main.c -o main.o

# 2. 将 main.o 链接成动态库。
#    重要：我们没有把 special.o 链接进去！
#    因此，这个 libmain.so 本身是不“完整”的，它知道它需要一个叫
#    special_variable 的变量，但它自己内部没有。
#    这完全没问题，因为动态库允许有未解析的符号，这些符号
#    会在最终链接可执行文件时或者在运行时被解析。
gcc -shared -o libmain.so main.o
echo "生成了 'libmain.so'。可以视其为'有外部依赖的so包'。"
echo ""


echo "========================================================="
echo "第三步：链接所有部分，生成最终可执行文件"
echo "========================================================="
# 这是最终的链接步骤。我们把所有东西都交给链接器：
#  - test_app.c: 我们应用程序的主入口。
#  - special.o:  我们的“插件包”，它提供了 special_variable 的定义。
#  - -L. -lmain: 我们依赖的“核心so包”。
#
# 链接器的工作流程：
# 1. 发现 test_app 调用了 libmain.so 中的 print_variable。
# 2. 发现 print_variable 引用了 special_variable。
# 3. 在我们提供的 special.o 中找到了 special_variable 的定义，依赖解决！
# 4. 发现 special.o 调用了 get_base_value。
# 5. 在我们提供的 libmain.so 中找到了 get_base_value 的定义，依赖解决！
#
# 所有依赖都解决了，链接成功！
gcc test_app.c special.o -L. -lmain -o test_app_final
echo "生成了最终可执行文件 'test_app_final'。"
echo ""


echo "========================================================="
echo "运行最终程序"
echo "========================================================="
# 在Linux/macOS上，需要设置LD_LIBRARY_PATH，以便运行时能找到 libmain.so
export LD_LIBRARY_PATH=.
./test_app_final

echo "模拟完成！"

