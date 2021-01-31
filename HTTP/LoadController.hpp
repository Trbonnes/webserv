#ifndef LOADCONTROLLER_HPP
# define LOADCONTROLLER_HPP

# include <string>
# include <sys/sysinfo.h>
# include <stdlib.h>
# include <unistd.h>

# define MULT 7

class LoadController
{
    public:
    static void    loadController(int nb, std::string &str);
    static void    waitController(int nb);
};

#endif
