#include "../HTTP.hpp"

void    HTTP::head(void)
{
    get();
    _body.clear();
    return ;
}
