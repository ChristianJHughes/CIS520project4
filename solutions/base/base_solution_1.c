#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define WIKI_ARRAY_SIZE 50 /* Should be 1000000 */
#define WIKI_STRING_SIZE 2003
#define WORDS_ARRAY_SIZE 75 /* Should be 50000 */
#define WORDS_STRING_SIZE 10

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

    if(file == NULL) {
      printf("fail");
      return;
    }
    int line_num = 0;
    char * line = malloc(WIKI_STRING_SIZE);
    while(fgets(line, WIKI_STRING_SIZE, file) != NULL) {
        strcpy(wiki_array[line_num], line);
        line_num++;
    }
    fclose(file);
    free(line);
    printf("Load Memory: %s, %d\n", fileName, line_num);

    /* Read the words list to memory line by line. */
    //*fileName = "../../../../../scratch/dan/words_4-8chars_50k";
    fileName = "../../words_4-8chars75";
    file = fopen(fileName, "r"); /* should check the result */
    if(file == NULL) {
      printf("fail2");
      return;
    }

    line_num = 0;
    line = malloc(WORDS_STRING_SIZE);
    while (fgets(line, WORDS_STRING_SIZE, file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        strcpy(words_array[line_num], line);
	      // printf("%s\n", words_array[line_num]);
        line_num++;
    }
    fclose(file);
    free(line);

    printf("Load Memory: %s, %d\n", fileName, line_num);
}

void find_word_in_wiki()
{
  int i, j;
  int found_word = 0; // Assume that the words has not been found (false).

  for(i = 0; i < WORDS_ARRAY_SIZE; i++)
  {
    found_word = 0;
    for(j = 0; j < WIKI_ARRAY_SIZE; j++)
    {
      char *p = strstr(wiki_array[j], words_array[i]);
      if(p)
      {
        if (found_word == 0)
        {
          found_word = 1; // Set found_word to true.
          printf("%s: %d", words_array[i], j);
        }
        else
        {
          printf(", %d", j);
        }
      }
    }
    if (found_word) printf("\n");
  }
}

int main() {

	gettimeofday(&t1, NULL);

	//DO PROCESSING HERE
	read_to_memory();
	find_word_in_wiki();

	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("NSLOTS"),  elapsedTime);
}
