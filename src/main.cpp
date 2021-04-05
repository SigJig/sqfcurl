
#include <iostream>

#include "request.h"

int main()
{
    Request req("GET", "/docs");

    req.perform();
}