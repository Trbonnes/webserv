#include "LoadController.hpp"

void    LoadController::loadController(int nb, std::string &str)
{
    long alloc;
    int timerand;
    int i;

    timerand = rand() % 10;
    usleep(timerand);
    i = 1000;
    while (str.capacity() < (unsigned long)nb)
    {
        while (1)
        {
            alloc = get_avphys_pages();
            if (alloc > (nb / 4096) * MULT)
                break;
        }
        str.reserve(i);
        i *= 10;
    }
}

void    LoadController::waitController(int nb)
{
    long alloc;
    int timerand;

    timerand = rand() % 10;
    usleep(timerand);
    while (1)
    {
        alloc = get_avphys_pages();
        if (alloc > (nb / 4096) * (MULT - 2))
            break;
    }
}