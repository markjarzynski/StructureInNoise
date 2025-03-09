#pragma once

#include <stdio.h>
#include <stdint.h>

#include "uint3.h"

int crop(uint8_t* pixels, int width, int height, uint8_t* kernel, int x, int y, int w, int h, int channels)
{
    //printf("width: %d, height %d\n", width, height);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                int p = channels * (i * h + j) + k;
                int q = channels * (height * (y + i) + x + j) + k;
                //printf("%d, %d: %d, %d\n", p, q, kernel[p], pixels[q]);
                kernel[channels * (i * h + j) + k] = pixels[channels * (height * (y + i) + x + j) + k];
            }
        }
    }
 
    return 1;
}

int crop(uint3* pixels, int width, int height, uint3* kernel, int x, int y, int w, int h)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            kernel[i * w + j] = pixels[width * (y + i) + x + j];
        }
    }

    return 1;
}
