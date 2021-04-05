
#include "request.h"
#include "parser.h"

#include <iostream>

Request::Request(std::string method, std::string endpoint)
    : m_method(method), m_endpoint(endpoint)
{

}

Request::Request(std::string method, std::string endpoint, std::string body)
    : m_method(method), m_endpoint(endpoint), m_body(body)
{

}

Request::Request(std::string method, std::string endpoint, std::string body, std::list<std::string> headers)
    : m_method(method), m_endpoint(endpoint), m_body(body), m_headers(headers)
{

}

Request::Request(std::string method, std::string endpoint, std::list<std::string> headers)
    : m_method(method), m_endpoint(endpoint), m_headers(headers)
{

}

void Request::perform()
{
    try
    {
        this->prepare();

        m_request.perform();
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (curlpp::LogicError& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Request::prepare()
{
    using namespace curlpp::Options;

    m_request.setOpt(new Url(m_endpoint));
    m_request.setOpt(new HttpHeader(m_headers));

    if (m_method == "POST")
    {
        prep_post();
    }
    else if (m_method == "PUT" || m_method == "PATCH")
    {
        m_request.setOpt(new CustomRequest(m_method));
        prep_post();
    }
    else if (m_method == "DELETE")
    {
        m_request.setOpt(new CustomRequest("DELETE"));
    }
    else if (m_method != "GET")
    {
        // TODO: Log weirdo method
    }
}

void Request::prep_post()
{
    using namespace curlpp::Options;

    m_request.setOpt(new PostFields(m_body));
}