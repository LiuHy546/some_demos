#!/bin/bash
# 用法: ./ultimate_pack.sh <目标.o> [输出库名.a] [项目根目录]

set -eo pipefail

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# 日志函数
log_info() { echo -e "${BLUE}ℹ️ $1${NC}"; }
log_success() { echo -e "${GREEN}✅ $1${NC}"; }
log_warning() { echo -e "${YELLOW}⚠️ $1${NC}"; }
log_error() { echo -e "${RED}❌ $1${NC}"; }

# 配置参数（可通过参数或环境变量覆盖）
: ${PROJECT_ROOT:="${3:-$(pwd)}"}
: ${CACHE_BASE_DIR:="${TMPDIR:-/tmp}/symbol_cache"}
: ${MAX_DEPTH:=20}                      # 最大目录搜索深度
: ${SEARCH_THREADS:=$(nproc)}           # 并行线程数
: ${SYSTEM_SYMBOLS:="^$|^_|@|^malloc|^free|^printf|^_GLOBAL_"} # 系统符号过滤
: ${KEEP_CPP_SYMBOLS:=1}                # 是否保留C++符号(默认保留)
: ${VERBOSE_FILTER:=0}                  # 显示过滤详情(调试用)

# 初始化全局变量
TARGET_OBJ=$(realpath "$1")
OUTPUT_LIB=$(realpath "${2:-"libultimate.a"}")
CACHE_DIR="$CACHE_BASE_DIR/$(echo "$PROJECT_ROOT" | md5sum | cut -d' ' -f1)_cache"
DEPENDENCY_LOG="${CACHE_DIR}/dependency.log"

# 增强型符号过滤器
filter_symbols() {
    local sym=$1
    local from=$2
    
    # 基本系统符号过滤
    if [[ "$sym" =~ $SYSTEM_SYMBOLS ]]; then
        [[ $VERBOSE_FILTER -eq 1 ]] && echo "  [FILTER] 系统符号: $sym (来自 $from)"
        return 1
    fi
    
    # C++符号处理
    if [[ $KEEP_CPP_SYMBOLS -eq 0 ]] && [[ "$sym" =~ ^_Z ]]; then
        [[ $VERBOSE_FILTER -eq 1 ]] && echo "  [FILTER] C++符号: $sym (来自 $from)"
        return 1
    fi
    
    # 有效符号
    [[ $VERBOSE_FILTER -eq 1 ]] && echo "  [KEEP] 保留符号: $sym (来自 $from)"
    return 0
}

# 1. 确保缓存目录存在
ensure_cache_dir() {
    echo "初始化缓存目录..."
    
    local retries=3
    while ((retries-- > 0)); do
        if mkdir -p "$CACHE_DIR"; then
            echo "缓存目录: $CACHE_DIR"
            return 0
        fi
        
        echo "警告: 无法创建缓存目录 $CACHE_DIR"
        if ((retries > 0)); then
            echo "尝试备用缓存位置... (剩余重试: $retries)"
            CACHE_BASE_DIR="/dev/shm/symbol_cache"
            CACHE_DIR="$CACHE_BASE_DIR/$(echo "$PROJECT_ROOT" | md5sum | cut -d' ' -f1)_cache"
        fi
    done
    
    echo "错误: 无法创建任何缓存目录"
    return 1
}

# 2. 构建全局符号缓存
build_symbol_cache() {
    log_info "构建符号缓存（深度: $MAX_DEPTH, 线程: $SEARCH_THREADS）..."
    # 清空旧缓存
    rm -rf "${CACHE_DIR:?}"/*.cache 2>/dev/null || true
    # 查找所有.o文件
    local obj_count=0
    obj_count=$(find "$PROJECT_ROOT" -maxdepth "$MAX_DEPTH" -name "*.o" -print 2>/dev/null | wc -l)
    log_info "发现 $obj_count 个目标文件，开始分析..."
    time {
        find "$PROJECT_ROOT" -maxdepth "$MAX_DEPTH" -name "*.o" -print0 2>/dev/null | \
            xargs -0 -P "$SEARCH_THREADS" -I {} bash -c '
                obj={}
                base=$(basename "$obj")
                cache_file="$0/${base}.cache"
                # 检查是否已缓存
                if [[ -f "$cache_file" && "$obj" -ot "$cache_file" ]]; then
                    exit 0
                fi
                # 使用nm提取符号
                if nm "$obj" 2>/dev/null | \
                    awk -v obj="$obj" '\''
                        / [TtWw] / { print $3 "::T::" obj }
                        / [Uu] / { print $2 "::U::" obj }
                    '\'' > "$cache_file"; then
                    echo "  📝 缓存 $base"
                else
                    rm -f "$cache_file"
                    echo "  ❗ 失败 $base"
                fi
            ' "$CACHE_DIR"
        # 合并缓存
        cat "$CACHE_DIR"/*.cache 2>/dev/null | sort -u > "$CACHE_DIR/global.cache"
        local cache_count=$(wc -l < "$CACHE_DIR/global.cache" 2>/dev/null || echo 0)
        log_success "缓存完成 (${cache_count} 条符号记录)"
    } 2>&1 | sed 's/^/  /'
}

# 3. 查找符号提供者
find_symbol_providers() {
    local sym=$1
    grep "^${sym}::T::" "$CACHE_DIR/global.cache" 2>/dev/null | cut -d: -f3 | sort -u
}

# 4. 主依赖解析
resolve_dependencies() {
    declare -A all_objects
    declare -A processed_objs
    declare -a pending_objs
    pending_objs=("$TARGET_OBJ")
    while [[ ${#pending_objs[@]} -gt 0 ]]; do
        obj="${pending_objs[0]}"
        pending_objs=("${pending_objs[@]:1}")
        [[ -n "${processed_objs[$obj]}" ]] && continue
        processed_objs["$obj"]=1
        all_objects["$obj"]=1
        log_info "分析依赖: $obj" >&2
        syms=()
        while IFS= read -r sym; do
            sym=$(echo "$sym" | tr -d '[:space:]')
            [[ -z "$sym" ]] && continue
            if [[ "$sym" =~ $SYSTEM_SYMBOLS ]]; then
                continue
            fi
            syms+=("$sym")
        done < <(nm "$obj" 2>/dev/null | awk '/ U /{print $2}')
        for sym in "${syms[@]}"; do
            log_info "  未定义符号: [$sym]" >&2
            while IFS= read -r prov; do
                prov=$(echo "$prov" | tr -d '[:space:]')
                [[ -z "$prov" ]] && continue
                if [[ -n "$prov" && -z "${processed_objs[$prov]}" && -z "${all_objects[$prov]}" ]]; then
                    log_info "  发现依赖: $prov (for symbol $sym)" >&2
                    pending_objs+=("$prov")
                fi
            done < <(grep "^${sym}::T::" "$CACHE_DIR/global.cache" | awk -F '::' '{print $3}')
        done
    done
    printf '%s\n' "${!all_objects[@]}" | grep '\.o$'
}

# 5. 验证符号完整性
verify_symbols() {
    local missing=0
    echo "验证符号完整性..."
    
    # 检查所有未定义符号是否都有定义
    while IFS= read -r line; do
        sym=$(echo "$line" | cut -d: -f1)
        obj=$(echo "$line" | cut -d: -f3)
        if filter_symbols "$sym" "$obj"; then
            if ! grep -q "^${sym}::T::" "$CACHE_DIR/global.cache"; then
                echo "  未解决符号: ${sym} (来自 $(realpath --relative-to="$PROJECT_ROOT" "$obj"))"
                ((missing++))
            fi
        fi
    done < <(grep "::U::" "$CACHE_DIR/global.cache")
    
    [[ $missing -eq 0 ]] && echo "所有符号均已解决" || \
    echo "警告: 发现 ${missing} 个未解决符号"
}

# 主流程
main() {
    # 参数检查
    if [[ $# -lt 1 ]]; then
        echo "用法: $0 <目标.o> [输出库名.a] [项目根目录]"
        exit 1
    fi
    
    if [[ ! -f "$TARGET_OBJ" ]]; then
        echo "错误: 目标文件不存在: $TARGET_OBJ"
        exit 1
    fi

    # 初始化
    echo -e "\n项目根目录: ${PROJECT_ROOT}"
    ensure_cache_dir
    
    # 构建缓存
    if ! build_symbol_cache; then
        echo "错误: 符号缓存构建失败，退出"
        exit 1
    fi
    
    # 解析依赖
    echo "解析依赖关系..."
    ALL_OBJECTS=$(resolve_dependencies | sort -u)
    
    # 验证
    verify_symbols
    
    # 打包
    local obj_count=$(echo "$ALL_OBJECTS" | grep '\.o$' | wc -l)
    echo "打包结果 (共 ${obj_count} 个.o文件):"
    echo "$ALL_OBJECTS" | grep '\.o$' | while read -r obj; do
        if [[ -f $obj ]]; then
            echo "  $(realpath --relative-to=$PROJECT_ROOT $obj)"
        fi
    done
    
    if ! ar rcs "$OUTPUT_LIB" $(echo "$ALL_OBJECTS" | grep '\.o$'); then
        echo "错误: 打包静态库失败"
        exit 1
    fi
    
    echo "生成静态库: ${OUTPUT_LIB}"
    echo "库内容验证:"
    ar t "$OUTPUT_LIB" | head -n 20 | sed 's/^/  /'
    [[ $(ar t "$OUTPUT_LIB" | wc -l) -gt 20 ]] && echo "  ... (共 $(ar t "$OUTPUT_LIB" | wc -l) 个文件)"
}

# 执行
main "$@"