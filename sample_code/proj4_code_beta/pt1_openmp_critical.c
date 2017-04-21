#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4

#define ARRAY_SIZE 8000000
#define STRING_SIZE 16
#define ALPHABET_SIZE 26

char char_array[ARRAY_SIZE][STRING_SIZE];
int char_counts[ALPHABET_SIZE];			// count of individual characters

char getRandomChar()
{
	int randNum = 0;
	char randChar = ' ';

	randNum = ALPHABET_SIZE * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
	randNum = randNum + 97;				// scale to 'a'
	randChar = (char) randNum;

	// printf("%c", randChar);
	return randChar;
}

void init_arrays()
{
  int i, j;

  for ( i = 0; i < ARRAY_SIZE; i++) {
	for ( j = 0; j < STRING_SIZE; j++ ) {
		 char_array[i][j] = getRandomChar();
	}
  }

  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
  	char_counts[i] = 0;
  }
}

void *count_array(int myID)
{
	char theChar;
	int i, j, charLoc;
	int local_char_count[ALPHABET_SIZE];
	int startPos, endPos;

	#pragma omp private(myID,theChar,charLoc,local_char_count,startPos,endPos,i,j)
	{
		startPos = myID * (ARRAY_SIZE / NUM_THREADS);
		endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

		printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);

						// init local count array
		for ( i = 0; i < ALPHABET_SIZE; i++ ) {
			local_char_count[i] = 0;
		}
						// count up our section of the global array
		for ( i = startPos; i < endPos; i++) {
			for ( j = 0; j < STRING_SIZE; j++ ) {
					 theChar = char_array[i][j];
					 charLoc = ((int) theChar) - 97;
					 local_char_count[charLoc]++;
			}
		}

						// sum up the partial counts into the global arrays
		#pragma omp critical
		{
			for ( i = 0; i < ALPHABET_SIZE; i++ ) {
		 		char_counts[i] += local_char_count[i];
			}
		}
	}
}

void print_results(int the_char_counts[])
{
  int i,j, total = 0;

  					// then print out the totals
  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     total += the_char_counts[i];
     printf(" %c %d\n", (char) (i + 97), the_char_counts[i]);
  }
  printf("\nTotal characters:  %d\n", total);
}

main() {

	omp_set_num_threads(NUM_THREADS);

	init_arrays();

	#pragma omp parallel 
	{
		count_array(omp_get_thread_num());
	}

	print_results(char_counts);

	printf("Main: program completed. Exiting.\n");
}

