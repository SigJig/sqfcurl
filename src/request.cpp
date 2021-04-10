
#include "request.h"
#include "parser.h"

#include <iostream>

Request::Request(std::string method, std::string url)
    : m_method(method), m_url(url)
{
}

Request::Request(std::string method, std::string url, std::string body)
    : m_method(method), m_url(url), m_body(body)
{

}

Request::Request(std::string method, std::string url, std::string body, cpr::Header headers)
    : m_method(method), m_url(url), m_body(body), m_headers(headers)
{

}

Request::Request(std::string method, std::string url, cpr::Header headers)
    : m_method(method), m_url(url), m_headers(headers)
{

}

cpr::Response Request::perform()
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
        //return std::move(res);
    }

    return std::move(res);
}
