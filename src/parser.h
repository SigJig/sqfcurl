
#ifndef SQFPARSER_H
#define SQFPARSER_H

#include <string>
#include <vector>

inline std::string parse_string(std::string input)
{
    std::string output;

    char quot;
    bool in_string = false;
    bool last_is_quote = false;
    bool done = false;

    for (auto& c : input)
    {
        if (done)
        {
            if (!isspace(c))
            {
                // ERROR
            }
        }
        else if (c == '"' || c == '\'')
        {
            if (!in_string)
            {
                in_string = true;
                quot = c;
            }
            else if (c == quot && !last_is_quote)
            {
                last_is_quote = true;
            }
            else
            {
                output += c;
                last_is_quote = false;
            }
        }
        else if (last_is_quote)
        {
            in_string = false;
            done = true;
        }
        else if (in_string)
        {
            output += c;
        }
        else if (!isspace(c))
        {
            // ERROR
        }
    }

    return std::move(output);
}

inline std::vector<std::string> parse_1d(std::string data)
{
    std::vector<std::string> out;
    std::string current;

    int depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (auto &c : data)
    {
        if (depth < 1)
        {
            if (c == '[')
            {
                depth += 1;
                continue;
            }
        }
        else if (!in_string)
        {
            if (c == '"')
            {
                in_string = true;
                continue;
            }
            else if (c == ']')
            {
                depth -= 1;
            }
            else if (c == ',')
            {

            }
            else if (!isspace(c))
            {
                // ERROR TIME >:D
            }
        }
        else if (c != '"' && escaped)
        {
            escaped = false;
            current += '"';
        }
        else if (c == '"')
        {
            escaped = true;
        }
    }

    return std::move(out);
}

#endif // SQFPARSER_H