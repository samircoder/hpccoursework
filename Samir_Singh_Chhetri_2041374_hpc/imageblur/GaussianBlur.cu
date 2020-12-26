#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"


//compile with c++ lodepng file

//nvcc GaussianBlur.cu lodepng.cpp

__global__ void blurKernel(unsigned char * gpu_imageOuput, unsigned char * gpu_imageInput,unsigned int width,unsigned int height) 
{ 
	        int r = 0;
		int g = 0;
		int b = 0;
		int t = 0;
		int x,y;
		int count = 0;

		int idx = blockDim.x * blockIdx.x + threadIdx.x;
		int pixel = idx*4;

		for(x = (pixel - 4); x <=  (pixel + 4); x+=4){
			// Checking conditions so pixel is available at x
			if ((x > 0) && x < (height * width * 4) && ((x-4)/(4*width) == pixel/(4*width))){
				for(y = (x - (4 * width)); y <=  (x + (4 * width)); y+=(4*width)){
					if(y > 0 && y < (height * width * 4)){
						r += gpu_imageInput[y];
						g += gpu_imageInput[1+y];
						b += gpu_imageInput[2+y]; 
						count++;
					}
				}
			}
		}
		
		t = gpu_imageInput[3+pixel];

		gpu_imageOuput[pixel] = r / count;
		gpu_imageOuput[1+pixel] = g / count;
		gpu_imageOuput[2+pixel] = b / count;
		gpu_imageOuput[3+pixel] = t;
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

int main(int argc, char **argv){

struct timespec start, finish;
    	long long int time_elapsed;
    	 //Start Timer
    	clock_gettime(CLOCK_MONOTONIC, &start);
    	
	unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	const char* filename = "hck.png";
	const char* newFileName = "Output.png";

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error){
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	const int ARRAY_SIZE = width*height*4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++) {
		host_imageInput[i] = image[i];
	}

	// declare GPU memory pointers
	unsigned char * d_in;
	unsigned char * d_out;

	// allocate GPU memory
	cudaMalloc((void**) &d_in, ARRAY_BYTES);
	cudaMalloc((void**) &d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel
	blurKernel<<<height, width>>>(d_out, d_in, width,height);

	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);
	
	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}

	cudaFree(d_in);
	cudaFree(d_out);
	
	clock_gettime(CLOCK_MONOTONIC, &finish);
        time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9));
	
	

	return 0;
}
