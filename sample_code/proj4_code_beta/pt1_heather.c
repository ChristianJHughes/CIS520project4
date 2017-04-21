#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 12

#define CHUNK_SIZE 80000		// H- size of chunk we're reading from file
#define STRING_SIZE 1024			// H- size of individual genome lines
#define ALPHABET_SIZE 26

pthread_mutex_t mutexsum;			// mutex for char_counts

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

  pthread_mutex_init(&mutexsum, NULL);

  for ( i = 0; i < ARRAY_SIZE; i++) {
	for ( j = 0; j < STRING_SIZE; j++ ) {
		 char_array[i][j] = getRandomChar();
	}
  }

  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
  	char_counts[i] = 0;
  }
}


//So, Heather - I suggest adding some code to not count special characters, and keep track of how many characters you count.
void *count_array(void *myID)
{
  char theChar;
  int i, j, charLoc;
  int local_char_count[ALPHABET_SIZE];

  int startPos = ((int) myID) * (ARRAY_SIZE / NUM_THREADS);
  int endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

  printf("myID = %d startPos = %d endPos = %d \n", (int) myID, startPos, endPos);

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
  pthread_mutex_lock (&mutexsum);
  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     char_counts[i] += local_char_count[i];
  }
  pthread_mutex_unlock (&mutexsum);

  pthread_exit(NULL);
}

//H - obviously you want to change this so it actually prints the average. No need to do much here.
void print_results()
{
  int i,j, total = 0;

  					// then print out the totals
  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     total += char_counts[i];
     printf(" %c %d\n", (char) (i + 97), char_counts[i]);
  }
  printf("\nTotal characters:  %d\n", total);
}

main() {
	int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	char s[BIG_HEADER_SIZE];


	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	init_arrays();

// so insert outer loop here
   while ( !done ) {

					// H- now fill chunk
		 erase_chunk();
	     for ( i < chunksize ) {
	     	readline(s);
	     	if (s != header_line )
	     		copy(char_array[i], s);
	     if (feof(f)) {
	        done = TRUE;
	     	break;
		 }
		} // for loop


		     	// h- now do our calculations

		for (i = 0; i < NUM_THREADS; i++ ) {
			  rc = pthread_create(&threads[i], &attr, count_array, (void *)i);
			  if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
			  }
		}

		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
		for(i=0; i<NUM_THREADS; i++) {
			 rc = pthread_join(threads[i], &status);
			 if (rc) {
			   printf("ERROR; return code from pthread_join() is %d\n", rc);
			   exit(-1);
			 }
		}
	} //H- end of while(!done) loop

	print_results();

	pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
}

