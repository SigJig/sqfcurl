
#include "logger.h"

#include <iostream>

Logger::Logger()
{  }

void Logger::info(std::string msg)
{
    this->write(INFO, "INFO", msg);
}

void Logger::warn(std::string msg)
{
    this->write(WARNING, "WARNING", msg);
}

void Logger::err(std::string msg)
{
    this->write(ERROR, "ERROR", msg);
}

void Logger::debug(std::string msg)
{
    this->write(DEBUG, "DEBUG", msg);
}

bool Logger::should_log(Level level)
{
    return (level <= m_level);
}

void Logger::write(Level level, std::string prefix, std::string msg)
{
    if (this->should_log(level))
    {
        std::cout << "[" << prefix << "]: " << msg << std::endl;
    }
}