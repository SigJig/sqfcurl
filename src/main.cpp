
#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

int main()
{
    try {
        curlpp::Cleanup cup;

        std::cout << curlpp::options::Url("127.0.0.1:8000/docs");
    }
    catch (curlpp::RuntimeError &e) {
        std::cout << e.what() << std::endl;
    }
    catch (curlpp::LogicError &e) {
        std::cout << e.what() << std::endl;
    }
}