#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "ppm.h"
#include "crop.h"
#include "bitshift.h"
#include "print_pixel.h"

#define CHANNELS 3
#define BITS 8

int usage(char* name);

int compare(uint8_t* a, uint8_t* b, int w, int h);

int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[CHANNELS]);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int height = 256, width = 256;
    int SIZE = height * width * CHANNELS;
    uint8_t buffer[SIZE];
    uint8_t output[SIZE];
    memset(output, 0, SIZE * sizeof(uint8_t));

    read_ppm(argv[1], buffer, SIZE);

    char* basename = strrchr(argv[1], '/') + 1;
    char hashname[256];
    strncpy(hashname, basename, 256);
    *strrchr(hashname, '.') = '\0';
    
    uint8_t white[3] = {255u, 255u, 255u};

    /*
    for (int i = 0; i < 2 * 3; i += 3)
    {
        printf("%d, %d, %d\n", buffer[i], buffer[i+1], buffer[i+2]);
    }
    */

    int h = 3, w = 3;
    int s = h * w * CHANNELS;

    uint8_t kernel[s], sample[s], compare1[s], compare2[s];

    int matches = 0;

    for (int y = 0; y < height - h; y++)
    {
        for (int x = 0; x < width - w; x++)
        {
            int result = crop(buffer, width, height, kernel, x, y, w, h, CHANNELS); 

            for (int b1 = 0; b1 < BITS; b1++)
            {
                bitshift(compare1, kernel, s, b1);

                for (int i = y; i < height - h; i++)
                {
                    for (int j = x + 1; j < width - w; j++)
                    {
                        if (i == y && j == x)
                        {
                            continue;
                        }

                        for (int b2 = b1; b2 < BITS; b2++)
                        {
                            result = crop(buffer, width, height, sample, j, i, w, h, CHANNELS);
                            bitshift(compare2, sample, s, b2);
                            result = compare(compare1, compare2, w, h);

                            if (result)
                            {
                               //printf("%d, %d matches %d, %d\n", y, x, i, j);
                               //printf("  (%d, %d, %d) == (%d, %d, %d)\n", kernel[0], kernel[1], kernel[2], sample[0], sample[1], sample[2]);

                               //print_pixels(compare1, w, h, 0, 0, w, h, CHANNELS);
                               //print_pixels(compare2, w, h, 0, 0, w, h, CHANNELS);
           
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
                BREAK:;
            }
        }
    }

    char outname[256];
    strcpy(outname, hashname);
    strcat(outname, ".bitmatch.ppm");
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

int compare(uint8_t* a, uint8_t* b, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            for (int k = 0; k < CHANNELS; k++)
            {
                if (a[CHANNELS * (i * h + j) + k] != b[CHANNELS * (i * h + j) + k])
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

int set_pixel(uint8_t* buffer, int width, int height, int x, int y, uint8_t rgb[CHANNELS])
{
    for (int c = 0; c < CHANNELS; c++)
    {
        buffer[CHANNELS * (height * y + x) + c] = rgb[c];
    }
    return 1;
}
