
#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
public:
    Logger();

    void info(std::string msg);
    void warn(std::string msg);
    void err(std::string msg);
    void debug(std::string msg);

    enum Level
    {
        ERROR = 0,
        WARNING,
        INFO,
        DEBUG
    };

private:
    void write(Level level, std::string prefix, std::string msg);
    bool should_log(Level level);

    Level m_level = DEBUG;
};

#endif // LOGGER_H