
#include <iostream>
#include <string>

#include "extension.h"

int main(int argc, char** argv)
{
    Extension ext;

    callback_raw_t callback = [](const char* name, const char* function, const char* data) -> int {
        std::cout << name << "::" << function << ": " << data << std::endl;
        return 0;
    };

    ext.register_callback(callback);

    std::string first = argc > 1 ? argv[1] : "";
    std::string second = argc > 2 ? argv[2] : "";
    std::string arg = ("[ \"" + first + "\", \"" + second + "\" ]");

    const char* l[5] = {
        "0",
        "POST",
        "\"http://127.0.0.1:8000/gangs\"",
        "[]",
        arg.c_str()
    };

    int output_sz = 1000;
    char output[1000];
    
    std::cout << ext.call(output, output_sz, "oo", l, 5) << std::endl;

    if (output[0] != '\0') std::cout << output << std::endl;

    return 0;
}