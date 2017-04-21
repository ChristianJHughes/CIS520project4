// includes, system
#include <stdio.h>
#include <assert.h>

#define ARRAY_SIZE 2000000
#define STRING_SIZE 16
int char_array[ARRAY_SIZE*STRING_SIZE];
int char_counts[26];

char getRandomChar()
{
	int randNum = 0;
	char randChar = ' ';

	randNum = 26 * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
	randNum = randNum + 97;				// scale to 'a'
	randChar = (char) randNum;

	// printf("%c", randChar);
	return randChar;
}

void init_arrays()
{
  int i, j, randNum;
  char randChar;

  for ( i = 0; i < ARRAY_SIZE *STRING_SIZE; i++) {
		 char_array[i] = (int)getRandomChar();
	
  }

  for ( i = 0; i < 26; i++ ) {
  	char_counts[i] = 0;
  }
}

void checkCUDAError(const char* msg);


__global__ void countChar(int *d_out, int *d_in, int perThread, int numThreads)
{
    int size = ARRAY_SIZE*STRING_SIZE;
    
    int i;
    int inOffset  = blockDim.x * blockIdx.x;
    int in  = inOffset + threadIdx.x;
    int theChar;
    int charLoc;
    for(i = 0; i < perThread; i++)
    {
        theChar = d_in[in*perThread+i];
        charLoc = theChar - 97;
        d_out[in*26+charLoc]++;
    }
    
}

void print_results(int* results, int totalRuns)
{
    int i, j, total = 0, count;
    for(i = 0; i < 26; i++)
    {
        count = 0;
        for(j = 0; j < totalRuns; j++)
        {
            count += results[j*26+i];            
        }
        total += count;
        printf(" %c %d\n", (char) (i + 97), count);
    }
    printf("\nTotal characters:  %d\n", total);
}

/////////////////////////////////////////////////////////////////////
// Program main
/////////////////////////////////////////////////////////////////////
int main( int argc, char** argv) 
{

    int totalSize = ARRAY_SIZE * STRING_SIZE; 
    int perThread = 10000;
    int size = totalSize/perThread;
    int *deviceInput, *deviceAnswer;
    int numThreadsPerBlock = 100;
    int numBlocks =  size / numThreadsPerBlock; 

    int totalThreads = numThreadsPerBlock * numBlocks; 

    // allocate host and device memory
    size_t memSize = ARRAY_SIZE*STRING_SIZE * sizeof(int);
    cudaMalloc((void **) &deviceInput, memSize );
    cudaMalloc((void **) &deviceAnswer, totalThreads * 26 * sizeof(int));
    int* localReturn = (int*)malloc(sizeof(int)*26*totalThreads);

    
    init_arrays();
    
    cudaMemcpy( deviceInput, char_array, memSize, cudaMemcpyHostToDevice);
    cudaMemset( deviceAnswer, 0, totalThreads*26*sizeof(int));
    // launch kernel
    dim3 dimGrid(numBlocks);
    dim3 dimBlock(numThreadsPerBlock);
    countChar <<< dimGrid, dimBlock >>>( deviceAnswer, deviceInput, perThread, totalThreads);

    
    cudaThreadSynchronize();
    checkCUDAError("kernel invocation");

    cudaMemcpy( localReturn , deviceAnswer, totalThreads*26*sizeof(int), cudaMemcpyDeviceToHost );
    checkCUDAError("memcpy");

    cudaFree(deviceAnswer);
    cudaFree(deviceInput);

    print_results(localReturn, totalThreads);

    return 0;
}
void checkCUDAError(const char *msg)
{
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        fprintf(stderr, "Cuda error: %s: %s.\n", msg, 
                             cudaGetErrorString( err) );
        exit(EXIT_FAILURE);
    }                         
}

