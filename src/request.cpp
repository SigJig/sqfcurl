
#include "request.h"
#include "parser.h"
#include "error.h"

#include <iostream>

Request::Request(Extension* ext, std::string method, std::string url)
    : m_ext(ext), m_method(method), m_url(url)
{
}

Request::Request(Extension* ext, std::string method, std::string url, std::string body)
    : m_ext(ext), m_method(method), m_url(url), m_body(body)
{

}

Request::Request(Extension* ext, std::string method, std::string url, std::string body, cpr::Header headers)
    : m_ext(ext), m_method(method), m_url(url), m_body(body), m_headers(headers)
{

}

Request::Request(Extension* ext, std::string method, std::string url, cpr::Header headers)
    : m_ext(ext), m_method(method), m_url(url), m_headers(headers)
{

}

void Request::perform(const boost::function<int(int, const std::string&)>& cb)
{
    try
    {
        cpr::Response res;
        if (m_method == "GET")
        {
            res = cpr::Get(m_url, m_headers);
        }
        else if (m_method == "POST")
        {
            res = cpr::Post(m_url, m_body, m_headers);
        }
        else if (m_method == "PUT")
        {
            res = cpr::Put(m_url, m_body, m_headers);
        }
        else if (m_method == "PATCH")
        {
            res = cpr::Patch(m_url, m_body, m_headers);
        }
        else if (m_method == "DELETE")
        {
            res = cpr::Delete(m_url, m_headers);
        }
        else
        {
            throw CallError(ErrorCode::INVALID_METHOD, "Invalid method " + m_method);
        }

        auto status_code = res.status_code;
        if (status_code < 200 || status_code > 202)
        {
            throw CallError(
                ErrorCode::HTTP_ERROR, std::to_string(static_cast<int>(status_code)));
        }

        cb(0, res.text);
    }
    catch (const CallError& e)
    {
        cb(static_cast<int>(e.error_code), e.what());
    }
}
