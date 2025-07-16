# Universal Plugin Wrapper System

这是一个通用的插件包装系统，可以在不知道原始库内部结构的情况下，将任何动态库包装成插件来覆盖系统默认值。

## 系统架构

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   应用程序      │    │  Universal       │    │   原始系统库    │
│                 │    │  Wrapper         │    │                 │
│  test_system    │───▶│  universal_      │───▶│  mock_system_   │
│  test_with_     │    │  wrapper_plugin  │    │  library.so     │
│  wrapper        │    │  .so             │    │                 │
└─────────────────┘    └──────────────────┘    └─────────────────┘
```

## 文件说明

### 核心组件
- `universal_wrapper.h/c` - Universal wrapper的核心实现
- `universal_demo.c` - 演示程序，展示如何创建和使用wrapper

### 模拟系统
- `mock_system_library.h/c` - 模拟的系统库，提供默认值
- `mock_system_library.so` - 编译后的模拟系统库

### 测试程序
- `test_system.c` - 测试原始系统库的功能
- `test_with_wrapper.c` - 测试使用wrapper覆盖系统默认值

## 编译和运行

### 1. 编译所有组件
```bash
make all
```

### 2. 运行完整测试
```bash
make test
```

### 3. 单独测试

#### 测试原始系统库
```bash
./test_system
```

#### 测试Universal Wrapper
```bash
./test_with_wrapper
```

## 工作原理

### 1. 模拟系统库 (`mock_system_library.so`)
- 提供 `get_system_value()` 函数，返回默认值 "System Default Value"
- 包含其他辅助函数如配置、初始化等

### 2. Universal Wrapper
- 动态加载原始系统库
- 创建包装函数，返回指定的插件值而不是原始值
- 生成 `universal_wrapper_plugin.so` 作为插件

### 3. 测试流程
1. **原始测试**: 直接调用系统库，获取默认值
2. **Wrapper创建**: 使用universal wrapper创建插件，覆盖默认值
3. **插件测试**: 调用包装后的函数，验证值是否被正确覆盖

## 示例输出

### 原始系统库测试
```
=== 测试模拟系统库 ===

1. 直接调用系统库函数:
System library: Returning default value
   返回值: System Default Value

System library: Returning default number
   返回数字: 42

2. 测试系统库的其他功能:
System library: Configuring with 'test_config'
System library: Initializing system
System library: Cleaning up system

=== 测试完成 ===
```

### Universal Wrapper测试
```
=== 测试Universal Wrapper覆盖系统默认值 ===

1. 测试原始系统库:
   原始系统返回值: System Default Value

2. 创建universal wrapper:
   Universal wrapper创建成功!

3. 测试包装后的库:
   包装后返回值: Plugin Override Value - 这是插件覆盖的值!

   ✓ 成功: 系统默认值已被插件值覆盖!

=== 测试完成 ===
```

## 优势

1. **无需了解原始库内部结构** - 只需要知道函数名和返回类型
2. **动态生成插件** - 运行时根据配置生成插件代码
3. **灵活配置** - 可以覆盖任何字符串类型的返回值
4. **保持原始库完整性** - 不修改原始库，只是包装

## 扩展性

这个系统可以轻松扩展以支持：
- 不同类型的返回值（int, float, struct等）
- 多个函数的覆盖
- 更复杂的插件逻辑
- 配置文件驱动的插件生成

## 清理
```bash
make clean
``` 