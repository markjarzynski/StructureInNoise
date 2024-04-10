#include <stdio.h>
#include "random.h"

#define SIZE 32

int main(int argc, char** argv)
{
    printf("hello\n");

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            uint3 v = pcg3d(uint3(i, j, 0u));
            printf("%d %d %d\n", v.x, v.y, v.z);
            
            
        }
    }

    return 0;
}
