# Linux黑盒库自动执行示例

这个项目演示了如何通过C语言编写的共享库来在运行未知可执行文件时自动执行自定义函数。

## 适用场景

- 对原有可执行文件一无所知（黑盒）
- 无法修改可执行文件代码
- 只能修改库的代码
- 库已经按照规则连接好
- Linux环境

## 核心原理

利用Linux共享库的构造函数属性 `__attribute__((constructor))`，在库被加载时自动执行自定义代码。

## 文件说明

- `blackbox_library.c` - 主要的C语言库文件
- `example_program.c` - 示例可执行程序
- `Makefile` - 编译脚本
- `README.md` - 说明文档

## 快速开始

```bash
# 编译库和示例程序
make

# 运行示例程序
make run
```

## 编译方法

```bash
# 使用Makefile编译
make

# 或者手动编译
gcc -shared -fPIC -o libblackbox.so blackbox_library.c -lpthread -ldl
gcc -o example_program example_program.c
```

## 使用方法

### 方法1: 放在可执行文件同目录
```bash
# 将库放在可执行文件同目录
cp libblackbox.so /path/to/executable/
cd /path/to/executable/
./your_program
```

### 方法2: 设置环境变量
```bash
# 设置库路径
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./your_program
```

### 方法3: 安装到系统目录
```bash
# 安装到系统目录
sudo make install

# 运行程序
./your_program
```

## 自定义函数

在`blackbox_library.c`中的`customFunction()`函数中添加您需要的逻辑：

```c
void customFunction(void) {
    // 在这里添加您的自定义代码
    // 例如：文件操作、网络请求、系统调用等
}
```

## 执行时机

库会在以下时机自动执行：

1. **库加载时** - 构造函数自动执行
2. **库卸载时** - 析构函数自动执行

## 示例程序演示

示例程序展示了库的自动执行功能：
- 程序启动时，如果库在搜索路径中，会自动加载
- 库加载时会自动执行构造函数
- 程序正常运行，同时库在后台执行自定义逻辑

运行示例：
```bash
make run
```

## 日志记录

库会自动创建`blackbox_library.log`文件，记录：
- 库加载时间
- 自定义函数调用时间
- 库卸载时间

## 高级功能

- **线程安全** - 使用互斥锁保护日志文件访问
- **自动初始化** - 库加载时自动执行初始化
- **导出函数** - 提供外部调用接口
- **错误处理** - 完善的错误检查和处理

## 注意事项

1. 确保库的ABI兼容性
2. 避免在构造函数中执行耗时操作
3. 正确处理异常和错误情况
4. 注意线程安全问题
5. 确保库文件有执行权限

## 故障排除

### 库无法加载
```bash
# 检查库文件
file libblackbox.so
ldd libblackbox.so

# 检查符号
nm -D libblackbox.so
```

### 权限问题
```bash
# 确保库文件有执行权限
chmod +x libblackbox.so
```

### 路径问题
```bash
# 检查库搜索路径
echo $LD_LIBRARY_PATH
ldconfig -p | grep blackbox
``` 