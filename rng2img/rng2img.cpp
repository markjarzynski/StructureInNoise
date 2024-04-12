#include <stdio.h>
#include "random.h"

#define SIZE 32

int usage()
{
    fprintf(stderr, "Usage: rng2img hash > hash.ppm\n");
    return 1;
}


int main(int argc, char** argv)
{

    if (argc != 2)
        return usage();

    uint8_t (*pixels)[3] = new uint8_t[SIZE*SIZE][3];
    

    uint8_t (*pixel)[3] = pixels;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            uint3 v = pcg3d(uint3(i, j, 0u));

            (*pixel)[0] = uint8_t(v.x % 255);
            (*pixel)[1] = uint8_t(v.y % 255);
            (*pixel)[2] = uint8_t(v.z % 255);

            pixel++;
        }
    }

    /*
    FILE* f = fopen("out.ppm", "wb");
    fprintf(f, "P6\n%d %d\n%d\n", SIZE, SIZE, 255);
    fwrite(pixels, 1, SIZE*SIZE*3, f);
    fclose(f);
    */
    
    fprintf(stdout, "P6\n%d %d\n%d\n", SIZE, SIZE, 255);
    fwrite(pixels, 1, SIZE*SIZE*3, stdout);

    return 0;
}
