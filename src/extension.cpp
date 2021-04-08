
#include "extension.h"

Extension::Extension()
{

}

void Extension::register_callback(callback_t cb)
{
    m_callback = cb;
}

int Extension::call(char* output, int output_sz, const char* function, const char** argv, int argc)
{

}