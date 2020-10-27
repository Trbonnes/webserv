#ifndef COMPARE_HPP
# define COMPARE_HPP

template <class T>
struct   Compare
{
    bool operator() (const T& x, const T& y) const
    {
        if (x.length() > y.length())
            return true;
        return false;
    }
};

#endif