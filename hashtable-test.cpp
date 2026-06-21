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
    size_t operator()(const std::vector<uint32_t>& v) const
    {
        size_t hash = 0;

        for (const uint32_t& i : v)
        {
            hash ^= std::hash<uint32_t>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
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

    //uint32_t buffer[bheight * bwidth * bdepth * btrength];
    size_t bsize = (size_t)bheight * (size_t)bwidth * (size_t)bdepth * (size_t)btrength;
    std::vector<uint32_t> buffer(bsize);

    if (fflag)
    {
        FILE* f = fopen(filename, "rb");
        //size_t read_bytes = fread(buffer, sizeof(buffer), 1, f);
        size_t read_bytes = fread(buffer.data(), sizeof(uint32_t), buffer.size(), f);
        hashname = filename;
        fclose(f);
    }
    else
    {
        hashname = argv[argc - 1];
    }

    typedef std::unordered_map<size_t, std::vector<std::pair<std::vector<uint32_t>, size_t>>> bit_map;
    std::vector<bit_map> bit_maps(BITS);

    auto insert = [&bit_maps](const std::vector<uint32_t>& neighborhood, int bit_index) {
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

    //uint32_t neighborhood1[nsize], neighborhood2[nsize];
    std::vector<uint32_t> neighborhood(nsize);

    int world_size = 1, world_rank = 0;

#if defined(USE_MPI)
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

    uint64_t bit_matches[BITS] = { 0 };

    int start = world_rank * (BITS) / world_size;
    int end = (world_rank + 1) * (BITS) / world_size;

    std::vector<uint32_t> bit_buffer(bsize);

    for (int b = start; b < end; b++)
    {
        for (size_t i = 0; i < buffer.size(); i++)
        {
            bit_buffer[i] = (buffer[i] >> b) & 1u;
        }

        for (int ny1 = 0; ny1 < bheight - nheight + 1; ny1++)
        {
            for (int nx1 = 0; nx1 < bwidth - nwidth + 1; nx1++)
            {
                for (int nz1 = 0; nz1 < bdepth - ndepth + 1; nz1++)
                {
                    for (int nw1 = 0; nw1 < btrength - ntrength  + 1; nw1++)
                    {
                        crop(bit_buffer.data(), bwidth, bheight, bdepth, btrength, neighborhood.data(), nx1, ny1, nz1, nw1, nwidth, nheight, ndepth, ntrength);
                        
                        insert(neighborhood, b);
                    }
                }
            }
        }

        for (const auto& [hash, bucket] : bit_maps[b])
        {
            for (const auto& [entry, count] : bucket)
            {
                if (count > 1)
                {
                    bit_matches[b] += (uint64_t)count * (uint64_t)(count - 1);
                }
            }
        }
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