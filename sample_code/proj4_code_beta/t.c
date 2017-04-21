#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 8 

#define ARRAY_SIZE 80000000
#define STRING_SIZE 16
#define ALPHABET_SIZE 26

omp_lock_t *mutexsum;			// mutex for char_counts

char char_array[ARRAY_SIZE][STRING_SIZE];
int local_char_count[NUM_THREADS][ALPHABET_SIZE];			// count of individual characters for each thread
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

  for ( i = 0; i < NUM_THREADS; i++ ) {
		for ( j = 0; j < ALPHABET_SIZE; j++ ) {
		 	local_char_count[i][j] = 0;
		}
  }

  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
  	char_counts[i] = 0;
  }
}

void *count_array()
{
	char theChar;
	int i, j, charLoc;
	int myID, startPos, endPos;

	omp_set_num_threads(NUM_THREADS);

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(myID, theChar, charLoc, startPos, endPos, i, j)
	{ 												// begin parallel section
		myID = omp_get_thread_num();

		startPos = myID * (ARRAY_SIZE / NUM_THREADS);
		endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

		printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);

						// count up our section of the global array
		for ( i = startPos; i < endPos; i++) {
			for ( j = 0; j < STRING_SIZE; j++ ) {
					 theChar = char_array[i][j];
					 charLoc = ((int) theChar) - 97;
					 local_char_count[myID][charLoc]++;
			}
		}
	} 												// end parallel section

													// sum up local totals to global array
	for ( i = 0; i < NUM_THREADS; i++ ) {
		for ( j = 0; j < ALPHABET_SIZE; j++ ) {
		 	char_counts[j] += local_char_count[i][j];
		}
	}
}

void print_results(int the_char_counts[])
{
  int i,j, total = 0;

  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     total += the_char_counts[i];
     printf(" %c %d\n", (char) (i + 97), the_char_counts[i]);
  }
  printf("\nTotal characters:  %d\n", total);
}

main() {

	init_arrays();

	count_array();

	print_results(char_counts);

	printf("Main: program completed. Exiting.\n");
}

