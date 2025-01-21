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

    uint8_t kernel[6];

    int result = crop(buffer, SIZE, SIZE, kernel, 0, 0, 1);

    printf("%d, %d, %d\n", kernel[0], kernel[1], kernel[2]);
    printf("%d, %d, %d\n", kernel[3], kernel[4], kernel[5]);
    
    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <image.ppm>\n", name);
    return 0;
}

int crop(uint8_t* pixels, int width, int height, uint8_t* kernel, int x, int y, int w, int h)
{
    for (int i = 0; i <= h; i++)
    {
        for (int j = 0; j <= w; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                kernel[i * h + j + k] = pixels[3 * (height + i) * y + 3 * x + j + k];
            }
        }
    }

    kernel[0] = pixels[3 * height * y + 3 * x + 0];
    kernel[1] = pixels[3 * height * y + 3 * x + 1];
    kernel[2] = pixels[3 * height * y + 3 * x + 2];

    kernel[3 + 0] = pixels[3 * height * y + 3 * x + 3 + 0];
    kernel[3 + 1] = pixels[3 * height * y + 3 * x + 3 + 1];
    kernel[3 + 2] = pixels[3 * height * y + 3 * x + 3 + 2];

    return 1;
}
