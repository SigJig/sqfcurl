
#include <cstring>
#include <memory>

#include "extension.h"

#ifdef __GNUC__

Extension* ext;

extern "C"
{
    __attribute__((dllexport))
    int RVExtensionArgs(char* output, int output_sz, const char* function, const char** argv, int argc);

    __attribute__((dllexport))
    void RVExtensionRegisterCallback(callback_t cb);

    __attribute__((dllexport))
    void RVExtensionVersion(char* output, int output_sz);
}

static void __attribute__((constructor))
extension_init()
{
    ext = new Extension();
}

static void __attribute__((destructor))
extension_del()
{
    delete ext;
}

int RVExtensionArgs(char* output, int output_sz, const char* function, const char** argv, int argc)
{
    ext->call(output, output_sz, function, argv, argc);

    return 0;
}

void RVExtensionRegisterCallback(callback_t cb)
{
    ext->register_callback(cb);
}

void RVExtensionVersion(char* output, int output_sz)
{
    std::strncpy(output, "Version 1.0.0", output_sz - 1);
}

#elif _MSC_VER

#endif