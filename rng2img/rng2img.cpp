#include <stdio.h>
#include "random.h"

#define SIZE 256

int usage();

uint3 hash2rgb(char* hashnamei, uint x, uint y);


int main(int argc, char** argv)
{

    if (argc != 2)
        return usage();

    char* hashname = argv[1];


    uint8_t (*pixels)[3] = new uint8_t[SIZE*SIZE][3];
    uint8_t (*pixel)[3] = pixels;
    for (uint i = 0; i < SIZE; i++)
    {
        for (uint j = 0; j < SIZE; j++)
        {
            uint3 v = hash2rgb(hashname, i, j);

            float scale = 1.f / (float)0xffffffffu;
            float3 f = v * float3(scale);

            (*pixel)[0] = uint8_t(f.x * 255u);
            (*pixel)[1] = uint8_t(f.y * 255u);
            (*pixel)[2] = uint8_t(f.z * 255u);

            pixel++;
        }
    }

    char filename[255];
    strncpy(filename, hashname, 250);
    strncat(filename, ".ppm", 254);

    FILE* f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n%d\n", SIZE, SIZE, 255);
    fwrite(pixels, 1, SIZE*SIZE*3, f);
    fclose(f);

    /*
    fprintf(stdout, "P6\n%d %d\n%d\n", SIZE, SIZE, 255);
    fwrite(pixels, 1, SIZE*SIZE*3, stdout);
    */

    return 0;
}

int usage()
{
    fprintf(stderr, "Usage: rng2img hash\n");
    return 1;
}

uint3 hash2rgb(char* hashname, uint x, uint y)
{
    if (false) {}

#define ELSEIF11(HASH)                      \
    else if (strcmp(hashname, #HASH "_linear") == 0) \
    {                                       \
        uint h = HASH(seed(uint2(x,y)));    \
        return uint3(h,h,h);                \
    }                                       \
    else if (strcmp(hashname, #HASH "_nested") == 0) \
    {                                       \
        uint h = HASH(HASH(x) + y);         \
        return uint3(h,h,h);                \
    }


#define ELSEIF12(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint2(x,y));          \
        return uint3(h,h,h);                \
    }

#define ELSEIF33(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint3 h = HASH(uint3(x, y, 0u));    \
        return h;                           \
    }

#define ELSEIF44(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint4 h = HASH(uint4(x, y, 0u, 0u));\
        return uint3(h.x, h.y, h.z);        \
    }

#define ELSEIF14(HASH)                      \
    else if (strcmp(hashname, #HASH) == 0)  \
    {                                       \
        uint h = HASH(uint4(x, y, 0u, 0u)); \
        return uint3(h, h, h);              \
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

    else
    {
        return uint3(0,0,0);
    }

#undef ELSEIF11
#undef ELSEIF21
#undef ELSEIF33
}
