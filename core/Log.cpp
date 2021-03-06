#include "Log.hpp"


Log::Log(const Log &)
{    
}

Log &Log::operator=(const Log &)
{
    return *this;
}

Log::~Log()
{    
}

void Log::debug(std::string &str)
{
    std::cerr << ANSI_COLOR_YELLOW << str << ANSI_COLOR_RESET << std::endl;    
}

void Log::debug(char *str)
{
    std::cerr << ANSI_COLOR_YELLOW << str << ANSI_COLOR_RESET << std::endl;    
}

void Log::debug(const char *str)
{
    std::cerr << ANSI_COLOR_YELLOW << str << ANSI_COLOR_RESET << std::endl;    
}

void Log::debug(int i)
{
    std::cerr << ANSI_COLOR_YELLOW << i << ANSI_COLOR_RESET << std::endl;    
}
