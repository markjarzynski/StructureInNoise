#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "random.h"

#define SIZE 256
#define SIZE2 (SIZE*SIZE)

int usage();

uint3 hash2rgb(char* hashname, uint x, uint y);

int main(int argc, char** argv)
{

    if (argc < 2)
        return usage();

    int fflag = 0, bflag = 0, qflag = 0, cflag = 0;
    char* filename = NULL;
    unsigned char buffer[SIZE2 * 3];
    char* hashname = NULL;
    int c;
        
    while ((c = getopt(argc, argv, "bqf:c")) != -1)
    {
        switch (c)
        {
        case 'b':
            bflag = 1;
            break;
        case 'f':
            fflag = 1;
            filename = optarg;
            break;
        case 'q':
            qflag = 1;
            break;
        case 'c':
            cflag = 1;
            break;
        }
    }

    if (fflag)
    {
        FILE* f = fopen(filename, "rb");
        fread(buffer, sizeof(buffer), 1, f);
        hashname = filename;
        fclose(f);
    }
    else
    {
        hashname = argv[argc - 1];
    }


    uint8_t (*pixels)[3] = new uint8_t[SIZE*SIZE][3];
    uint8_t (*pixel)[3] = pixels;
    for (uint i = 0; i < SIZE; i++)
    {
        for (uint j = 0; j < SIZE; j++)
        {
            if (fflag)
            {
                if (cflag)
                {
                    (*pixel)[0] = buffer[3 * (j * SIZE + i) + 0];
                    (*pixel)[1] = buffer[3 * (j * SIZE + i) + 1];
                    (*pixel)[2] = buffer[3 * (j * SIZE + i) + 2];
                }
                else 
                {
                    char c = buffer[j * SIZE + i];

                    (*pixel)[0] = c;
                    (*pixel)[1] = c;
                    (*pixel)[2] = c;
                }
            }
            else if (bflag)
            {
                uint x = uint(8.f * (float(i) / float(SIZE)));
                uint y = uint(4.f * (float(j) / float(SIZE)));
                uint bit = x + 8u * y;

                uint3 v = hash2rgb(hashname, i, j);

                (*pixel)[0] = uint8_t(((v.x >> bit) & 1u) * 255u);
                (*pixel)[1] = uint8_t(((v.y >> bit) & 1u) * 255u);
                (*pixel)[2] = uint8_t(((v.z >> bit) & 1u) * 255u);
            }
            else if (qflag)
            {
                uint3 v = hash2rgb(hashname, i, j);
                uint b = 0;

                if (i >= (SIZE >> 1))
                {
                    if (j >= (SIZE >> 1))
                    {
                        b = 3;
                    }
                    else
                    {
                        b = 1;
                    }

                }
                else if (j >= (SIZE >> 1))
                {
                    b = 2;
                }
                        
                (*pixel)[0] = uint8_t((v.x >> (b * 8u)) & 0xff);
                (*pixel)[1] = uint8_t((v.y >> (b * 8u)) & 0xff);
                (*pixel)[2] = uint8_t((v.z >> (b * 8u)) & 0xff);
            }
            else
            {
                uint3 v = hash2rgb(hashname, i, j);

                float scale = 1.f / (float)0xffffffffu;
                float3 f = v * float3(scale);

                (*pixel)[0] = uint8_t(f.x * 255u);
                (*pixel)[1] = uint8_t(f.y * 255u);
                (*pixel)[2] = uint8_t(f.z * 255u);
            };

            pixel++;
        }
    }

    char outname[255];
    strncpy(outname, hashname, 250);
    strncat(outname, ".ppm", 254);

    FILE* f = fopen(outname, "wb");
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
    fprintf(stderr, "Usage: rng2img [hash]\n");
    return 1;
}

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
