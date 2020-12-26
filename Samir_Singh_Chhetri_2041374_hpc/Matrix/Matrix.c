/******************************************************************************
	This program demonstrates matrix multiplication with threads.
	Compile this program with cc -o Matrix  Matrix.c  -pthread

******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define MAT_SIZE 1024
int MAX_THREADS;


int A,B,C;         //Parameters For Rows And Columns
int matrix1[MAT_SIZE][MAT_SIZE]; //First Matrix
int matrix2[MAT_SIZE][MAT_SIZE]; //Second Matrix
int result [MAT_SIZE][MAT_SIZE]; //Multiplied Matrix
int step_i = 0;

//Type Defining For Passing Function Argumnents
//typedef struct parameters {
//    int x,y;
//}args;

//Function For Calculate Each Element in Result Matrix Used By Threads - - -//
void* mult(void* arg){


    int core = step_i++;
    //Calculating Each Element in Result Matrix Using Passed Arguments
    for (int i = core * A / MAX_THREADS; i < (core + 1) * A / MAX_THREADS; i++)
        for (int k = 0; k< C; k++)
            for (int j = 0; j< B; j++)
                result[i][j] += matrix1[i][k] * matrix2[k][j];
    //sleep(3);

    //End Of Thread
    pthread_exit(0);
}
int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(){
    struct timespec start, finish;
    long long int time_elapsed;
    // Getting Row And Column(Same As Row In Matrix2) Number For Matrix1
    printf("Enter number of rows in matrix 1: ");
    scanf("%d",&A);
    printf("Enter number of columns in matrix 1: ");
    scanf("%d",&B);
    printf("Enter number of columns in matrix 2: ");
    scanf("%d",&C);
    printf("Enter the number for threads you want to create  \n");
    scanf("%d",&MAX_THREADS);


    for(int x=0;x<A;x++){
        for(int y=0;y<B;y++){
            matrix1[x][y]=rand()%50;
        }
    }

    for(int x=0;x<B;x++){
        for(int y=0;y<C;y++){
            matrix2[x][y]=rand()%50;
        }
    }

    pthread_t *t = malloc(sizeof(pthread_t) * MAX_THREADS);

    //Defining Threads
    pthread_t thread[MAX_THREADS];

    //Counter For Thread Index
    int thread_number = 0;

    //Defining p For Passing Parameters To Function As Struct


    //Start Timer
    clock_gettime(CLOCK_MONOTONIC, &start);


    for (int x = 0; x < MAX_THREADS; x++)
    {
        int *p;
            //Status For Checking Errors
            int status;

            //Create Specific Thread For Each Element In Result Matrix
            status = pthread_create(&thread[thread_number], NULL, mult, (void *) &p[thread_number]);

            //Check For Error
            if(status!=0){
                printf("Thread Error");
                exit(0);
            }

            thread_number++;
        }





    //Wait For All Threads Done - - - - - - - - - - - - - - - - - - - - - - //

   for (int z = 0; z < MAX_THREADS; z++)
        pthread_join(thread[z], NULL);


    //Print Multiplied Matrix (Result) - - - - - - - - - - - - - - - - - - -//

    printf(" --- Multiplied Matrix ---\n\n");
    for(int x=0;x<A;x++){
        for(int y=0;y<C;y++){
            printf("%5d",result[x][y]);
        }
        printf("\n\n");
    }


    //Calculate Total Time Including 3 Soconds Sleep In Each Thread - - - -//

    	clock_gettime(CLOCK_MONOTONIC, &finish);
        time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9));


    //Total Threads Used In Process - - - - - - - - - - - - - - - - - - - -//

    printf(" ---> Used Threads : %d \n\n",thread_number);
    printf("Number of rows for matrix 1: %d \n",A);
    printf("Number of columns for matrix 1: %d \n",B);
    printf("Number of rows for matrix 2: %d \n",B);
    printf("Number of columns for matrix 2: %d \n",C);
    for(int z=0;z<thread_number;z++)
        printf(" - Thread %d ID : %d\n",z+1,(int)thread[z]);
    free(t);
    return 0;
}
