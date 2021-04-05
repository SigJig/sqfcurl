
#ifndef EXT_H
#define EXT_H

#include <functional>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

typedef std::function<int(char const* name, char const* function, char const* data)> callback_t;

class Extension
{
public:
    Extension();

    void register_callback(callback_t cb);
    void call(char* output, int output_size, const char* function);

private:
    curlpp::Cleanup m_cleanup;

    callback_t m_callback;
};

#endif // EXT_H
