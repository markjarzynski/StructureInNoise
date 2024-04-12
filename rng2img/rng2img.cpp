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

#define ELSEIF11(NAME, HASH)                \
    else if (strcmp(hashname, NAME) == 0)   \
    {                                       \
        uint h = HASH(seed(uint2(x,y)));    \
        return uint3(h,h,h);                \
    }

#define ELSEIF21(NAME, HASH)                \
    else if (strcmp(hashname, NAME) == 0)   \
    {                                       \
        uint h = HASH(uint2(x,y));          \
        return uint3(h,h,h);                \
    }

#define ELSEIF33(NAME, HASH)                \
    else if (strcmp(hashname, NAME) == 0)   \
    {                                       \
        uint3 h = HASH(uint3(x, y, 0u));    \
        return h;                           \
    }


    ELSEIF21("city", city)
    ELSEIF11("lcg", lcg)
    ELSEIF33("pcg3d", pcg3d)

    else
    {
        return uint3(0,0,0);
    }

#undef ELSEIF11
#undef ELSEIF21
#undef ELSEIF33
}
