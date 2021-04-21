
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

typedef int(*callback_raw_t)(char const*, char const*, char const*);
typedef boost::function<int(int, const std::string&)> callback_t;

class Extension
{
public:
    Extension();
    ~Extension();

    void register_callback(callback_raw_t cb);

    int callback(const char* function, int queue_id, int status, const std::string& data) const;
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

    std::shared_ptr<spdlog::logger> logger;
private:
    void init_asio();
    void clear_asio();

    std::unique_ptr<boost::asio::io_service::work> m_work;
    boost::asio::io_service m_io_service;
    boost::thread_group m_threadpool;

    callback_raw_t m_callback;
    std::mutex m_cb_mutex;

    uint8_t m_threadcount;
};

#endif // EXTENSION_H
