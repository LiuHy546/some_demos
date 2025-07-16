#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
int plugin_get_value(void) {
    return 123;
} 