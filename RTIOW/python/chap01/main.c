/*
 * A simple program to write a PPM file.
 * From Chapter 1 of "Ray Tracing in one Weekend".
 */

#include <stdio.h>

int
main(void)
{
    int nx = 200;
    int ny = 100;

    printf("P3\n%d %d 255\n", nx, ny);

    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float r = (float) i / (float) nx;
            float g = (float) j / (float) ny;
            float b = 0.2;

            int ir = (int) (255.99 * r);
            int ig = (int) (255.99 * g);
            int ib = (int) (255.99 * b);

            printf("%d %d %d\n", ir, ig, ib);
        }
    }

    return 0;
}
