# 联合体最大成员分析工具

## 工具简介

`analyze_union_max_member.py` 是一个用于分析 C 语言预处理文件（.i 文件），自动找出每个联合体（union）中占用内存最大的成员变量的 Python 脚本。该工具支持复杂的 C 结构体/联合体嵌套、位域、匿名体、typedef、多维数组、指针、内存对齐、数组长度为宏等多种情况。

---

## 支持的 C 语言特性
- 多级嵌套结构体和联合体
- 匿名结构体/联合体成员
- 位域（bitfield）成员
- 联合体嵌套结构体，结构体嵌套联合体
- typedef 类型别名、多级 typedef
- 指针类型（可自定义指针大小）
- 多维数组、数组长度为宏常量
- 内存对齐规则
- 支持常见的 typedef union/struct 写法（无名、有名、别名等）

---

## 使用方法

### 1. 生成预处理文件（.i 文件）

假设你的 C 文件名为 `super_complex.c`，使用 gcc 生成预处理文件：

```sh
gcc -E super_complex.c -o super_complex.i
```

### 2. 运行分析脚本

```sh
python3 analyze_union_max_member.py super_complex.i
```

脚本会自动输出每个 union 的最大成员及其类型和字节数。

---

## 示例

假设有如下 union 定义：

```c
typedef union {
    int a;
    double b;
    char arr[100];
} MyUnion;
```

分析输出：

```
Union MyUnion: 最大成员是 arr 类型 char[100]，大小 100 字节
```

对于复杂嵌套、位域、匿名体等也会自动递归分析。

---

## 注意事项
- 需使用 C 预处理器（如 gcc -E）生成 .i 文件，不能直接分析 .c 文件。
- 仅支持常见 C 语法，极端宏嵌套、条件编译、复杂表达式等可能需手动调整。
- 指针大小可在脚本内 `POINTER_SIZE` 变量中自定义。
- 位域大小按最小字节对齐，结构体/联合体整体按最大成员对齐。
- 仅分析 typedef struct/union 定义，不分析非 typedef 的匿名体。

---

## 适用场景
- C 代码静态分析、内存优化
- 联合体最大成员自动检测
- 代码审查、嵌入式开发、协议解析等场景

---

## 贡献与反馈

如需扩展更多 C 语法支持、发现 bug 或有建议，欢迎提交 issue 或 PR。

---

**作者：**
- 本工具由 AI 辅助生成和维护。 