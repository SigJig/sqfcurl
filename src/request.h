
#ifndef REQUEST_H
#define REQUEST_H

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

#include <list>
#include <vector>
#include <string>
#include <memory>
#include <boost/function.hpp>

class Request
{
public:
    Request(std::string method, std::string endpoint);
    Request(std::string method, std::string endpoint, std::string body);
    Request(std::string method, std::string endpoint, std::string body, cpr::Header headers);
    Request(std::string method, std::string endpoint, cpr::Header headers);

    void perform(
        const boost::function<int(int, const std::string&)>& cb,
        std::shared_ptr<spdlog::logger> logger
    );

private:
    std::string m_method;
    cpr::Url m_url;
    cpr::Body m_body;
    cpr::Header m_headers;

    std::string encode_body();

};

#endif // REQUEST_H