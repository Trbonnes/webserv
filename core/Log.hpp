#if !defined(LOG_HPP)
#define LOG_HPP

#include <iostream>
#include <string>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

class Log
{
private:
    Log();
    Log(const Log &);
    Log &operator=(const Log &);
    ~Log();
public:

    static void debug(std::string &str);
    static void debug(const char *str);
    static void debug(char *str);
};

#endif // LOG_HPP
