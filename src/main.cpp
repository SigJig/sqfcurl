
#include <iostream>

#include "extension.h"

int main()
{
    Extension ext;

    callback_t callback = [](const char* name, const char* function, const char* data) -> int {
        std::cout << name << "::" << function << ": " << data << std::endl;
        return 0;
    };

    ext.register_callback(callback);

    const char* l[3] = {
        "\"http://127.0.0.1:8000/gangs\"",
        "[ \"[\"\"testheader\"\", \"\"1\"\"]\" ]",
        "{\"name\": \"test\", \"creator\": \"21784218421822222\"}"
    };

    int output_sz = 1000;
    char output[1000];
    
    std::cout << ext.call(output, output_sz, "POST", l, 3) << std::endl;

    if (output[0] != '\0') std::cout << output << std::endl;

    return 0;
}