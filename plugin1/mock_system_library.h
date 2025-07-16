#ifndef MOCK_SYSTEM_LIBRARY_H
#define MOCK_SYSTEM_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

// 获取系统默认值
const char* get_system_value(void);

// 获取系统默认数字
int get_system_number(void);

// 配置系统
void configure_system(const char* config);

// 初始化系统
int init_system(void);

// 清理系统
void cleanup_system(void);

#ifdef __cplusplus
}
#endif

#endif // MOCK_SYSTEM_LIBRARY_H 