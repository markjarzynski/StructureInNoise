#include <stdio.h>
#include <stdint.h>

#include "ppm.h"
#include "crop.h"

int usage(char* name);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int SIZE = 256*256*3;
    uint8_t buffer[SIZE];

    read_ppm(argv[1], buffer, SIZE);

    for (int i = 0; i < 2 * 3; i += 3)
    {
        printf("%d, %d, %d\n", buffer[i], buffer[i+1], buffer[i+2]);
    }

    int h = 3, w = 3;

    uint8_t kernel[h * w * 3];

    int result = crop(buffer, 256, 256, kernel, 0, 0, h, w);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int pos = 3 * (i * h + j);
            printf("%d, %d, %d\n", kernel[pos], kernel[pos + 1], kernel[pos + 2]);
        }
    }
    
    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <image.ppm>\n", name);
    return 0;
}
