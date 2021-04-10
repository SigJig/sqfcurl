
#ifndef EXTENSION_H
#define EXTENSION_H

#include <functional>
#include <memory>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include "request.h"

typedef std::function<int(char const* name, char const* function, char const* data)> callback_t;

class Extension
{
public:
    Extension();
    ~Extension();

    void register_callback(callback_t cb);
    int call(char* output, int output_sz, const char* function, const char** argv, int argc);

    void test_async();
    void test_sync();

    // Index of each element when recieved raw from SQF
    // When called from SQF, the request would look something like (after parsing)
    // [ "/api/endpoint", "[""[ ""myHeader"", ""header"" ]""]", "[ 1, 2, 3 ]" ]
    //         URL                HEADERS               HEADERS
    enum RawIndex
    {
        URL = 0,
        HEADERS,
        BODY
    };

private:
    std::unique_ptr<boost::asio::io_service::work> m_work;
    boost::asio::io_service m_io_service;
    boost::thread_group m_threadpool;

    callback_t m_callback;

    std::shared_ptr<Request> make_request(std::string method, std::string endpoint, std::string body);
};

#endif // EXTENSION_H
