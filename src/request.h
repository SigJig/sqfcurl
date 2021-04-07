
#ifndef REQUEST_H
#define REQUEST_H

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <list>
#include <vector>
#include <string>
#include <memory>

class Request
{
public:
    Request(std::string method, std::string endpoint);
    Request(std::string method, std::string endpoint, std::string body);
    Request(std::string method, std::string endpoint, std::string body, std::list<std::string> headers);
    Request(std::string method, std::string endpoint, std::list<std::string> headers);


    // Index of each element when recieved raw from SQF
    // When called from SQF, the request would look something like (after parsing)
    // [ "GET", "/api/endpoint", [ 1, 2, 3 ], [ "myHeader: header" ] ]
    //  METHOD     ENDPOINT         BODY             HEADERS
    enum RawIndex
    {
        METHOD = 0,
        ENDPOINT,
        BODY,
        HEADERS
    };

    void perform();

private:
    std::string m_method;
    std::string m_endpoint;
    std::string m_body;
    std::list<std::string> m_headers;

    std::unique_ptr<curlpp::Easy> m_request;

    void prepare();
    void prep_post();

    std::string encode_body();

};

#endif // REQUEST_H