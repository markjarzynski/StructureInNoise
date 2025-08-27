#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <inttypes.h>

#if defined(USE_MPI)
#include <mpi.h>
#endif

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

uint32_t compare(uint32_t* a, uint32_t* b, int w, int h, int shift);
 
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

    int c, fflag = 0;
    while ((c = getopt(argc, argv, "x:y:rw:h:k:f:")) != -1)
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

    int SIZE = height * width * 3; // window size
    int s = h * w; // neighborhood size

    uint32_t neighborhood1[s], neighborhood2[s];

    int world_size = 1, world_rank = 0;

#if defined(USE_MPI)
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

    uint64_t matches = 0;
    uint64_t bitplane_matches[BITS] = { 0 };

    int start = world_rank * (height - h + 1) / world_size;
    int end = (world_rank + 1) * (height - h + 1) / world_size;

    for (int y = start; y < end; y++)
    {
        for (int x = 0; x < width - w + 1; x++)
        {
            crop(buffer, width, height, neighborhood1, x, y, w, h);

            for (int j = 0; j < height - h + 1; j++)
            {
                for (int i = 0; i < width - w + 1; i++)
                {
                    crop(buffer, width, height, neighborhood2, i, j, w, h);

                    for (int b = 0; b < BITS; b++)
                    {
                        if ((i == x) && (j == y) && (b == 0))
                        {
                            rotr(neighborhood2, s, 1);
                            continue; // same index and same bit
                        }
                        uint32_t comp = compare(neighborhood1, neighborhood2, w, h, 0); 
                        matches += popcnt(comp);
                        
                        if (b == 0)
                        {
                            for (int a = 0; a < BITS; a++)
                            {
                                bitplane_matches[a] += (comp >> a) & 1;
                            }
                        }
        
                        rotr(neighborhood2, s, 1);
                    }
                }
            }
        }
    }

#if defined(USE_MPI)
    uint64_t* world_matches = NULL;
    uint64_t* world_bitplane_matches = NULL;
    if (world_rank == 0)
    {
        world_matches = (uint64_t*)malloc(sizeof(uint64_t) * world_size);
        world_bitplane_matches = (uint64_t*)malloc(sizeof(uint64_t) * world_size * BITS);
    }

    MPI_Gather(&matches, 1, MPI_UINT64_T, world_matches, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);
    MPI_Gather(bitplane_matches, BITS, MPI_UINT64_T, world_bitplane_matches, BITS, MPI_UINT64_T, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            matches += world_matches[i];
            for (int b = 0; b < BITS; b++)
            {
                bitplane_matches[b] += world_bitplane_matches[i * BITS + b];
            }
        }
    }
#endif

    if (world_rank == 0)
    {
        printf("%s", hashname);

        for (int b = 0; b < BITS; b++)
        {
            printf(",%" PRIu64, bitplane_matches[b]);
        }
        printf(",%" PRIu64, matches);

        printf("\n");
    }

#if defined(USE_MPI)
    MPI_Finalize();
#endif

    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <hashname>\n", name);
    return 0;
}

uint32_t compare(uint32_t* a, uint32_t* b, int w, int h, int shift)
{
    uint32_t result = UINT32_MAX >> shift;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            result &= ~(a[j * w + i] ^ b[j * w + i]);
        }
    }

    return result;
}
