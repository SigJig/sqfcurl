
#ifndef EXTENSION_H
#define EXTENSION_H

#include <functional>
#include <memory>
#include <mutex>

#include <boost/asio/io_service.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <spdlog/spdlog.h>

#include "request.h"

using namespace boost::placeholders;

typedef std::function<int(char const* name, char const* function, char const* data)> callback_raw_t;
typedef boost::function<int(int, const std::string&)> callback_t;

class Extension
{
public:
    Extension();
    ~Extension();

    void register_callback(const callback_raw_t& cb);
    void init_asio();
    int call(char* output, int output_sz, const char* function, const char** argv, int argc);

    // Index of each element when recieved raw from SQF
    // When called from SQF, the request would look something like (after parsing)
    // [ "0", "POST", "/api/endpoint", "[""[ ""myHeader"", ""header"" ]""]", "[ 1, 2, 3 ]" ]
    //   ID   METHOD       URL                      HEADERS                       BODY
    enum RawIndex
    {
        ID = 0,
        METHOD,
        URL,
        HEADERS,
        BODY
    };

    std::shared_ptr<spdlog::logger> m_logger;
private:
    std::unique_ptr<boost::asio::io_service::work> m_work;
    boost::asio::io_service m_io_service;
    boost::thread_group m_threadpool;

    boost::function<int(const char*, int, int, const std::string&)> m_callback;
    std::mutex m_cb_mutex;

    uint8_t m_threadcount;
};

#endif // EXTENSION_H
