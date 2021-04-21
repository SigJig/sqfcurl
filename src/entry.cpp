
#include <memory>

#include "extension.h"
#include "utils.h"

Extension* ext;

#ifdef __GNUC__

#define STDCALL

extern "C"
{
    __attribute__((dllexport))
    int RVExtensionArgs(char* output, int output_sz, const char* function, const char** argv, int argc);

    __attribute__((dllexport))
    void RVExtensionRegisterCallback(callback_raw_t cb);

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


#elif _MSC_VER

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            ext = new Extension();
            break;
        case DLL_PROCESS_DETACH:
            delete ext;
            break;
    }

    return TRUE;
}

extern "C"
{
    #define STDCALL __stdcall

    __declspec(dllexport) void __stdcall RVEXtensionVersion(char* output, int output_sz);
    __declspec(dllexport) void __stdcall RVExtensionRegisterCallback(callback_raw_t cb);
    __declspec(dllexport) int __stdcall RVExtensionArgs(char* output, int output_sz, const char* function, const char** argv, int argc);
}

#endif

int STDCALL RVExtensionArgs(char* output, int output_sz, const char* function, const char** argv, int argc)
{
    return ext->call(output, output_sz, function, argv, argc);
}

void STDCALL RVExtensionRegisterCallback(callback_raw_t cb)
{
    //auto ptr = std::make_unique<callback_raw_t>(cb);
    ext->logger->info("Registering callback");
    ext->register_callback(cb);
}

void STDCALL RVExtensionVersion(char* output, int output_sz)
{
    string_copy(output, output_sz-1, "Version 1.0.0");
}