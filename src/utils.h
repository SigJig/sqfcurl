
#ifndef UTILS_H
#define UTILS_H

//#include <cstring>

inline void string_copy(char* dest, int output_sz, const char* str)
{
    #if _MSC_VER
    strncpy_s(dest, output_sz, str, _TRUNCATE);
    #else if __GNUC__
    strncpy(dest, str, output_sz);
    #endif
}

#endif // UTILS_H