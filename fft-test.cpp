#include <getopt.h>
#include <sys/time.h>
#include <fftw3.h>
#include <inttypes.h>

#include "random.h"
#include "hash2rgb.h"

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

void crop(double* output, double* input,
    uint32_t bwidth, uint32_t bheight, uint32_t bdepth, uint32_t btrength,
    uint32_t xi, uint32_t yi, uint32_t zi, uint32_t wi,
    uint32_t cwidth, uint32_t cheight, uint32_t cdepth, uint32_t ctrength);

void multiply(fftw_complex* result, fftw_complex* a, fftw_complex* b, size_t size);

uint64_t test(double* input, size_t size);

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

    size_t bsize = bheight * bwidth * bdepth * btrength;
    uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t) * bsize);

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



    double* double_buffer = (double*)malloc(sizeof(double) * bsize);
    double* crop_buffer = (double*)malloc(sizeof(double) * bsize);
    double* reconstructed = (double*)malloc(sizeof(double) * bsize);

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

    // TODO: MPI Scatter/Gather buffer
    fftw_complex* fft_result = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * bsize);
    fftw_complex* crop_result = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * bsize);
    fftw_complex* multiply_result = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * bsize);

    int ranks[4] = {bheight, bwidth, bdepth, btrength};
    fftw_plan forward_plan = fftw_plan_dft_r2c(4, ranks, double_buffer, fft_result, FFTW_ESTIMATE);
    fftw_plan crop_forward_plan = fftw_plan_dft_r2c(4, ranks, crop_buffer, crop_result, FFTW_ESTIMATE);
    fftw_plan inverse_plan = fftw_plan_dft_c2r(4, ranks, multiply_result, reconstructed, FFTW_ESTIMATE);

    uint64_t score = 0u;

    for (int b = 0; b < 32; b++)
    {
        for (size_t i = 0; i < bsize; i++)
        {
            double_buffer[i] = (double)((buffer[i] >> b) & 1u);
        }

        fftw_execute(forward_plan);

        for (int y = 0; y < bheight - nheight; y++)
        {
            for (int x = 0; x < bwidth - nwidth; x++)
            {
                for (int z = 0; z < bdepth - ndepth; z++)
                {
                    for (int w = 0; w < btrength - ntrength; w++)
                    {
                        crop(crop_buffer, double_buffer, bwidth, bheight, bdepth, btrength, x, y, z, w, nwidth, nheight, ndepth, ntrength);

                        fftw_execute(crop_forward_plan);

                        multiply(multiply_result, fft_result, crop_result, bsize);

                        fftw_execute(inverse_plan);

                        score += test(reconstructed, bsize);
                    }
                }
            }
        }
    }

    printf(PRIu64 "\n", score);

    fftw_destroy_plan(forward_plan);
    fftw_destroy_plan(crop_forward_plan);
    fftw_destroy_plan(inverse_plan);

    fftw_free(fft_result);
    fftw_free(crop_result);
    fftw_free(multiply_result);

    free(buffer);
    free(double_buffer);
    free(crop_buffer);
    free(reconstructed);

    return 0;
}

int usage(char* name)
{
    printf("Usage: %s <hashname>\n", name);
    return 0;
}

void crop(double* output, double* input,
    uint32_t bwidth, uint32_t bheight, uint32_t bdepth, uint32_t btrength,
    uint32_t xi, uint32_t yi, uint32_t zi, uint32_t wi,
    uint32_t cwidth, uint32_t cheight, uint32_t cdepth, uint32_t ctrength)
{
    memset(output, 0, bwidth * bheight * bdepth * btrength);

    for (uint32_t y = yi; y < cheight; y++)
    {
        for (uint32_t x = xi; x < cwidth; x++)
        {
            for (uint32_t z = zi; z < cdepth; z++)
            {
                for (uint32_t w = wi; w < ctrength; w++)
                {
                    output[((y * bwidth + x) * bdepth + z) * btrength + w] = input[((y * bwidth + x) * bdepth + z) * btrength + w];
                }
            }
        }
    }
}

void multiply(fftw_complex* result, fftw_complex* a, fftw_complex* b, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        result[i][0] = a[i][0] * b[i][0] - a[i][1] * b[i][1];
        result[i][1] = a[i][0] * b[i][1] + a[i][1] * b[i][0];
    }
}

uint64_t test(double* input, size_t size)
{
    uint64_t score = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (input[i] > 0.0)
            score++;
    }
    return score;
}