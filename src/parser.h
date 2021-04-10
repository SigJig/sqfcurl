
#ifndef SQFPARSER_H
#define SQFPARSER_H

#include <string>
#include <vector>

#include "error.h"

inline std::vector<std::string> parse_array(const std::string& input)
{
    std::vector<std::string> output;
    std::string current;
    bool in_string = false;
    bool opener_found = false;
    char quote;

    for (auto it = input.begin(); it != input.end(); it++)
    {
        char c = *it;

        if (!opener_found)
        {
            if (c == '[')
            {
                opener_found = true;
            }
        }
        else if (c == ']' && !in_string)
        {
            output.push_back(current);

            return std::move(output);
        }
        else if (c == '"' || c == '\'')
        {
            if (!in_string)
            {
                in_string = true;
                quote = c;
            }
            else if (c != quote)
            {
                current += c;
            }
            else if (*(it + 1) != c)
            {
                in_string = false;
            }
            else
            {
                current += c;
                it++; // Skip the next one since its the same quote
            }
        }
        else if (c == ',' && !in_string)
        {
            output.push_back(current);
            current.clear();
        }
        else if (in_string)
        {
            current += c;
        }
    }

    throw CallError(ErrorCode::PARAMS_PARSE_ERROR, "Unable to parse input array");
}

// Because it would make too much sense for string arguments to be converted to actual strings,
// not an arma string wrapped in a c string
// The url argument will be """http://endpoint/blahblah""" when recieved from the extension
inline std::string sanitize_input(std::string input)
{
    if (input.size() > 2)
    {
        char front = input.front();
        if (front == input.back() && (front == '"' || front == '\''))
        {
            // Length is length of string minus 2 (quote at the start of the string + quote at end)
            return input.substr(1, input.size()-2);
        }
    }

    return input;
}

#endif // SQFPARSER_H