
#ifndef EXTENSION_H
#define EXTENSION_H

#include <functional>
#include <memory>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
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

private:
    curlpp::Cleanup m_cleanup;

    std::unique_ptr<boost::asio::io_service::work> m_work;
    boost::asio::io_service m_io_service;
    boost::thread_group m_threadpool;

    callback_t m_callback;

    std::shared_ptr<Request> make_request(std::string method, std::string endpoint, std::string body);
};

#endif // EXTENSION_H
