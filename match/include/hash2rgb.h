#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "random.h"

uint3 hash2rgb(char* hashname, uint x, uint y)
{
    if (false) {}

#define ELSEIF11(HASH)                      \
    else if (strcmp(hashname, #HASH "_linear") == 0) \
    {                                       \
        uint h = HASH(seed(uint2(x,y)));    \
        return uint3(h, h, h);              \
    }                                       \
    else if (strcmp(hashname, #HASH "_nested") == 0) \
    {                                       \
        uint h = HASH(HASH(x) + y);         \
        return uint3(h, h, h);              \
    }

#define ELSEIF12(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint2(x,y));          \
        return uint3(h, h, h);              \
    }

#define ELSEIF13(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint3(x, y, 0u));     \
        return uint3(h, h, h);              \
    }

#define ELSEIF14(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint4(x, y, 0u, 0u)); \
        return uint3(h, h, h);              \
    }

#define ELSEIF22(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint2(x, y));        \
        return uint3(h.x, h.y, 0u);         \
    }

#define ELSEIF23(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint3(x, y, 0u));    \
        return uint3(h.x, h.y, 0u);         \
    }

#define ELSEIF24(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint4(x, y, 0u, 0u));\
        return uint3(h.x, h.y, 0);          \
    }

#define ELSEIF32(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint3 h = HASH(uint2(x, y));        \
        return h;                           \
    }

#define ELSEIF33(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint3 h = HASH(uint3(x, y, 0u));    \
        return h;                           \
    }

#define ELSEIF34(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint3 h = HASH(uint4(x, y, 0u, 0u));\
        return uint3(h.x, h.y, h.z);        \
    }

#define ELSEIF42(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint2(x, y));        \
        return uint3(h.x, h.y, h.z);        \
    }

#define ELSEIF43(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint3(x, y, 0u));    \
        return uint3(h.x, h.y, h.z);        \
    }

#define ELSEIF44(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint4(x, y, 0u, 0u));\
        return uint3(h.x, h.y, h.z);        \
    }


    ELSEIF12(city)
    ELSEIF11(esgtsa)
    ELSEIF11(iqint1)
    ELSEIF33(iqint2)
    ELSEIF12(iqint3)
    ELSEIF12(jkiss32)
    ELSEIF11(lcg)
    ELSEIF44(md5)
    ELSEIF12(murmur3)
    ELSEIF33(pcg3d)
    ELSEIF44(pcg4d)
    ELSEIF11(ranlim32)
    ELSEIF12(superfast)
    
    else if (strcmp(hashname, "tea2") == 0)
    {
        uint2 h = tea(2, uint2(x,y));
        return uint3(h.x, h.y, 0u);
    }
    else if (strcmp(hashname, "tea3") == 0)
    {
        uint2 h = tea(3, uint2(x,y));
        return uint3(h.x, h.y, 0u);
    }
    else if (strcmp(hashname, "tea4") == 0)
    {
        uint2 h = tea(4, uint2(x,y));
        return uint3(h.x, h.y, 0u);
    }
    else if (strcmp(hashname, "tea5") == 0)
    {
        uint2 h = tea(5, uint2(x,y));
        return uint3(h.x, h.y, 0u);
    }

    ELSEIF11(wang)
    ELSEIF44(xorshift128)
    ELSEIF11(xorshift32)
    ELSEIF12(xxhash32)
    ELSEIF14(hybridtaus)

    else if (strcmp(hashname, "white") == 0)
    {
        return uint3(0xffffffff, 0xffffffff, 0xffffffff);
    }
    else if (strcmp(hashname, "black") == 0)
    {
        return uint3(0, 0, 0);
    }
    else if (strcmp(hashname, "checkerboard") == 0)
    {
        if (x & 1 == 1 && y & 1 == 1 || x & 1 == 0 && y & 1 == 0)
        {
            return uint3(0xffffffff, 0xffffffff, 0xffffffff);
        }
        else
        {
            return uint3(0, 0, 0);
        }
    }

    else
    {
        return uint3(0,0,0);
    }

#undef ELSEIF11
#undef ELSEIF12
#undef ELSEIF13
#undef ELSEIF14
#undef ELSEIF22
#undef ELSEIF23
#undef ELSEIF24
#undef ELSEIF32
#undef ELSEIF33
#undef ELSEIF34
#undef ELSEIF42
#undef ELSEIF43
#undef ELSEIF44
}
