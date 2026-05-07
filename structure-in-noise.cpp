#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <inttypes.h>
#include <getopt.h>

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

static struct option long_options[] = {
    {"width",   optional_argument, 0, 'w'},
    {"height",  optional_argument, 0, 'h'},
    {"depth",   optional_argument, 0, 'd'},
    {"trength", optional_argument, 0, 't'},
    {"nwidth",   optional_argument, 0, 'i'},
    {"nheight",  optional_argument, 0, 'j'},
    {"ndepth",   optional_argument, 0, 'k'},
    {"ntrength", optional_argument, 0, 'l'},
};

int usage(char* name);

uint32_t compare(uint32_t* a, uint32_t* b, int width, int height, int length, int trength, int shift);
void compare_count(uint64_t* out, uint32_t* a, uint32_t* b, int width, int height, int depth, int trength, float threshold);

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int bheight = 128, bwidth = 128, bdepth = 1, btrength = 1;
    int xi = 0, yi = 0, zi = 0, wi = 0;
    int nheight = 3, nwidth = 3, ndepth = 1, ntrength = 1;

    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    uint time = current_time.tv_sec * 1000000 + current_time.tv_usec;

    char* hashname = NULL;
    char* filename = NULL;

    int c, fflag = 0, option_index = 0;
    while ((c = getopt_long(argc, argv, "x:y:z:v:rw:h:d:t:i:j:k:l:f:", long_options, &option_index)) != -1)
    {
        switch (c)
        {
        case 'x': // initial x position
            xi = atoi(optarg);
            break;
        case 'y': // initial y position
            yi = atoi(optarg);
            break;
        case 'z': // initial z position
            zi = atoi(optarg);
            break;
        case 'v': // initial w position
            wi = atoi(optarg);
            break;
        case 'r': // use random x/y positions
            xi = pcg(time);
            yi = pcg(xi);
            zi = pcg(yi);
            wi = pcg(zi);
            break;
        case 'w': // width
            bwidth = atoi(optarg);
            break;
        case 'h': // height
            bheight = atoi(optarg);
            break;
        case 'd': // depth
            bdepth = atoi(optarg);
            break;
        case 't': // trength
            btrength = atoi(optarg);
            break;
        case 'i': // Kernel size
            nwidth = atoi(optarg);
            break;
        case 'j':
            nheight = atoi(optarg);
            break;
        case 'k':
            ndepth = atoi(optarg);
            break;
        case 'l':
            ntrength = atoi(optarg);
            break;
        case 'f':
            fflag = 1;
            filename = optarg;
            break;
        }
    }

    //printf("x: %d, y: %d, w: %d, h: %d, k: %d\n", xi, yi, width, height, h);

    uint32_t buffer[bheight * bwidth * bdepth * btrength];

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
    }

    for (int y = 0; y < bheight; y++)
    {
        for (int x = 0; x < bwidth; x++)
        {
            for (int z = 0; z < bdepth; z++)
            {
                for (int w = 0; w < btrength; w++)
                {
                    buffer[((y * bwidth + x) * bdepth + z) * btrength + w] = hash2rgb(hashname, xi + x, yi + y, zi + z, wi + w);
                }
            }
        }
    }

    int nsize = nheight * nwidth * ndepth * ntrength; // neighborhood size

    uint32_t neighborhood1[nsize], neighborhood2[nsize];

    int world_size = 1, world_rank = 0;

#if defined(USE_MPI)
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

    uint64_t matches = 0;
    uint64_t bitplane_matches[BITS] = { 0 };
    uint64_t count_matches[BITS] = { 0 };

    int start = world_rank * (bheight - nheight + 1) / world_size;
    int end = (world_rank + 1) * (bheight - nheight + 1) / world_size;

    for (int ny1 = start; ny1 < end; ny1++)
    {
        for (int nx1 = 0; nx1 < bwidth - nwidth + 1; nx1++)
        {
            for (int nz1 = 0; nz1 < bdepth - ndepth + 1; nz1++)
            {
                for (int nw1 = 0; nw1 < btrength - ntrength  + 1; nw1++)
                {
                    crop(buffer, bwidth, bheight, bdepth, btrength, neighborhood1, nx1, ny1, nz1, nw1, nwidth, nheight, ndepth, ntrength);

                    for (int ny2 = 0; ny2 < bheight - nheight + 1; ny2++)
                    {
                        for (int nx2 = 0; nx2 < bwidth - nwidth + 1; nx2++)
                        {
                            for (int nz2 = 0; nz2 < bdepth - ndepth + 1; nz2++)
                            {
                                for (int nw2 = 0; nw2 < btrength - ntrength + 1; nw2++)
                                {
                                    crop(buffer, bwidth, bheight, bdepth, btrength, neighborhood2, nx2, ny2, nz2, nw2, nwidth, nheight, ndepth, ntrength);
                                
                                    for (int b = 0; b < BITS; b++)
                                    {
                                        if ((nx1 == nx2) && (ny1 == ny2) && (nz1 == nz2) && (nw1 == nw2) && (b == 0))
                                        {
                                            rotr(neighborhood2, nsize, 1);
                                            continue; // same index and same bit
                                        }

                                        uint32_t comp = compare(neighborhood1, neighborhood2, nwidth, nheight, ndepth, ntrength, 0);
                                        matches += popcnt(comp);

                                        if (b == 0)
                                        {
                                            for (int a = 0; a < BITS; a++)
                                            {
                                                bitplane_matches[a] += (comp >> a) & 1;
                                            }

                                            compare_count(count_matches, neighborhood1, neighborhood2, nwidth, nheight, ndepth, ntrength, 1.0f);
                                        }

                                        rotr(neighborhood2, nsize, 1);
                                    }
                                }
                            }
                        }
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

        printf("%s", hashname);

        for (int b = 0; b < BITS; b++)
        {
            printf(",%" PRIu64, count_matches[b]);
        }

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

uint32_t compare(uint32_t* a, uint32_t* b, int width, int height, int depth, int trength, int shift)
{
    uint32_t result = UINT32_MAX >> shift;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < depth; k++)
            {
                for (int l = 0; l < trength; l++)
                {
                    result &= ~(a[((i * width + j) * depth + k) * trength + l] ^ b[((i * width + j) * depth + k) * trength + l]);
                }
            }
        }
    }

    return result;
}

uint32_t max_count(int width, int height, int depth, int trength)
{
    return width * height * depth * trength;
}

void compare_count(uint64_t* out, uint32_t* a, uint32_t* b, int width, int height, int depth, int trength, float threshold)
{
    uint32_t count[BITS] = { 0 };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < depth; k++)
            {
                for (int l = 0; l < trength; l++)
                {
                    uint32_t result = ~(a[((i * width + j) * depth + k) * trength + l] ^ b[((i * width + j) * depth + k) * trength + l]);
                    
                    for (int m = 0; m < BITS; m++)
                    {
                        count[m] += (result >> m) & 1u;
                    }
                }
            }
        }
    }

    uint32_t iThreshold = threshold * max_count(width, height, depth, trength);
    for (int m = 0; m < BITS; m++)
    {
        if (count[m] >= iThreshold)
        {
            out[m]++;
        }
    }
}
