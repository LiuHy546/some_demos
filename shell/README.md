# 多功能工具脚本 (multi_tool.sh)

一个功能齐全的shell脚本，支持多种系统操作和参数选择。

## 功能特性

- 🗂️ **文件操作**: 备份、压缩、信息查看
- 🌐 **网络操作**: ping测试、文件下载、连接检查
- 💻 **系统操作**: 系统信息、磁盘、内存、进程监控
- 💾 **备份操作**: 创建、列表、恢复备份
- 📊 **监控操作**: CPU、内存、磁盘、网络实时监控
- 🧪 **测试功能**: 依赖检查、功能测试

## 安装和使用

### 1. 下载脚本
```bash
# 确保脚本有执行权限
chmod +x multi_tool.sh
```

### 2. 基本用法
```bash
./multi_tool.sh [选项] [命令] [参数...]
```

## 命令详解

### 文件操作 (file)
```bash
# 备份文件或目录
./multi_tool.sh file backup /path/to/file

# 压缩文件或目录
./multi_tool.sh file compress /path/to/directory

# 查看文件信息
./multi_tool.sh file info /path/to/file
```

### 网络操作 (network)
```bash
# ping测试
./multi_tool.sh network ping google.com

# 下载文件
./multi_tool.sh network download https://example.com/file.zip

# 检查网络连接
./multi_tool.sh network check https://example.com
```

### 系统操作 (system)
```bash
# 显示系统信息
./multi_tool.sh system info

# 查看磁盘使用情况
./multi_tool.sh system disk

# 查看内存使用情况
./multi_tool.sh system memory

# 查看进程信息
./multi_tool.sh system processes

# 查看系统运行时间
./multi_tool.sh system uptime
```

### 备份操作 (backup)
```bash
# 创建备份
./multi_tool.sh backup create /home/user

# 列出备份
./multi_tool.sh backup list

# 恢复备份
./multi_tool.sh backup restore /path/to/backup.tar.gz
```

### 监控操作 (monitor)
```bash
# CPU使用率监控 (默认60秒)
./multi_tool.sh monitor cpu

# 内存使用率监控 (30秒)
./multi_tool.sh monitor memory 30

# 磁盘使用率监控 (120秒)
./multi_tool.sh monitor disk 120

# 网络连接监控 (60秒)
./multi_tool.sh monitor network 60
```

### 测试功能 (test)
```bash
# 运行所有测试
./multi_tool.sh test all

# 测试依赖
./multi_tool.sh test dependencies

# 测试文件操作
./multi_tool.sh test file

# 测试网络连接
./multi_tool.sh test network

# 测试系统功能
./multi_tool.sh test system
```

## 全局选项

```bash
-h, --help          显示帮助信息
-v, --version       显示版本信息
-d, --debug         启用调试模式
-q, --quiet         静默模式
-l, --log           启用日志记录
-c, --config FILE   指定配置文件
```

## 配置文件

脚本支持配置文件来自定义默认行为。默认配置文件为 `config.conf`。

### 配置文件示例
```bash
# 默认超时时间（秒）
DEFAULT_TIMEOUT=30

# 默认输出目录
DEFAULT_OUTPUT_DIR="./output"

# 日志级别
LOG_LEVEL="INFO"

# 网络相关配置
NETWORK_TIMEOUT=10
NETWORK_RETRY=3
```

## 使用示例

### 1. 基本系统信息查看
```bash
./multi_tool.sh system info
```

### 2. 备份重要文件
```bash
./multi_tool.sh file backup /home/user/documents
```

### 3. 监控系统资源
```bash
./multi_tool.sh monitor cpu 30
```

### 4. 网络连接测试
```bash
./multi_tool.sh network ping 8.8.8.8
```

### 5. 运行完整测试
```bash
./multi_tool.sh test all
```

### 6. 使用调试模式
```bash
./multi_tool.sh -d system info
```

### 7. 启用日志记录
```bash
./multi_tool.sh -l file backup /path/to/file
```

### 8. 使用自定义配置文件
```bash
./multi_tool.sh -c my_config.conf system info
```

## 输出说明

- **INFO**: 绿色 - 正常信息
- **WARN**: 黄色 - 警告信息
- **ERROR**: 红色 - 错误信息
- **DEBUG**: 蓝色 - 调试信息（仅在调试模式下显示）

## 依赖要求

脚本需要以下系统命令：
- `curl` - 网络请求
- `wget` - 文件下载
- `tar` - 文件压缩
- `gzip` - 压缩工具

## 日志文件

当启用日志记录时，脚本会创建 `multi_tool.log` 文件记录所有操作。

## 输出目录

默认情况下，所有输出文件（备份、下载等）都保存在 `./output` 目录中。

## 错误处理

脚本包含完善的错误处理机制：
- 参数验证
- 文件存在性检查
- 网络连接检查
- 依赖检查

## 许可证

MIT License

## 作者

AI Assistant

## 版本历史

- v1.0 - 初始版本，包含基本功能

## 贡献

欢迎提交问题和改进建议！

## 注意事项

1. 某些功能可能需要管理员权限
2. 网络功能需要网络连接
3. 监控功能在Windows环境下可能有限制
4. 建议在测试环境中先验证功能 