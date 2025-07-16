# C 插件系统 - 生产级实现

## 概述

这是一个生产级的C语言插件系统，支持简单的值覆盖功能。系统包含两个版本：

1. **简单版本**：最基础的插件覆盖值功能
2. **生产版本**：完整的插件管理系统，包含错误处理、版本检查、配置管理等

## 核心特性

### 简单版本特性
- 系统默认值：42
- 插件可选加载，有插件则用插件值，无插件则用默认值
- 系统编译和运行不依赖插件

### 生产版本特性
- **完整的插件生命周期管理**：加载、初始化、执行、卸载
- **版本兼容性检查**：确保插件与系统版本兼容
- **配置验证**：插件配置参数验证
- **错误处理**：详细的错误码和错误信息
- **状态管理**：插件状态跟踪（未加载、已加载、已初始化、错误、已禁用）
- **统计信息**：插件管理器运行统计
- **日志记录**：详细的插件操作日志
- **热重载**：支持插件重新加载
- **启用/禁用**：动态启用或禁用插件

## 文件结构

```
plugin_interface.h      # 插件接口定义（生产版）
plugin_manager.h        # 插件管理器头文件（生产版）
plugin_manager.c        # 插件管理器实现（生产版）
main_simple.c          # 简单版本主程序
main_production.c      # 生产版本主程序
plugin_value_plugin.c  # 简单版本插件
production_plugin.c    # 生产版本插件
Makefile               # 构建脚本
```

## 编译

### 编译所有版本
```bash
make
```

### 编译简单版本
```bash
make main_simple plugin_value_plugin.so
```

### 编译生产版本
```bash
make production_demo
```

## 运行

### 简单版本
```bash
./main_simple
```

### 生产版本
```bash
./main_production
```

### 运行测试
```bash
make test_simple    # 测试简单版本
make test_production # 测试生产版本
```

## 插件开发

### 简单版本插件
只需导出一个函数：
```c
PLUGIN_EXPORT int plugin_get_value(void) {
    return 123; // 你的值
}
```

### 生产版本插件
需要实现以下接口：
- `plugin_create()` - 创建插件实例
- `plugin_destroy()` - 销毁插件实例
- `plugin_get_value()` - 获取插件值
- `plugin_get_info()` - 获取插件信息
- `plugin_validate_config()` - 验证配置

## 生产版本功能演示

生产版本主程序演示了以下功能：
1. 无插件时使用默认值
2. 加载插件并获取信息
3. 列出所有插件
4. 获取统计信息
5. 禁用/启用插件
6. 重新加载插件
7. 卸载插件
8. 验证插件兼容性

## 错误处理

系统定义了详细的错误码：
- `PLUGIN_SUCCESS` - 成功
- `PLUGIN_ERROR_INVALID_HANDLE` - 无效句柄
- `PLUGIN_ERROR_VERSION_MISMATCH` - 版本不匹配
- `PLUGIN_ERROR_INIT_FAILED` - 初始化失败
- `PLUGIN_ERROR_INVALID_CONFIG` - 无效配置
- `PLUGIN_ERROR_MEMORY_ALLOCATION` - 内存分配失败
- `PLUGIN_ERROR_NOT_IMPLEMENTED` - 功能未实现

## 跨平台支持

- **Linux**: 使用 `.so` 动态库
- **Windows**: 使用 `.dll` 动态库
- **macOS**: 使用 `.dylib` 动态库

## 清理

```bash
make clean
```

## 注意事项

1. 生产版本需要实现完整的插件接口
2. 插件版本必须与系统版本兼容
3. 插件配置必须通过验证
4. 系统默认值始终可用，插件是可选的
5. 插件加载失败不会影响系统正常运行 