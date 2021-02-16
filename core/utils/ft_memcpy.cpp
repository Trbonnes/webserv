#include "utils.hpp"

void* ft_memcpy(void* dst, void const* src, size_t len)
{
    u_int64_t* plDst = (u_int64_t*) dst;
    u_int64_t* plSrc = (u_int64_t*) src;

	// Check if the memory is 8 byte aligned, meaning the address is a multiple of 8 <==> the three first bits should be 0s
	// You need to cast the pointer into an uint64 because the compiler prohibits bitwise operations on pointers
    if (!((u_int64_t)src & 0x7) && !((u_int64_t)dst & 0x7))
    {
        while (len >= 8)
    {
            *plDst++ = *plSrc++;
            len -= 8;
        }
    }
    char * pcDst = (char *) plDst;
    char const * pcSrc = (char *) plSrc;
    while (len--)
    {
        *pcDst++ = *pcSrc++;
    }
    return (dst);
}
