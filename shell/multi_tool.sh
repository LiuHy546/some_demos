#!/bin/bash

# 多功能工具脚本
# 作者: AI Assistant
# 版本: 1.0
# 描述: 一个功能齐全的shell脚本，支持多种参数和功能

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 全局变量
SCRIPT_NAME=$(basename "$0")
VERSION="1.0"
LOG_FILE="multi_tool.log"
CONFIG_FILE="config.conf"

# 默认配置
DEFAULT_TIMEOUT=30
DEFAULT_RETRY_COUNT=3
DEFAULT_OUTPUT_DIR="./output"

# 显示帮助信息
show_help() {
    cat << EOF
${CYAN}多功能工具脚本 v${VERSION}${NC}

${YELLOW}用法:${NC}
    $SCRIPT_NAME [选项] [命令] [参数...]

${YELLOW}命令:${NC}
    ${GREEN}file${NC}      - 文件操作
    ${GREEN}network${NC}   - 网络操作
    ${GREEN}system${NC}    - 系统操作
    ${GREEN}backup${NC}    - 备份操作
    ${GREEN}monitor${NC}   - 监控操作
    ${GREEN}test${NC}      - 测试功能

${YELLOW}全局选项:${NC}
    -h, --help          显示此帮助信息
    -v, --version       显示版本信息
    -d, --debug         启用调试模式
    -q, --quiet         静默模式
    -l, --log           启用日志记录
    -c, --config FILE   指定配置文件

${YELLOW}示例:${NC}
    $SCRIPT_NAME file backup /path/to/file
    $SCRIPT_NAME network ping google.com
    $SCRIPT_NAME system info
    $SCRIPT_NAME backup create /home/user
    $SCRIPT_NAME monitor cpu
    $SCRIPT_NAME test all

${YELLOW}配置文件:${NC}
    默认配置文件: $CONFIG_FILE
    可以通过 -c 选项指定其他配置文件

EOF
}

# 显示版本信息
show_version() {
    echo -e "${CYAN}多功能工具脚本${NC} v${VERSION}"
    echo "作者: AI Assistant"
    echo "许可证: MIT"
}

# 日志函数
log_message() {
    local level="$1"
    local message="$2"
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    case "$level" in
        "INFO")
            echo -e "${GREEN}[INFO]${NC} $message"
            ;;
        "WARN")
            echo -e "${YELLOW}[WARN]${NC} $message"
            ;;
        "ERROR")
            echo -e "${RED}[ERROR]${NC} $message"
            ;;
        "DEBUG")
            if [[ "$DEBUG_MODE" == "true" ]]; then
                echo -e "${BLUE}[DEBUG]${NC} $message"
            fi
            ;;
    esac
    
    if [[ "$LOG_ENABLED" == "true" ]]; then
        echo "[$timestamp] [$level] $message" >> "$LOG_FILE"
    fi
}

# 检查依赖
check_dependencies() {
    local deps=("curl" "wget" "tar" "gzip")
    local missing_deps=()
    
    for dep in "${deps[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing_deps+=("$dep")
        fi
    done
    
    if [[ ${#missing_deps[@]} -gt 0 ]]; then
        log_message "WARN" "缺少依赖: ${missing_deps[*]}"
        return 1
    fi
    
    return 0
}

# 加载配置文件
load_config() {
    if [[ -f "$CONFIG_FILE" ]]; then
        log_message "DEBUG" "加载配置文件: $CONFIG_FILE"
        source "$CONFIG_FILE"
    else
        log_message "DEBUG" "配置文件不存在，使用默认配置"
    fi
}

# 文件操作功能
file_operations() {
    local operation="$1"
    local target="$2"
    
    case "$operation" in
        "backup")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要备份的文件或目录"
                return 1
            fi
            
            if [[ ! -e "$target" ]]; then
                log_message "ERROR" "目标不存在: $target"
                return 1
            fi
            
            local backup_name="backup_$(date +%Y%m%d_%H%M%S)_$(basename "$target")"
            local backup_path="$DEFAULT_OUTPUT_DIR/$backup_name.tar.gz"
            
            mkdir -p "$DEFAULT_OUTPUT_DIR"
            
            log_message "INFO" "开始备份: $target -> $backup_path"
            
            if tar -czf "$backup_path" "$target" 2>/dev/null; then
                log_message "INFO" "备份成功: $backup_path"
                echo "备份文件大小: $(du -h "$backup_path" | cut -f1)"
            else
                log_message "ERROR" "备份失败"
                return 1
            fi
            ;;
            
        "compress")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要压缩的文件或目录"
                return 1
            fi
            
            if [[ ! -e "$target" ]]; then
                log_message "ERROR" "目标不存在: $target"
                return 1
            fi
            
            local compressed_name="$(basename "$target").tar.gz"
            local compressed_path="$DEFAULT_OUTPUT_DIR/$compressed_name"
            
            mkdir -p "$DEFAULT_OUTPUT_DIR"
            
            log_message "INFO" "开始压缩: $target -> $compressed_path"
            
            if tar -czf "$compressed_path" "$target" 2>/dev/null; then
                log_message "INFO" "压缩成功: $compressed_path"
                echo "压缩后大小: $(du -h "$compressed_path" | cut -f1)"
            else
                log_message "ERROR" "压缩失败"
                return 1
            fi
            ;;
            
        "info")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要查看信息的文件"
                return 1
            fi
            
            if [[ ! -e "$target" ]]; then
                log_message "ERROR" "文件不存在: $target"
                return 1
            fi
            
            echo -e "${CYAN}文件信息:${NC}"
            echo "路径: $target"
            echo "大小: $(du -h "$target" | cut -f1)"
            echo "权限: $(ls -la "$target" | awk '{print $1}')"
            echo "所有者: $(ls -la "$target" | awk '{print $3}')"
            echo "修改时间: $(ls -la "$target" | awk '{print $6, $7, $8}')"
            
            if [[ -f "$target" ]]; then
                echo "文件类型: $(file "$target" | cut -d: -f2-)"
                echo "行数: $(wc -l < "$target" 2>/dev/null || echo "无法统计")"
            fi
            ;;
            
        *)
            log_message "ERROR" "未知的文件操作: $operation"
            echo "可用的文件操作: backup, compress, info"
            return 1
            ;;
    esac
}

# 网络操作功能
network_operations() {
    local operation="$1"
    local target="$2"
    
    case "$operation" in
        "ping")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要ping的目标"
                return 1
            fi
            
            log_message "INFO" "开始ping: $target"
            
            if ping -c 4 "$target" 2>/dev/null; then
                log_message "INFO" "ping成功"
            else
                log_message "ERROR" "ping失败"
                return 1
            fi
            ;;
            
        "download")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要下载的URL"
                return 1
            fi
            
            local filename=$(basename "$target")
            local output_path="$DEFAULT_OUTPUT_DIR/$filename"
            
            mkdir -p "$DEFAULT_OUTPUT_DIR"
            
            log_message "INFO" "开始下载: $target -> $output_path"
            
            if wget -O "$output_path" "$target" 2>/dev/null; then
                log_message "INFO" "下载成功: $output_path"
                echo "文件大小: $(du -h "$output_path" | cut -f1)"
            else
                log_message "ERROR" "下载失败"
                return 1
            fi
            ;;
            
        "check")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要检查的URL"
                return 1
            fi
            
            log_message "INFO" "检查网络连接: $target"
            
            local response_code=$(curl -s -o /dev/null -w "%{http_code}" "$target" 2>/dev/null)
            
            if [[ "$response_code" == "200" ]]; then
                log_message "INFO" "连接正常 (HTTP $response_code)"
            else
                log_message "WARN" "连接异常 (HTTP $response_code)"
            fi
            ;;
            
        *)
            log_message "ERROR" "未知的网络操作: $operation"
            echo "可用的网络操作: ping, download, check"
            return 1
            ;;
    esac
}

# 系统操作功能
system_operations() {
    local operation="$1"
    
    case "$operation" in
        "info")
            echo -e "${CYAN}系统信息:${NC}"
            echo "操作系统: $(uname -s)"
            echo "内核版本: $(uname -r)"
            echo "主机名: $(hostname)"
            echo "架构: $(uname -m)"
            echo "用户: $(whoami)"
            echo "当前目录: $(pwd)"
            echo "Shell: $SHELL"
            ;;
            
        "disk")
            echo -e "${CYAN}磁盘使用情况:${NC}"
            df -h
            ;;
            
        "memory")
            echo -e "${CYAN}内存使用情况:${NC}"
            if command -v free &> /dev/null; then
                free -h
            else
                log_message "WARN" "free命令不可用"
            fi
            ;;
            
        "processes")
            echo -e "${CYAN}进程信息:${NC}"
            ps aux --sort=-%cpu | head -10
            ;;
            
        "uptime")
            echo -e "${CYAN}系统运行时间:${NC}"
            uptime
            ;;
            
        *)
            log_message "ERROR" "未知的系统操作: $operation"
            echo "可用的系统操作: info, disk, memory, processes, uptime"
            return 1
            ;;
    esac
}

# 备份操作功能
backup_operations() {
    local operation="$1"
    local target="$2"
    
    case "$operation" in
        "create")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要备份的目录"
                return 1
            fi
            
            if [[ ! -d "$target" ]]; then
                log_message "ERROR" "目录不存在: $target"
                return 1
            fi
            
            local backup_name="backup_$(date +%Y%m%d_%H%M%S)_$(basename "$target")"
            local backup_path="$DEFAULT_OUTPUT_DIR/$backup_name.tar.gz"
            
            mkdir -p "$DEFAULT_OUTPUT_DIR"
            
            log_message "INFO" "开始创建备份: $target -> $backup_path"
            
            if tar -czf "$backup_path" -C "$(dirname "$target")" "$(basename "$target")" 2>/dev/null; then
                log_message "INFO" "备份创建成功: $backup_path"
                echo "备份大小: $(du -h "$backup_path" | cut -f1)"
            else
                log_message "ERROR" "备份创建失败"
                return 1
            fi
            ;;
            
        "list")
            if [[ -d "$DEFAULT_OUTPUT_DIR" ]]; then
                echo -e "${CYAN}备份列表:${NC}"
                ls -la "$DEFAULT_OUTPUT_DIR"/*.tar.gz 2>/dev/null | while read -r line; do
                    if [[ -n "$line" ]]; then
                        echo "$line"
                    fi
                done
            else
                log_message "INFO" "备份目录不存在"
            fi
            ;;
            
        "restore")
            if [[ -z "$target" ]]; then
                log_message "ERROR" "请指定要恢复的备份文件"
                return 1
            fi
            
            if [[ ! -f "$target" ]]; then
                log_message "ERROR" "备份文件不存在: $target"
                return 1
            fi
            
            local restore_dir="$DEFAULT_OUTPUT_DIR/restored_$(date +%Y%m%d_%H%M%S)"
            
            mkdir -p "$restore_dir"
            
            log_message "INFO" "开始恢复备份: $target -> $restore_dir"
            
            if tar -xzf "$target" -C "$restore_dir" 2>/dev/null; then
                log_message "INFO" "备份恢复成功: $restore_dir"
            else
                log_message "ERROR" "备份恢复失败"
                return 1
            fi
            ;;
            
        *)
            log_message "ERROR" "未知的备份操作: $operation"
            echo "可用的备份操作: create, list, restore"
            return 1
            ;;
    esac
}

# 监控操作功能
monitor_operations() {
    local operation="$1"
    local duration="${2:-60}"
    
    case "$operation" in
        "cpu")
            echo -e "${CYAN}CPU使用率监控 (${duration}秒):${NC}"
            for ((i=1; i<=duration; i++)); do
                local cpu_usage=$(top -bn1 | grep "Cpu(s)" | awk '{print $2}' | cut -d'%' -f1)
                echo "[$(date '+%H:%M:%S')] CPU使用率: ${cpu_usage}%"
                sleep 1
            done
            ;;
            
        "memory")
            echo -e "${CYAN}内存使用率监控 (${duration}秒):${NC}"
            for ((i=1; i<=duration; i++)); do
                if command -v free &> /dev/null; then
                    local mem_usage=$(free | grep Mem | awk '{printf "%.1f", $3/$2 * 100.0}')
                    echo "[$(date '+%H:%M:%S')] 内存使用率: ${mem_usage}%"
                fi
                sleep 1
            done
            ;;
            
        "disk")
            echo -e "${CYAN}磁盘使用率监控 (${duration}秒):${NC}"
            for ((i=1; i<=duration; i++)); do
                local disk_usage=$(df / | tail -1 | awk '{print $5}' | cut -d'%' -f1)
                echo "[$(date '+%H:%M:%S')] 磁盘使用率: ${disk_usage}%"
                sleep 1
            done
            ;;
            
        "network")
            echo -e "${CYAN}网络连接监控 (${duration}秒):${NC}"
            for ((i=1; i<=duration; i++)); do
                local connections=$(netstat -an | wc -l)
                echo "[$(date '+%H:%M:%S')] 网络连接数: $connections"
                sleep 1
            done
            ;;
            
        *)
            log_message "ERROR" "未知的监控操作: $operation"
            echo "可用的监控操作: cpu, memory, disk, network"
            return 1
            ;;
    esac
}

# 测试功能
test_operations() {
    local operation="$1"
    
    case "$operation" in
        "all")
            echo -e "${CYAN}运行所有测试...${NC}"
            test_operations "dependencies"
            test_operations "file"
            test_operations "network"
            test_operations "system"
            ;;
            
        "dependencies")
            echo -e "${CYAN}测试依赖...${NC}"
            if check_dependencies; then
                log_message "INFO" "所有依赖检查通过"
            else
                log_message "WARN" "部分依赖缺失"
            fi
            ;;
            
        "file")
            echo -e "${CYAN}测试文件操作...${NC}"
            local test_file="test_file_$(date +%s).txt"
            echo "测试内容" > "$test_file"
            
            if [[ -f "$test_file" ]]; then
                log_message "INFO" "文件创建测试通过"
                rm "$test_file"
            else
                log_message "ERROR" "文件创建测试失败"
            fi
            ;;
            
        "network")
            echo -e "${CYAN}测试网络连接...${NC}"
            if ping -c 1 8.8.8.8 &>/dev/null; then
                log_message "INFO" "网络连接测试通过"
            else
                log_message "WARN" "网络连接测试失败"
            fi
            ;;
            
        "system")
            echo -e "${CYAN}测试系统功能...${NC}"
            if [[ -n "$(uname)" ]]; then
                log_message "INFO" "系统信息获取测试通过"
            else
                log_message "ERROR" "系统信息获取测试失败"
            fi
            ;;
            
        *)
            log_message "ERROR" "未知的测试操作: $operation"
            echo "可用的测试操作: all, dependencies, file, network, system"
            return 1
            ;;
    esac
}

# 主函数
main() {
    # 解析命令行参数
    local command=""
    local operation=""
    local target=""
    local duration=""
    
    # 全局选项
    DEBUG_MODE="false"
    QUIET_MODE="false"
    LOG_ENABLED="false"
    
    while [[ $# -gt 0 ]]; do
        case "$1" in
            -h|--help)
                show_help
                exit 0
                ;;
            -v|--version)
                show_version
                exit 0
                ;;
            -d|--debug)
                DEBUG_MODE="true"
                shift
                ;;
            -q|--quiet)
                QUIET_MODE="true"
                shift
                ;;
            -l|--log)
                LOG_ENABLED="true"
                shift
                ;;
            -c|--config)
                CONFIG_FILE="$2"
                shift 2
                ;;
            file|network|system|backup|monitor|test)
                command="$1"
                shift
                if [[ $# -gt 0 ]]; then
                    operation="$1"
                    shift
                    if [[ $# -gt 0 ]]; then
                        target="$1"
                        shift
                        if [[ $# -gt 0 ]]; then
                            duration="$1"
                            shift
                        fi
                    fi
                fi
                break
                ;;
            *)
                log_message "ERROR" "未知参数: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # 加载配置
    load_config
    
    # 检查依赖
    check_dependencies
    
    # 执行命令
    case "$command" in
        "file")
            file_operations "$operation" "$target"
            ;;
        "network")
            network_operations "$operation" "$target"
            ;;
        "system")
            system_operations "$operation"
            ;;
        "backup")
            backup_operations "$operation" "$target"
            ;;
        "monitor")
            monitor_operations "$operation" "$duration"
            ;;
        "test")
            test_operations "$operation"
            ;;
        "")
            log_message "ERROR" "请指定要执行的命令"
            show_help
            exit 1
            ;;
        *)
            log_message "ERROR" "未知命令: $command"
            show_help
            exit 1
            ;;
    esac
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi 