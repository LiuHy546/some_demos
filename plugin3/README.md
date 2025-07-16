# LD_PRELOAD 变量覆盖系统

这是一个使用LD_PRELOAD技术覆盖系统默认值的简单解决方案，适用于不支持插件机制的原系统。

## 系统架构

```
┌─────────────────┐    ┌──────────────────┐
│   应用程序      │    │  变量覆盖库      │
│                 │    │                  │
│  test_variable_ │───▶│  variable_       │
│  override       │    │  override.so     │
└─────────────────┘    └──────────────────┘
```

## 文件说明

### 核心组件
- `variable_override.c` - 变量覆盖库的源代码
- `variable_override.so` - 编译后的变量覆盖库
- `test_variable_override.c` - 测试程序

### 模拟系统
- `mock_system_library.h/c` - 模拟的系统库，提供默认值
- `mock_system_library.so` - 编译后的模拟系统库

## 编译和运行

### 1. 编译所有组件
```bash
make all
```

### 2. 运行测试
```bash
make test
```

### 3. 单独测试

#### 不使用LD_PRELOAD（使用默认值）
```bash
./test_variable_override
```

#### 使用LD_PRELOAD（使用覆盖值）
```bash
LD_PRELOAD=./variable_override.so ./test_variable_override
```

## 工作原理

### 1. 变量覆盖库 (`variable_override.so`)
- 定义与原系统相同名称的全局变量
- 使用构造函数在库加载时输出信息
- 使用析构函数在库卸载时输出信息

### 2. LD_PRELOAD机制
- 使用环境变量 `LD_PRELOAD` 预加载覆盖库
- 系统会优先使用预加载库中的符号
- 不需要修改原系统代码

### 3. 测试流程
1. **默认测试**: 直接运行程序，使用默认值
2. **覆盖测试**: 使用LD_PRELOAD运行程序，使用覆盖值

## 示例输出

### 不使用LD_PRELOAD
```
=== 测试变量覆盖 ===

当前系统默认值: 42
使用该值的计算结果: 84

=== 测试完成 ===

使用方法:
LD_PRELOAD=./variable_override.so ./test_variable_override
```

### 使用LD_PRELOAD
```
=== 变量覆盖库已加载 ===
系统默认值已被覆盖为: 999
=== 测试变量覆盖 ===

当前系统默认值: 999
使用该值的计算结果: 1998

=== 测试完成 ===

使用方法:
LD_PRELOAD=./variable_override.so ./test_variable_override
=== 变量覆盖库已卸载 ===
```

## 实际使用

### 1. 修改覆盖值
编辑 `variable_override.c` 中的 `system_default_value`：
```c
int system_default_value = 999;  // 改为你需要的值
```

### 2. 重新编译
```bash
make clean
make all
```

### 3. 应用到你的系统
```bash
# 替换为你的实际程序
LD_PRELOAD=./variable_override.so ./your_original_program
```

## 优势

1. **无需修改原系统** - 完全通过环境变量控制
2. **简单易用** - 只需要重新编译覆盖库
3. **安全可靠** - 不会破坏原系统文件
4. **灵活配置** - 可以随时启用或禁用

## 注意事项

1. **变量名必须完全一致** - 覆盖库中的变量名必须与原系统相同
2. **类型必须匹配** - 变量类型必须与原系统一致
3. **符号可见性** - 原系统中的变量必须是全局可见的

## 清理
```bash
make clean
``` 