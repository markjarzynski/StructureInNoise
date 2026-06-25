#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <inttypes.h>
#include <getopt.h>
#include <unordered_map>
#include <vector>

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

struct Hash
{
    size_t operator()(const std::vector<uint8_t>& v) const
    {
        size_t hash = 0;

        for (const uint32_t& i : v)
        {
            hash ^= (size_t)std::hash<uint8_t>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        return hash;
    }
};

int main(int argc, char** argv)
{
    if (argc < 2)
        return usage(argv[0]);

    int bheight = 128, bwidth = 128, bdepth = 1, btrength = 1;
    int xi = 0, yi = 0, zi = 0, wi = 0;
    int nheight = 5, nwidth = 5, ndepth = 1, ntrength = 1;

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

    size_t bsize = (size_t)bheight * (size_t)bwidth * (size_t)bdepth * (size_t)btrength;
    std::vector<uint32_t> buffer;

    if (fflag)
    {
        buffer.resize(bsize);
        FILE* f = fopen(filename, "rb");
        size_t read_bytes = fread(buffer.data(), sizeof(uint32_t), buffer.size(), f);
        hashname = filename;
        fclose(f);
    }
    else
    {
        hashname = argv[argc - 1];
    }

    int world_size = 1, world_rank = 0;

#if defined(USE_MPI)
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

    //printf("World size: %d, rank: %d\n", world_size, world_rank);

    // check to ensure world_size is a power of 2
    if (!(world_size > 0 && !(world_size & (world_size - 1))))
    {
        fprintf(stderr, "World size (%d) is not a power of 2!\n", world_size); 
        return 0;
    }

    int start = world_rank * (BITS) / world_size;
    int end = (world_rank + 1) * (BITS) / world_size;

    int nsize = nheight * nwidth * ndepth * ntrength; // neighborhood size
    std::vector<uint8_t> neighborhood(nsize);

    typedef std::unordered_map<size_t, std::vector<std::pair<std::vector<uint8_t>, size_t>>> bit_map;
    std::vector<bit_map> bit_maps(end - start);

    auto insert = [&bit_maps](const std::vector<uint8_t>& neighborhood, int bit_index) {
        size_t hash = Hash{}(neighborhood);

        auto& bucket = bit_maps[bit_index][hash];

        for (auto& [entry, count] : bucket)
        {
            if (entry == neighborhood)
            {
                count++;
                return;
            }
        }

        bucket.push_back({ neighborhood, 1 });
    };

    uint64_t bit_matches[BITS] = { 0 };


    for (int b = start; b < end; b++)
    {
        for (int ny1 = 0; ny1 < bheight - nheight + 1; ny1++)
        {
            for (int nx1 = 0; nx1 < bwidth - nwidth + 1; nx1++)
            {
                for (int nz1 = 0; nz1 < bdepth - ndepth + 1; nz1++)
                {
                    for (int nw1 = 0; nw1 < btrength - ntrength + 1; nw1++)
                    {
                        //printf("%d: %d %d %d %d, %d %d %d %d, %d %d %d %d\n", world_rank, bwidth, bheight, bdepth, btrength, nx1, ny1, nz1, nw1, nwidth, nheight, ndepth, ntrength);
                        
                        // get neighborhood
                        if (fflag)
                        {
                            crop(buffer.data(), bwidth, bheight, bdepth, btrength, neighborhood.data(), nx1, ny1, nz1, nw1, nwidth, nheight, ndepth, ntrength, b);
                        }
                        else
                        {
                            for (int y = 0; y < nheight; y++)
                            {
                                for (int x = 0; x < nwidth; x++)
                                {
                                    for (int z = 0; z < ndepth; z++)
                                    {
                                        for (int w = 0; w < ntrength; w++)
                                        {
                                            uint32_t hash = hash2rgb(hashname, nx1 + xi + x, ny1 + yi + y, nz1 + zi + z, nw1 + wi + w);
                                            neighborhood[((y * nwidth + x) * ndepth + z) * ntrength + w] = (uint8_t)((hash >> b) & 1u);
                                        }
                                    }
                                }
                            }
                        }
                        
                        insert(neighborhood, b - start);
                    }
                }
            }
        }

        printf("rank %d done inserting into hash table.\n", world_rank);

        for (const auto& [hash, bucket] : bit_maps[b - start])
        {
            for (const auto& [entry, count] : bucket)
            {
                if (count > 1)
                {
                    bit_matches[b] += (uint64_t)count * (uint64_t)(count - 1);
                }
            }
        }

        printf("bit: %d, matches: %" PRIu64 "\n", b, bit_matches[b]);
    }

#if defined(USE_MPI)
    uint64_t* world_bit_matches = NULL;
    if (world_rank == 0)
    {
        world_bit_matches = (uint64_t*)malloc(sizeof(uint64_t) * world_size * BITS);
    }

    MPI_Gather(bit_matches, BITS, MPI_UINT64_T, world_bit_matches, BITS, MPI_UINT64_T, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            for (int b = 0; b < BITS; b++)
            {
                bit_matches[b] += world_bit_matches[i * BITS + b];
            }
        }
    }
#endif

    if (world_rank == 0)
    {
        printf("%s", hashname);

        for (int b = 0; b < BITS; b++)
        {
            printf(",%" PRIu64, bit_matches[b]);
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
