#ifndef COMPARE_HPP
# define COMPARE_HPP

template <class T>
struct   CompLength
{
    bool operator() (const T& x, const T& y) const
    {
        if (x.length() > y.length())
            return true;
        return false;
    }
};

template <class T>
struct  CompNormalOrder
{
    bool operator() (const T& x, const T& y) const
    {
        return x < y;
    }
};

#endif
