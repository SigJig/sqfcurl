
#include "extension.h"
#include "parser.h"

#include <utility>

#include <chrono>
#include <iostream>

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
    
    if (argc < 1)
    {
        // ERROR
    }

    std::string url = sanitize_input(argv[RawIndex::URL]);

    std::shared_ptr<Request> req;

    argc -= 1;

    if (argc >= RawIndex::HEADERS)
    {
        cpr::Header headers;

        for (auto s : parse_array(argv[RawIndex::HEADERS]))
        {
            auto pair = parse_array(s);

            if (pair.size() != 2)
            {
                // ERROR
            }

            headers.emplace(pair[0], pair[1]);
        }

        if (argc >= RawIndex::BODY)
        {
            req = std::make_shared<Request>(
                function, url,
                sanitize_input(argv[RawIndex::BODY]),
                headers);
        }
        else
        {
            req = std::make_shared<Request>(function, url, headers);
        }
    }
    else
    {
        req = std::make_shared<Request>(function, url);
    }

    m_io_service.post(boost::bind(&Request::perform, req));

    return argc;
}

void Extension::test_async()
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 500; i++)
    {
        std::shared_ptr<Request> req = make_request(
            "POST", "http://127.0.0.1:8000/gangs",
            "{\"name\": \"test\", \"creator\": \"21784218421822222\"}");

        //m_io_service.post(boost::bind(&Request::perform, req));
        //std::cout << i << std::endl;
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    //m_io_service.run();
}

void Extension::test_sync()
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; i++)
    {
        auto req = make_request(
            "POST", "http://127.0.0.1:8000/gangs",
            "{\"name\": \"test\", \"creator\": \"21784218421822222\"}");

        //req->perform();
        //std::cout << req->perform().text << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << dur.count() << "ms" << std::endl;
}

std::shared_ptr<Request> Extension::make_request(std::string method, std::string endpoint, std::string body)
{
    return std::make_shared<Request>(method, endpoint, body);
}