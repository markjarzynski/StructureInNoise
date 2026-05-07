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

int crop(uint32_t* pixels, int bwidth, int bheight, int bdepth, int btrength, uint32_t* crop, int cx, int cy, int cz, int cw, int cwidth, int cheight, int cdepth, int ctrength)
{
    for (int i = 0; i < cheight; i++)
    {
        for (int j = 0; j < cwidth; j++)
        {
            for (int k = 0; k < cdepth; k++)
            {
                for (int l = 0; l < ctrength; l++)
                {
                    crop[((i * cwidth + j) * cdepth + k) * ctrength + l] = pixels[(((cy + i) * bwidth + cx + j) * bdepth + cz + k) * btrength + cw + l];
                }
            }
        }
    }

    return 1;
}
