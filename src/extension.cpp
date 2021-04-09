
#include "extension.h"

#include <utility>

#include <chrono>

Extension::Extension()
{
    m_work.reset(new boost::asio::io_service::work(m_io_service));
    for (int i = 0; i < 4; i++)
    {
        m_threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
    }

    std::cout << "Created 4 threads" << std::endl;
}

Extension::~Extension()
{
    m_io_service.stop();
    m_threadpool.join_all();
}

void Extension::register_callback(callback_t cb)
{
    m_callback = cb;
}

int Extension::call(char* output, int output_sz, const char* function, const char** argv, int argc)
{
    return 0;
}

void Extension::test()
{
    for (int i = 0; i < 50; i++)
    {
        std::shared_ptr<Request> req = make_request(
            "POST", "http://127.0.0.1:8000/gangs",
            "{\"name\": \"test\", \"creator\": \"21784218421822222\"}");

        m_io_service.post(boost::bind(&Request::perform, req));
        std::cout << i << std::endl;
    }

    m_io_service.run();
}

std::shared_ptr<Request> Extension::make_request(std::string method, std::string endpoint, std::string body)
{
    return std::make_shared<Request>(method, endpoint, body);
}