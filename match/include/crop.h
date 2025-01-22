#pragma once

#include <stdio.h>
#include <stdint.h>

int crop(uint8_t* pixels, int width, int height, uint8_t* kernel, int x, int y, int w, int h)
{
    printf("width: %d, height %d\n", width, height);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int p = 3 * (i * h + j) + k;
                int q = 3 * (height * (y + i) + x + j) + k;
                printf("%d, %d: %d, %d\n", p, q, kernel[p], pixels[q]);
                kernel[3 * (i * h + j) + k] = pixels[3 * (height * (y + i) + x + j) + k];
            }
        }
    }

    
    /*
    kernel[0] = pixels[3 * height * y + 3 * x + 0];
    kernel[1] = pixels[3 * height * y + 3 * x + 1];
    kernel[2] = pixels[3 * height * y + 3 * x + 2];

    kernel[3 + 0] = pixels[3 * height * y + 3 * x + 3 + 0];
    kernel[3 + 1] = pixels[3 * height * y + 3 * x + 3 + 1];
    kernel[3 + 2] = pixels[3 * height * y + 3 * x + 3 + 2];
    */
    return 1;
}
