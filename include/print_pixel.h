#pragma once

#include <stdint.h>

void print_pixel(uint8_t* pixels, int width, int height, int x, int y, int channels)
{
    printf("[");
    for (int c = 0; c < channels; c++)
    {
        if (c > 0)
        {
            printf(",");
        }

        printf("%3d", pixels[channels * (height * y + x) + c]);
    }
    printf("]");
}

void print_pixels(uint8_t* pixels, int width, int height, int x, int y, int w, int h, int channels)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            print_pixel(pixels, width, height, x + j, y + i, channels);
            
            if (j < w - 1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void print_uint3(uint3* pixels, int width, int height, int x, int y)
{
    uint3 pixel = pixels[height * y + x];
    printf("[");
    printf("%u,%u,%u", pixel.x, pixel.y, pixel.z);
    printf("]");
}

void print_uint3(uint3* pixels, int width, int height, int x, int y, int w, int h)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            print_uint3(pixels, width, height, x + j, y + i);
            printf(" ");
        }
        printf("\n");
    }
}
