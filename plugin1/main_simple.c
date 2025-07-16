#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

int main() {
    int value = 42; // 默认值
#ifdef _WIN32
    HMODULE handle = LoadLibraryA("plugin_value_plugin.dll");
    if (handle) {
        typedef int (*get_value_func)(void);
        get_value_func plugin_get_value = (get_value_func)GetProcAddress(handle, "plugin_get_value");
        if (plugin_get_value) {
            value = plugin_get_value();
            printf("[INFO] Plugin loaded, value from plugin: %d\n", value);
        } else {
            printf("[INFO] Plugin loaded, but symbol not found.\n");
        }
        FreeLibrary(handle);
    } else {
        printf("[INFO] Plugin not found, using default value.\n");
    }
#else
    void* handle = dlopen("./plugin_value_plugin.so", RTLD_LAZY);
    if (handle) {
        typedef int (*get_value_func)(void);
        get_value_func plugin_get_value = (get_value_func)dlsym(handle, "plugin_get_value");
        if (plugin_get_value) {
            value = plugin_get_value();
            printf("[INFO] Plugin loaded, value from plugin: %d\n", value);
        } else {
            printf("[INFO] Plugin loaded, but symbol not found.\n");
        }
        dlclose(handle);
    } else {
        printf("[INFO] Plugin not found, using default value.\n");
    }
#endif
    printf("Current value: %d\n", value);
    return 0;
} 