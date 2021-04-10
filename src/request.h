
#ifndef REQUEST_H
#define REQUEST_H

#include <cpr/cpr.h>

#include <list>
#include <vector>
#include <string>
#include <memory>

class Request
{
public:
    Request(std::string method, std::string endpoint);
    Request(std::string method, std::string endpoint, std::string body);
    Request(std::string method, std::string endpoint, std::string body, cpr::Header headers);
    Request(std::string method, std::string endpoint, cpr::Header headers);

    cpr::Response perform();

private:
    std::string m_method;
    cpr::Url m_url;
    cpr::Body m_body;
    cpr::Header m_headers;

    std::string encode_body();

};

#endif // REQUEST_H