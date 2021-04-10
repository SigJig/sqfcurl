
#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <exception>

enum class ErrorCode
{
    PARAMS_PARSE_ERROR=1,
    BAD_INITIALIZATION=2,
    // Codes from wiki
    BAD_PARAM_SZ=101,
    BAD_PARAM_TYPE=102,
    BAD_PARAM_TOOMANY=201,
    TAKES_TOO_LONG=301
};

class CallError : public std::runtime_error
{
public:
    CallError(ErrorCode code, const std::string& message)
        : error_code{code}, std::runtime_error(message)
    {  }

    ErrorCode error_code;
};

#endif // ERROR_H