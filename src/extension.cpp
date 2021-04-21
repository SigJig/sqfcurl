
#include "extension.h"
#include "parser.h"
#include "error.h"
#include "utils.h"

#include <utility>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>


Extension::Extension()
    : m_threadcount{4}, m_callback{nullptr}
{
    logger = spdlog::basic_logger_mt("basic_logger", "sqfcurl.log");
    logger->set_level(spdlog::level::trace);

    init_asio();
}

Extension::~Extension()
{
    clear_asio();
}

void Extension::init_asio()
{
    m_work.reset(new boost::asio::io_service::work(m_io_service));
    for (int i = 0; i < m_threadcount; i++)
    {
        m_threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
    }

    logger->info("Created {} threads", m_threadcount);
}

void Extension::clear_asio()
{
    m_work.reset(nullptr);
    m_io_service.stop();

    m_threadpool.join_all();
}

void Extension::register_callback(callback_raw_t cb)
{
    if (m_callback)
    {
        logger->info("Attempted to register callback when already registered");
        return;
    }

    std::lock_guard<std::mutex> guard(m_cb_mutex);
    m_callback = cb;
    logger->info("Callback registered");
}

int Extension::callback(const char* function, int queue_id, int status, const std::string& data) const
{
    std::string result = "[" + std::to_string(status) + "," + std::to_string(queue_id) + "," + data + "]";

    return (m_callback)(EXTENSION_NAME, function, result.c_str());
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

        if (argc < static_cast<int>(RawIndex::URL))
        {
            throw CallError(
                ErrorCode::BAD_PARAM_SZ,
                "Not enough arguments (expected at least 1, got " + std::to_string(argc) + ")");
        }

        int queue_id = std::stoi(argv[RawIndex::ID]);
        std::string method = sanitize_input(argv[RawIndex::METHOD]);
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
                    method, url,
                    sanitize_input(argv[RawIndex::BODY]),
                    headers);
            }
            else
            {
                req = std::make_shared<Request>(method, url, headers);
            }
        }
        else
        {
            req = std::make_shared<Request>(method, url);
        }

        std::lock_guard<std::mutex> guard(m_cb_mutex);
        boost::function<int(int, const std::string&)> cb = boost::bind(
            &Extension::callback, this, function, queue_id, _1, _2);

        m_io_service.post(boost::bind(&Request::perform, req, cb, logger));
    }
    catch (const CallError& e)
    {
        string_copy(output, output_sz-1, e.what());
        return static_cast<int>(e.error_code);
    }

    return 0;
}

