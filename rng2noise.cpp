#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash2rgb.h"
#include "random.h"
#include "uint.h"
#include "uint3.h"
#include "ppm.h"
#include "hash2noise.h"

int usage();

int main(int argc, char** argv)
{

    if (argc < 2)
        return usage();

    int fflag = 0, oflag = 0, sflag = 0, aflag = 0, bflag = 0;
    int width = 256, height = 256;
    char* filename = NULL;
    char* outfilename = NULL;
    char* hashname = NULL;
    float s = 16.f;

    uint a = 13, b = 19; 
        
    int c;
    while ((c = getopt(argc, argv, "f:w:h:o:s:a:b:")) != -1)
    {
        switch (c)
        {
        case 'f':
            fflag = 1;
            filename = optarg;
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 'o':
            oflag = 1;
            outfilename = optarg;
            break;
        case 's':
            sflag = 1;
            s = atof(optarg);
            break;
        case 'a':
            aflag = 1;
            a = atoi(optarg);
            break;
        case 'b':
            bflag = 1;
            b = atoi(optarg);
            break;
        }
    }
    
    uint32_t* buffer = (uint32_t*)malloc(width * height * sizeof(uint32_t));

    if (fflag)
    {
        FILE* f = fopen(filename, "rb");
        size_t result = fread(buffer, sizeof(buffer), 1, f);
        hashname = filename;
        fclose(f);
    }
    else
    {
        hashname = argv[argc - 1];
    }

    char outname[256];
    if (oflag)
    {
        strncpy(outname, outfilename, 250);
    }
    else
    {
        strncpy(outname, hashname, 250);
        strncat(outname, ".ppm", 254);
    }

    uint8_t* pixels = (uint8_t*)malloc(width * height * 3 * sizeof(uint8_t));

    for (uint j = 0; j < height; j++)
    {
        for (uint i = 0; i < width; i++)
        {
            if (fflag)
            {
                uint8_t c = buffer[j * width + i];

                pixels[3 * (j * width + i) + 0] = c;
                pixels[3 * (j * width + i) + 1] = c;
                pixels[3 * (j * width + i) + 2] = c;
            }
            else
            {
                float2 uv = float2(i, j) / float2(height, width);
                uv = uv * s;
                float f = noise2(hashname, uv, uint2(a, b)) * 0.5f + 0.5f;

                pixels[3 * (j * width + i) + 0] = uint8_t(f * 255u);
                pixels[3 * (j * width + i) + 1] = uint8_t(f * 255u);
                pixels[3 * (j * width + i) + 2] = uint8_t(f * 255u);
            };
        }
    }

    FILE* f = fopen(outname, "wb");
    fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, width*height*3, f);
    fclose(f);

    free(buffer);
    free(pixels);

    return 0;
}

int usage()
{
    fprintf(stderr, "Usage: rng2noise [hash]\n");
    return 1;
}
