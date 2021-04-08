
#ifndef EXTENSION_H
#define EXTENSION_H

#include <functional>
#include <mutex>
#include <thread>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

typedef std::function<int(char const* name, char const* function, char const* data)> callback_t;

class Extension
{
public:
    Extension();

    void register_callback(callback_t cb);
    int call(char* output, int output_sz, const char* function, const char** argv, int argc);

private:
    curlpp::Cleanup m_cleanup;

    callback_t m_callback;
};

#endif // EXTENSION_H
