#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "random.h"

uint32_t hash2rgb(char* hashname, uint x, uint y)
{
    if (false) {}

#define ELSEIF11(HASH)                      \
    else if (strcmp(hashname, #HASH "_linear") == 0) \
    {                                       \
        uint h = HASH(seed(uint2(x,y)));    \
        return h;              \
    }                                       \
    else if (strcmp(hashname, #HASH "_nested") == 0) \
    {                                       \
        uint h = HASH(HASH(x) + y);         \
        return h;              \
    }

#define ELSEIF12(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint2(x,y));          \
        return h;              \
    }

#define ELSEIF13(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint3(x, y, 0u));     \
        return h;              \
    }

#define ELSEIF14(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint4(x, y, 0u, 0u)); \
        return h;              \
    }

#define ELSEIF12_(HASH,NAME)                \
    else if (strcmp(hashname, #NAME) == 0)  \
    {                                       \
        uint h = HASH(uint2(x,y));          \
        return h;              \
    }

#define ELSEIF13_(HASH,NAME)                \
    else if (strcmp(hashname, #NAME) == 0)  \
    {                                       \
        uint h = HASH(uint3(x, y, 0u));     \
        return h;              \
    }

#define ELSEIF14_(HASH,NAME)                \
    else if (strcmp(hashname, #NAME) == 0)  \
    {                                       \
        uint h = HASH(uint4(x, y, 0u, 0u)); \
        return h;              \
    }

#define ELSEIF22(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint2(x, y));        \
        return h.x;         \
    }

#define ELSEIF23(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint3(x, y, 0u));    \
        return h.x;         \
    }

#define ELSEIF24(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint2 h = HASH(uint4(x, y, 0u, 0u));\
        return h.x;          \
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
        return h.x;                         \
    }

#define ELSEIF34(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint3 h = HASH(uint4(x, y, 0u, 0u));\
        return h.x;        \
    }

#define ELSEIF42(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint2(x, y));        \
        return h.x;        \
    }

#define ELSEIF43(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint3(x, y, 0u));    \
        return h.x;        \
    }

#define ELSEIF44(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint4(x, y, 0u, 0u));\
        return h.x;        \
    }

#define ELSEIF12f(HASH)                     \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        float h = HASH(float2(x,y));        \
        uint u = h * float(0xffffffffu);    \
        return u;              \
    }

    ELSEIF11(bbs)
    ELSEIF12_(city, city_2)
    ELSEIF13_(city, city_3)
    ELSEIF14_(city, city_4)
    ELSEIF11(esgtsa)
    ELSEIF12f(fast)
    ELSEIF12f(ign)
    ELSEIF11(iqint1)
    ELSEIF33(iqint2)
    ELSEIF12(iqint3)
    ELSEIF12(jkiss32)
    ELSEIF11(lcg)
    ELSEIF42(md5_2)
    ELSEIF43(md5_3)
    ELSEIF44(md5)
    ELSEIF12_(murmur3, murmur3_2)
    ELSEIF13_(murmur3, murmur3_3)
    ELSEIF14_(murmur3, murmur3_4)
    ELSEIF11(pcg)
    ELSEIF22(pcg2d)
    ELSEIF33(pcg3d)
    ELSEIF33(pcg3d16)
    ELSEIF44(pcg4d)
    ELSEIF12f(pseudo)
    ELSEIF11(ranlim32)
    ELSEIF12_(superfast, superfast_2)
    ELSEIF13_(superfast, superfast_3)
    ELSEIF14_(superfast, superfast_4)
    
    else if (strcmp(hashname, "tea2") == 0)
    {
        uint2 h = tea(2, uint2(x,y));
        return h.x;
    }
    else if (strcmp(hashname, "tea3") == 0)
    {
        uint2 h = tea(3, uint2(x,y));
        return h.x;
    }
    else if (strcmp(hashname, "tea4") == 0)
    {
        uint2 h = tea(4, uint2(x,y));
        return h.x;
    }
    else if (strcmp(hashname, "tea5") == 0)
    {
        uint2 h = tea(5, uint2(x,y));
        return h.x;
    }
    
    ELSEIF12f(trig)
    ELSEIF11(wang)
    ELSEIF44(xorshift128)
    ELSEIF11(xorshift32)
    ELSEIF12_(xxhash32, xxhash32_2)
    ELSEIF13_(xxhash32, xxhash32_3)
    ELSEIF14_(xxhash32, xxhash32_4)
    ELSEIF14(hybridtaus)

    else if (strcmp(hashname, "white") == 0)
    {
        return 0xffffffff;
    }
    else if (strcmp(hashname, "black") == 0)
    {
        return 0;
    }
    else if (strcmp(hashname, "checkerboard") == 0)
    {
        if (x & 1 == 1 && y & 1 == 1 || x & 1 == 0 && y & 1 == 0)
        {
            return 0xffffffff;
        }
        else
        {
            return 0;
        }
    }

    else
    {
        return 0;
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
