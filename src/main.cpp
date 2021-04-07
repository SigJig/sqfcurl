
#include <iostream>

#include "request.h"

#include <curlpp/cURLpp.hpp>

int main()
{
    curlpp::Cleanup cleanup;

    Request req("GET", "http://127.0.0.1:8000/docs");

    req.perform();

    return 0;
}