#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash2rgb.h"
#include "random.h"
#include "uint.h"
#include "uint3.h"
#include "ppm.h"

int usage();

uint3 hash2rgb(char* hashname, uint x, uint y);

int main(int argc, char** argv)
{

    if (argc < 2)
        return usage();

    int fflag = 0, bflag = 0, qflag = 0, cflag = 0;
    int width = 256, height = 256;
    char* filename = NULL;
    char* hashname = NULL;
    int c;
        
    while ((c = getopt(argc, argv, "bqf:cw:h:")) != -1)
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
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        }
    }
    
    uint8_t buffer[width * height * 3];

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


    uint8_t (*pixels)[3] = new uint8_t[width*height][3];
    uint8_t (*pixel)[3] = pixels;
    for (uint j = 0; j < height; j++)
    {
        for (uint i = 0; i < width; i++)
        {
            if (fflag)
            {
                if (cflag)
                {
                    (*pixel)[0] = buffer[3 * (j * width + i) + 0];
                    (*pixel)[1] = buffer[3 * (j * width + i) + 1];
                    (*pixel)[2] = buffer[3 * (j * width + i) + 2];
                }
                else 
                {
                    char c = buffer[j * width + i];

                    (*pixel)[0] = c;
                    (*pixel)[1] = c;
                    (*pixel)[2] = c;
                }
            }
            else if (bflag)
            {
                uint x = uint(8.f * (float(i) / float(width)));
                uint y = uint(4.f * (float(j) / float(height)));
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

                if (i >= (width / 2))
                {
                    if (j >= (height / 2))
                    {                    
                        b = 3;
                    }
                    else
                    {
                        b = 1;
                    }

                }
                else if (j >= (height / 1))
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
    fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, width*height*3, f);
    fclose(f);

    /*
    fprintf(stdout, "P6\n%d %d\n%d\n", SIZE, SIZE, 255);
    fwrite(pixels, 1, SIZE*SIZE*3, stdout);
    */

    /*
    write_ppm(outname, pixels, height, width);
    */

    return 0;
}

int usage()
{
    fprintf(stderr, "Usage: rng2img [hash]\n");
    return 1;
}
