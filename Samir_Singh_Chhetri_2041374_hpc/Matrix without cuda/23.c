/******************************************************************************
        This program demonstrates matrix multiplication by utilizing cache memory.
	Compile this program with cc Task2.1.CacheUtilization.c -o Task2.1.CacheUtilization time_diff.c -lrt
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct timespec start, finish;
long long int time_elapsed;

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference)
{
    long long int ds = finish->tv_sec - start->tv_sec;
    long long int dn = finish->tv_nsec - start->tv_nsec;

    if (dn < 0)
    {
        ds--;
        dn += 1000000000;
    }
    *difference = ds * 1000000000 + dn;
    return !(*difference > 0);
}

int main()
{
    int N, M, P;
    int a[N][M], b[M][P], c[N][P];
    int i, j, k;

    printf("Enter number of rows of first matrix: ");
    scanf("%d", &N);
    printf("Enter number of columns of first matrix: ");
    scanf("%d", &M);
    printf("Number of Rows of second matrix = %d \n", M);
    printf("Enter number of columns of second matrix: ");
    scanf("%d", &P);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {

            a[i][j] = rand() % 100;
        }
    }

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < P; j++)
        {

            b[i][j] = rand() % 100;
        }
    }
    for (i = 0; i < M; i++)
    {
       for (k = 0; k < P; k++)
        {
            for (j = 0; j < N; j++)
            {

                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
    printf("\nThe results is\n");
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < P; j++)
        {
            printf("%d  ", c[i][j]);
        }
        printf("\n");
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    time_difference(&start, &finish, &time_elapsed);
    printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));

    return 0;
}
