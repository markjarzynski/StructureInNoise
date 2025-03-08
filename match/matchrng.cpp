#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "uint3.h"
#include "random.h"
#include "hash2rgb.h"
#include "bitshift.h"

#include "ppm.h"
#include "crop.h"
#include "print_pixel.h"

#define CHANNELS 3
#define BITS 32

int usage(char* name);

int compare(uint3* a, uint3* b, int w, int h);
 
int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[3]);

void make_kernel(uint3* kernel, char* hashname, int x, int y, int w, int h);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int height = 8, width = 8;
    int xi = 0, yi = 0;
    int h = 3, w = 3;

    char* hashname = NULL;
    char* filename = NULL;

    int c, fflag = 0;
    while ((c = getopt(argc, argv, "x:y:w:h:k:f:")) != -1)
    {
        switch (c)
        {
        case 'x': // initial x position
            xi = atoi(optarg);
            break;
        case 'y': // initial y position
            yi = atoi(optarg);
            break;
        case 'w': // width
            width = atoi(optarg);
            break;
        case 'h': // height
            height = atoi(optarg);
            break;
        case 'k': // Kernel size
            h = atoi(optarg);
            w = atoi(optarg);
            break;
        case 'f':
            fflag = 1;
            filename = optarg;
            break;
        }
    }

    uint3 buffer[height * width];

    if (fflag)
    {
        FILE* f = fopen(filename, "rb");
        size_t read_bytes = fread(buffer, sizeof(buffer), 1, f);
        hashname = filename;
        fclose(f);
    }
    else
    {
        hashname = argv[argc - 1];
    }

    int SIZE = height * width * 3;
    uint8_t output[SIZE];
    memset(output, 0, SIZE * sizeof(uint8_t));
 
    uint8_t white[3] = {255u, 255u, 255u};

    /*
    for (int i = 0; i < 2 * 3; i += 3)
    {
        printf("%d, %d, %d\n", buffer[i], buffer[i+1], buffer[i+2]);
    }
    */

    int s = h * w;

    uint3 kernel[s], sample[s], compare1[s], compare2[s];

    int matches = 0;

    for (int y = yi; y < yi + height - h; y++)
    {
        for (int x = xi; x < xi + width - w; x++)
        {

            //printf("%d, %d\n", x, y);
            if (fflag)
            {
                crop(buffer, width, height, kernel, x, y, w, h);
            }
            else
            {
                make_kernel(kernel, hashname, x, y, w, h);
            }

            for (int b1 = 0; b1 < BITS; b1++)
            {
                bitshift(compare1, kernel, s, b1);

                for (int i = y; i < yi + height - h; i++)
                {
                    for (int j = x; j < xi + width - w; j++)
                    {
                        if (i == y && j == x)
                        {
                            continue;
                        }

                        if (fflag)
                        {
                            crop(buffer, width, height, sample, j, i, w, h);
                        }
                        else
                        {
                            make_kernel(sample, hashname, j, i, w, h);
                        }

                        for (int b2 = b1; b2 < BITS; b2++)
                        {
                            bitshift(compare2, sample, s, b2);

                            int result = compare(compare1, compare2, w, h);

                            if (result)
                            {
                               //printf("%d, %d matches %d, %d\n", y, x, i, j);
                               //printf("  (%d, %d, %d) == (%d, %d, %d)\n", kernel[0], kernel[1], kernel[2], sample[0], sample[1], sample[2]);
           
                               //print_uint3(kernel, w, h, 0, 0, w, h);
                               //print_uint3(sample, w, h, 0, 0, w, h);

                               set_pixel(output, width, height, x, y, white);
                               set_pixel(output, width, height, j, i, white);

                               matches++;
                               goto BREAK;
                            }

                            //int pos = 3 * (i * h + j);
                            //printf("%d, %d, %d\n", kernel[pos], kernel[pos + 1], kernel[pos + 2]);

                        }

                    }
                }
            }
            BREAK:;
        }
    }

    char outname[256];
    snprintf(outname, 255, "%s.matchrng.x%dy%d-w%dh%d-k%d.ppm", hashname, xi, yi, width, height, h);
    //printf("%s\n", outname);

    write_ppm(outname, output, width, height);

    printf("%s,%d\n", hashname, matches);

    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <image.ppm>\n", name);
    return 0;
}

int compare(uint3* a, uint3* b, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (a[i * h + j] != b[i * h + j])
            {
                return 0;
            }
        }
    }

    return 1;
}

int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[3])
{
    for (int c = 0; c < 3; c++)
    {
        buffer[3 * (height * y + x) + c] = rgb[c];
    }
    return 1;
}

void make_kernel(uint3* kernel, char* hashname, int x, int y, int w, int h)
{
    //printf("x:%d, y:%d, w:%d, h:%d\n", x, y, w, h);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            kernel[i * h + j] = hash2rgb(hashname, x + j, y + i);
            //printf("%d x:%d y:%d\n", i * h + j, x+j, y+i);
        }
    }
    //printf("---\n");
}
