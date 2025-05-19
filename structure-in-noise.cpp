#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include <sys/time.h>

#include "uint3.h"
#include "random.h"
#include "hash2rgb.h"
#include "bitshift.h"
#include "popcnt.h"

#include "ppm.h"
#include "crop.h"
#include "print_pixel.h"

#define CHANNELS 3
#define BITS 32

int usage(char* name);

uint32_t compare(uint32_t* a, uint32_t* b, int w, int h);
 
int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[3]);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int height = 128, width = 128;
    int xi = 0, yi = 0;
    int h = 3, w = 3;

    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    uint t = current_time.tv_sec * 1000000 + current_time.tv_usec;

    char* hashname = NULL;
    char* filename = NULL;

    int c, fflag = 0, oflag = 0;
    while ((c = getopt(argc, argv, "x:y:rw:h:k:f:o")) != -1)
    {
        switch (c)
        {
        case 'x': // initial x position
            xi = atoi(optarg);
            break;
        case 'y': // initial y position
            yi = atoi(optarg);
            break;
        case 'r': // use random x/y positions
            xi = pcg(t);
            yi = pcg(xi);
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
        case 'o':
            oflag = 1;
            break;
        }
    }

    //printf("x: %d, y: %d, w: %d, h: %d, k: %d\n", xi, yi, width, height, h);

    uint32_t buffer[height * width];

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

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {   
                buffer[y * width + x] = hash2rgb(hashname, xi + x, yi + y);
            }
        }
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

    uint32_t neighborhood1[s], neighborhood2[s];

    int matches = 0;

    for (int y = 0; y < height - h; y++)
    {
        for (int x = 0; x < width - w; x++)
        {
            crop(buffer, width, height, neighborhood1, x, y, w, h);

            //for (int b1 = 0; b1 < BITS; b1++)
            {
                //bitshift(compare1, kernel, s, b1);
                //print_uint3(compare1, w, h, 0, 0, w, h);

                for (int j = 0; j < height - h; j++)
                {
                    for (int i = 0; i < width - w; i++)
                    {
                        //printf("%d, %d\n", i, j);

                        crop(buffer, width, height, neighborhood2, i, j, w, h);

                        //for (int b2 = 0; b2 < BITS; b2++)
                        {
                            if ((i == x) && (j == y))// && (b1 == b2))
                            {
                                continue;
                            }

                            //bitshift(compare2, sample, s, b2);

                            uint32_t result = compare(neighborhood1, neighborhood2, w, h);

                            matches += result;

                            /*
                            if (result)
                            {
                                //printf("%d, %d matches %d, %d\n", y, x, i, j);
                                //printf("  (%d, %d, %d) == (%d, %d, %d)\n", kernel[0], kernel[1], kernel[2], sample[0], sample[1], sample[2]);
           
                                //print_uint3(kernel, w, h, 0, 0, w, h);
                                //print_uint3(sample, w, h, 0, 0, w, h);

                                if (oflag)
                                {
                                    set_pixel(output, width, height, x, y, white);
                                    set_pixel(output, width, height, j, i, white);
                                }

                                matches++;
                            }
                            */

                            //int pos = 3 * (i * h + j);
                            //printf("%d, %d, %d\n", kernel[pos], kernel[pos + 1], kernel[pos + 2]);

                        }
                    }
                }
            }
        }
    }

    if (oflag)
    {
        char outname[256];
        snprintf(outname, 255, "%s.x%dy%d-w%dh%d-k%d.ppm", hashname, xi, yi, width, height, h);
        write_ppm(outname, output, width, height);
    }

    printf("%s,%d\n", hashname, matches);

    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <hashname>\n", name);
    return 0;
}

uint32_t compare(uint32_t* a, uint32_t* b, int w, int h)
{
    uint32_t result = 0xFFFFFFFFu;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            result &= ~(a[j * w + i] ^ b[j * w + i]);
        }
    }

    return popcnt(result);
}

int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[3])
{
    for (int c = 0; c < 3; c++)
    {
        buffer[3 * (y * width + x) + c] = rgb[c];
    }
    return 1;
}
