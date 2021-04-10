
#include <iostream>

#include "extension.h"

int main()
{
    Extension ext;
    //ext.test_async();

    //auto vec = parse_array("    ['12''3218  '  , '123\"\"12321', \"  fu\"\"ck\"]   ");

    //for (auto s : vec)
    //{
    //    std::cout << s << std::endl;
    //}

    const char* l[3] = {
        "\"http://127.0.0.1:8000/gangs\"",
        "[ \"[\"\"testheader\"\", \"\"1\"\"]\" ]",
        "{\"name\": \"test\", \"creator\": \"21784218421822222\"}"
    };
    ext.call("", 0, "POST", l, 3);

    return 0;
}