#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "ppm.h"
#include "crop.h"

int usage(char* name);

int compare(uint8_t* a, uint8_t* b, int w, int h);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int height = 256, width = 256;
    int SIZE = height * width * 3;
    uint8_t buffer[SIZE];

    read_ppm(argv[1], buffer, SIZE);

    char* basename = strrchr(argv[1], '/') + 1;
    char hashname[256];
    strncpy(hashname, basename, 256);
    *strrchr(hashname, '.') = '\0';
    
    /*
    for (int i = 0; i < 2 * 3; i += 3)
    {
        printf("%d, %d, %d\n", buffer[i], buffer[i+1], buffer[i+2]);
    }
    */

    int h = 3, w = 3;

    uint8_t kernel[h * w * 3], sample[h * w * 3];

    int matches = 0;

    for (int y = 0; y < height - h; y++)
    {
        for (int x = 0; x < width - w; x++)
        {
            int result = crop(buffer, width, height, kernel, x, y, w, h);

            for (int i = y; i < height - h; i++)
            {
                for (int j = x + 1; j < width - w; j++)
                {
                    if (i == y && j == x)
                    {
                        continue;
                    }

                    result = crop(buffer, width, height, sample, j, i, w, h);
                    result = compare(kernel, sample, w, h);
                    if (result)
                    {
                       //printf("%d, %d matches %d, %d\n", y, x, i, j);
                       //printf("  (%d, %d, %d) == (%d, %d, %d)\n", kernel[0], kernel[1], kernel[2], sample[0], sample[1], sample[2]);
                       matches++;
                       goto BREAK;
                    }

                    //int pos = 3 * (i * h + j);
                    //printf("%d, %d, %d\n", kernel[pos], kernel[pos + 1], kernel[pos + 2]);

                }
            }
            BREAK:;
        }
    }

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
            for (int k = 0; k < 3; k++)
            {
                if (a[3 * (i * h + j) + k] != b[3 * (i * h + j) + k])
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}
