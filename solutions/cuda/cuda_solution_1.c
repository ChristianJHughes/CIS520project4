#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define WIKI_ARRAY_SIZE 1000000
#define WIKI_STRING_SIZE 2000
#define WORDS_ARRAY_SIZE 50000
#define WORDS_STRING_SIZE 9

char wiki_array[WIKI_ARRAY_SIZE][WIKI_STRING_SIZE];
char words_array[WORDS_ARRAY_SIZE][WORDS_STRING_SIZE];

struct timeval t1, t2;
double elapsedTime;
int numSlots, line_num, myVersion = 1;

void read_to_memory()
{
	/* Read the Wiki article to memory line by line. */
	//char *fileName = "../../../../../scratch/dan/wiki.1Mshort";
	char *fileName = "../../wiki.50short";
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line_wiki[WIKI_STRING_SIZE];
    line_num = 0;
    while (fgets(line_wiki, sizeof(line_wiki), file)) {
    	//printf("%d\n", line_num);
        strcpy(wiki_array[line_num], line_wiki);
        line_num++;
    }
    fclose(file);

    printf("Load Memory: %s, %d\n", fileName, line_num);

    /* Read the words list to memory line by line. */
    //*fileName = "../../../../../scratch/dan/words_4-8chars_50k";
    fileName = "../../words_4-8chars75";
    file = fopen(fileName, "r"); /* should check the result */
    char line_words[WORDS_STRING_SIZE];
    line_num = 0;
    while (fgets(line_words, sizeof(line_words), file)) {
    	//printf("%d\n", line_num);
        strcpy(words_array[line_num], line_words);
        line_num++;
    }
    fclose(file);

    for(int i = 0; i < WORDS_ARRAY_SIZE; i++){
    	printf("%s\n", words_array[i]);
    }

    printf("Load Memory: %s, %d\n", fileName, line_num);
}

void find_word_in_wiki()
{
	// for(int i = 0; i < WIKI_ARRAY_SIZE; i++){
	// 	for(int j = 0; j < WORDS_ARRAY_SIZE; j++){
	// 		if(strstr(wiki_array[i], words_array[j]) != NULL) {
	// 			printf("%s, %d\n", words_array[j], i);
	// 		}
	// 	}
	// }
}


// char getRandomChar()
// {
// 	int randNum = 0;
// 	char randChar = ' ';

// 	randNum = 26 * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
// 	randNum = randNum + 97;				// scale to 'a'
// 	randChar = (char) randNum;

// 	// printf("%c", randChar);
// 	return randChar;
// }

// void init_arrays()
// {
//   int i, j, randNum; 
//   char randChar;

//   for ( i = 0; i < ARRAY_SIZE; i++) {
// 	for ( j = 0; j < STRING_SIZE; j++ ) {
// 		 char_array[i][j] = getRandomChar();
// 	}
//   }

//   for ( i = 0; i < 26; i++ ) {
//   	char_counts[i] = 0;
//   }
// }

// void count_array()
// {
//   char theChar;
//   int i, j, charLoc;

//   for ( i = 0; i < ARRAY_SIZE; i++) {
// 	for ( j = 0; j < STRING_SIZE; j++ ) {
// 	         theChar = char_array[i][j];
// 		 charLoc = ((int) theChar) - 97;
// 		 char_counts[charLoc]++;
// 	}
//   }
// }

// void print_results()
// {
//   int i, total = 0;

//   for ( i = 0; i < 26; i++ ) {
//      total += char_counts[i];
//      printf(" %c %d\n", (char) (i + 97), char_counts[i]);
//   }
//   printf("\nTotal characters:  %d\n", total);
// }

int main() {


	gettimeofday(&t1, NULL);

	
	printf("DEBUG: starting loop on %s\n", getenv("HOST"));

	//DO PROCESSING HERE
	//init_arrays();
	read_to_memory();
	find_word_in_wiki();
	//count_array();
	//print_results();

	gettimeofday(&t2, NULL);


	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("NSLOTS"),  elapsedTime);




	
}











