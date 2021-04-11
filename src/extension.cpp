
#include "extension.h"
#include "parser.h"
#include "error.h"

#include <cstring>
#include <utility>
#include <spdlog/sinks/rotating_file_sink.h>

#include <iostream>

Extension::Extension()
    : m_threadcount{4}
{
    m_logger = spdlog::rotating_logger_mt("file_logger", "sqfcurl.log", 1048576 * 5, 3);

    init_asio();
}

Extension::~Extension()
{
    m_io_service.stop();
    m_threadpool.join_all();
}

void Extension::init_asio()
{
    m_work.reset(new boost::asio::io_service::work(m_io_service));
    for (int i = 0; i < m_threadcount; i++)
    {
        m_threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
    }

    m_logger->info("Created {} threads", m_threadcount);
}

void Extension::register_callback(const callback_t& cb)
{
    std::lock_guard<std::mutex> guard(m_cb_mutex);
    m_callback = cb;
    m_logger->info("Callback registered");
}

int Extension::call(char* output, int output_sz, const char* function, const char** argv, int argc)
{
    try
    {
        {
            std::lock_guard<std::mutex> guard(m_cb_mutex);
            if (!m_callback)
            {
                throw CallError(ErrorCode::BAD_INITIALIZATION, "Callback has not been assigned");
            }
        }

        if (argc < 1)
        {
            throw CallError(
                ErrorCode::BAD_PARAM_SZ,
                "Not enough arguments (expected at least 1, got " + std::to_string(argc) + ")");
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
                    throw CallError(ErrorCode::PARAMS_PARSE_ERROR,
                        std::string("Invalid size for header key-value pair (expected 2, got " +
                        std::to_string(pair.size()) + ")"
                    ));
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

        std::lock_guard<std::mutex> guard(m_cb_mutex);
        m_io_service.post(boost::bind(&Request::perform, req, m_callback));
    }
    catch (const CallError& e)
    {
        std::strncpy(output, e.what(), output_sz-1);
        return static_cast<int>(e.error_code);
    }

    return 0;
}

