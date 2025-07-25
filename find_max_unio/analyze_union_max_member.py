import re
import sys

# 基本类型大小（可根据实际平台调整）
TYPE_SIZES = {
    'char': 1,
    'unsigned char': 1,
    'signed char': 1,
    'short': 2,
    'short int': 2,
    'signed short': 2,
    'signed short int': 2,
    'unsigned short': 2,
    'unsigned short int': 2,
    'int': 4,
    'signed': 4,
    'signed int': 4,
    'unsigned': 4,
    'unsigned int': 4,
    'long': 8,
    'long int': 8,
    'signed long': 8,
    'signed long int': 8,
    'unsigned long': 8,
    'unsigned long int': 8,
    'long long': 8,
    'long long int': 8,
    'signed long long': 8,
    'signed long long int': 8,
    'unsigned long long': 8,
    'unsigned long long int': 8,
    'float': 4,
    'double': 8,
    'long double': 16,
    'void': 1,  # 仅用于指针
}

POINTER_SIZE = 8  # 可根据平台调整

def clean_type(t):
    t = re.sub(r'\b(const|volatile|static|register|extern|struct|union|enum|typedef)\b', '', t)
    t = re.sub(r'\s+', ' ', t)
    return t.strip()

def parse_macros(code):
    macros = {}
    for m in re.finditer(r'#define\s+(\w+)\s+(\d+)', code):
        macros[m.group(1)] = int(m.group(2))
    return macros

def parse_typedefs(code):
    # 创建一个空字典，用于存储typedefs
    typedefs = {}
    # 使用正则表达式查找code中的typedefs
    for m in re.finditer(r'typedef\s+([^;{]+?)\s+(\w+)\s*;', code):
        # 获取typedefs的源类型和别名
        src, alias = m.groups()
        # 将源类型进行清洗，并存储到字典中
        typedefs[alias] = clean_type(src)
    # 返回字典
    return typedefs

def parse_array_sizes(arr, macros):
    if not arr:
        return []
    sizes = []
    for s in re.findall(r'\[([^\]]+)\]', arr):
        s = s.strip()
        if s.isdigit():
            sizes.append(int(s))
        elif s in macros:
            sizes.append(macros[s])
        else:
            sizes.append(1)  # 默认1，或可报错
    return sizes

def parse_structs_unions(code, typedefs, macros):
    # 支持所有常见typedef struct/union格式
    pattern = re.compile(
        r'typedef\s+(struct|union)\s*(\w+)?\s*\{([\s\S]*?)\}\s*(\w+)\s*;',
        re.MULTILINE
    )
    result = {}
    for kind, name, body, alias in pattern.findall(code):
        fields = []
        for line in body.split(';'):
            line = line.strip()
            if not line:
                continue
            # 位域成员
            m_bitfield = re.match(r'(.+?)\s+(\w+)\s*:(\d+)', line)
            if m_bitfield:
                t, var, bitwidth = m_bitfield.groups()
                t = clean_type(t)
                arr_sizes = []
                fields.append((t, var, arr_sizes, int(bitwidth)))
                continue
            # 匿名结构体/联合体成员
            m_anon = re.match(r'(struct|union)\s*\{(.*)\}', line)
            if m_anon:
                anon_kind, anon_body = m_anon.groups()
                anon_fields = []
                for anon_line in anon_body.split(';'):
                    anon_line = anon_line.strip()
                    if not anon_line:
                        continue
                    m2 = re.match(r'(.+?)\s+(\w+)(\[.*\])?', anon_line)
                    if m2:
                        t2, var2, arr2 = m2.groups()
                        t2 = clean_type(t2)
                        arr_sizes = parse_array_sizes(arr2, macros)
                        anon_fields.append((t2, var2, arr_sizes, None))
                fields.extend(anon_fields)
                continue
            # 普通成员
            m = re.match(r'(.+?)\s+(\w+)(\[.*\])?', line)
            if m:
                t, var, arr = m.groups()
                t = clean_type(t)
                arr_sizes = parse_array_sizes(arr, macros)
                fields.append((t, var, arr_sizes, None))
        result[alias] = {'kind': kind, 'fields': fields}
    # 兼容无名字struct/union（如typedef union { ... } Name;）
    pattern2 = re.compile(
        r'typedef\s+(struct|union)\s*\{([\s\S]*?)\}\s*(\w+)\s*;',
        re.MULTILINE
    )
    for kind, body, alias in pattern2.findall(code):
        if alias in result:
            continue
        fields = []
        for line in body.split(';'):
            line = line.strip()
            if not line:
                continue
            m_bitfield = re.match(r'(.+?)\s+(\w+)\s*:(\d+)', line)
            if m_bitfield:
                t, var, bitwidth = m_bitfield.groups()
                t = clean_type(t)
                arr_sizes = []
                fields.append((t, var, arr_sizes, int(bitwidth)))
                continue
            m_anon = re.match(r'(struct|union)\s*\{(.*)\}', line)
            if m_anon:
                anon_kind, anon_body = m_anon.groups()
                anon_fields = []
                for anon_line in anon_body.split(';'):
                    anon_line = anon_line.strip()
                    if not anon_line:
                        continue
                    m2 = re.match(r'(.+?)\s+(\w+)(\[.*\])?', anon_line)
                    if m2:
                        t2, var2, arr2 = m2.groups()
                        t2 = clean_type(t2)
                        arr_sizes = parse_array_sizes(arr2, macros)
                        anon_fields.append((t2, var2, arr_sizes, None))
                fields.extend(anon_fields)
                continue
            m = re.match(r'(.+?)\s+(\w+)(\[.*\])?', line)
            if m:
                t, var, arr = m.groups()
                t = clean_type(t)
                arr_sizes = parse_array_sizes(arr, macros)
                fields.append((t, var, arr_sizes, None))
        result[alias] = {'kind': kind, 'fields': fields}
    return result

def resolve_typedef(t, typedefs):
    t = t.strip()
    while t in typedefs:
        t = typedefs[t]
    return t

def get_alignment(type_name, structs_unions, typedefs):
    t = type_name.strip().replace('*', '')
    t = resolve_typedef(t, typedefs)
    if t in TYPE_SIZES:
        return TYPE_SIZES[t]
    if t in structs_unions:
        info = structs_unions[t]
        if info['kind'] == 'struct':
            return max((get_alignment(f[0], structs_unions, typedefs) for f in info['fields']), default=1)
        elif info['kind'] == 'union':
            return max((get_alignment(f[0], structs_unions, typedefs) for f in info['fields']), default=1)
    return 1

def calc_field_size(field, structs_unions, typedefs):
    t, var, arr_sizes, bitwidth = field
    # 位域
    if bitwidth is not None:
        # 位域大小以字节为单位，向上取整
        return (bitwidth + 7) // 8
    # 指针类型
    if '*' in t:
        base_size = POINTER_SIZE
    else:
        base_size = calc_size(t, structs_unions, typedefs)
    total = base_size
    for s in arr_sizes:
        total *= s
    return total

def calc_size(type_name, structs_unions, typedefs):
    t = type_name.strip()
    # 指针类型
    if '*' in t:
        return POINTER_SIZE
    t = t.replace('*', '').strip()
    t = resolve_typedef(t, typedefs)
    if t in TYPE_SIZES:
        return TYPE_SIZES[t]
    if t in structs_unions:
        info = structs_unions[t]
        if info['kind'] == 'struct':
            return calc_struct_size(info['fields'], structs_unions, typedefs)
        elif info['kind'] == 'union':
            return calc_union_size(info['fields'], structs_unions, typedefs)
    return 0

def calc_struct_size(fields, structs_unions, typedefs):
    offset = 0
    max_align = 1
    bitfield_remain = 0
    bitfield_unit = 0
    for f in fields:
        t, var, arr_sizes, bitwidth = f
        align = get_alignment(t, structs_unions, typedefs)
        max_align = max(max_align, align)
        # 位域特殊处理
        if bitwidth is not None:
            # 假设所有位域都在同一字节/字内，按类型对齐
            unit_size = get_alignment(t, structs_unions, typedefs)
            if bitfield_remain == 0 or bitfield_unit != unit_size:
                # 新的bitfield单元
                if offset % unit_size != 0:
                    offset += unit_size - (offset % unit_size)
                bitfield_remain = unit_size * 8
                bitfield_unit = unit_size
            if bitfield_remain < bitwidth:
                # 需要新单元
                offset += unit_size
                bitfield_remain = unit_size * 8
            bitfield_remain -= bitwidth
            continue
        # 普通成员
        if offset % align != 0:
            offset += align - (offset % align)
        size = calc_field_size(f, structs_unions, typedefs)
        offset += size
        bitfield_remain = 0
        bitfield_unit = 0
    if offset % max_align != 0:
        offset += max_align - (offset % max_align)
    return offset

def calc_union_size(fields, structs_unions, typedefs):
    max_size = 0
    max_align = 1
    for f in fields:
        t, var, arr_sizes, bitwidth = f
        align = get_alignment(t, structs_unions, typedefs)
        max_align = max(max_align, align)
        if bitwidth is not None:
            size = (bitwidth + 7) // 8
        else:
            size = calc_field_size(f, structs_unions, typedefs)
        if size > max_size:
            max_size = size
    if max_size % max_align != 0:
        max_size += max_align - (max_size % max_align)
    return max_size

def find_max_union_members(structs_unions, typedefs):
    for name, info in structs_unions.items():
        if info['kind'] == 'union':
            max_size = 0
            max_member = None
            for f in info['fields']:
                size = calc_field_size(f, structs_unions, typedefs)
                if size > max_size:
                    max_size = size
                    max_member = f
            if max_member:
                t, var, arr_sizes, bitwidth = max_member
                arr_str = ''.join(f'[{s}]' for s in arr_sizes)
                if bitwidth is not None:
                    print(f"Union {name}: 最大成员是 {var} 位域 类型 {t}:{bitwidth}，大小 {max_size} 字节")
                else:
                    print(f"Union {name}: 最大成员是 {var} 类型 {t}{arr_str}，大小 {max_size} 字节")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python analyze_union_max_member.py <your_file.i>")
        sys.exit(1)
    filename = sys.argv[1]
    with open(filename, "r", encoding="utf-8") as f:
        code = f.read()
    macros = parse_macros(code)
    typedefs = parse_typedefs(code)
    structs_unions = parse_structs_unions(code, typedefs, macros)
    find_max_union_members(structs_unions, typedefs) 