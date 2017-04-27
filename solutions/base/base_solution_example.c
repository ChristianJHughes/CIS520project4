#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define WIKI_ARRAY_SIZE 50 /* The number of wiki entries. Should be 1000000 for production. */
#define WIKI_STRING_SIZE 2003 /* The number of characters in each wiki entry. Must account for newline and terminating characters. */
#define WORDS_ARRAY_SIZE 75 /* The number of words being searched for in each wiki entry. Should be 50000 for production. */
#define WORDS_STRING_SIZE 11 /* The number of characters taken up by a word. Must account for newline and terminating characters. */

/* All of the wiki entries. */
char wiki_array[WIKI_ARRAY_SIZE][WIKI_STRING_SIZE];

/* All of the words being searched for. */
char words_array[WORDS_ARRAY_SIZE][WORDS_STRING_SIZE];

/* Results of the word search*/
char results_array[WORDS_ARRAY_SIZE][WIKI_ARRAY_SIZE];

/* Number of words to print */
int count = 0;

/* Read all of the wiki entries and words into local data structures from thier resprctive files. */
int read_to_memory()
{
  /* Read the wiki article into memory line by line. */
  //FILE *file = fopen("/scratch/dan/wiki.1Mshort", "r");
  FILE *file = fopen("/homes/cjhughes255/project4/wiki.50short", "r");

  if(file == NULL) {
    printf("fail");
    return -1;
  }

  /* Read each wiki line into memory. */
  int line_num = 0;
  char line[WIKI_STRING_SIZE];
  while(fgets(line, WIKI_STRING_SIZE, file) != NULL) {
      strcpy(wiki_array[line_num], line);
      line_num++;
  }
  fclose(file);

  /* Read the words list to memory line by line. */
  //file = fopen("/scratch/dan/words_4-8chars_50k", "r");
  file = fopen("/homes/cjhughes255/project4/words_4-8chars75", "r");

  if(file == NULL) {
    printf("fail2");
    return -1;
  }

  /* Read each word line into memory. */
  line_num = 0;
  char line2[WORDS_STRING_SIZE];
  while (fgets(line2, WORDS_STRING_SIZE, file) != NULL) {
      line2[strcspn(line2, "\n")] = 0;
      strcpy(words_array[line_num], line2);
      line_num++;
  }
  fclose(file);
}

/* If a given word is present in 1 or more wiki articles, print out the word with the lines number(s) of the assocaited articles. */
void find_word_in_wiki()
{
  int i, j, found_word;
  char string[10000];
  char temp[10];
  
  for(i = 0; i < WORDS_ARRAY_SIZE; i++) {
    found_word = 0;
    for(j = 0; j < WIKI_ARRAY_SIZE; j++) {
      char *p = strstr(wiki_array[j], words_array[i]);
      if(p)
      {
	if (found_word == 0) {
	  found_word = 1;
	  sprintf(string, "%s: %d", words_array[i], j+1);
	}
	else {
	  sprintf(temp, ", %d", j+1);
	  strncat(string, temp, 10);
	}
      }
    }
    if(found_word == 1)
      {
	strcpy(results_array[count], string);
	count++;
      }
  }
}

void print_results()
{
  int i, j, found_word;
  
  for(i = 0; i < count; i++)
    {
      printf("%s\n", results_array[i]);
    }
}

int main() {
  /* For measuring performance. */
  struct timeval t1, t2, t3, t4;
  double elapsedTime;
  int numSlots, myVersion = 1;

  gettimeofday(&t1, NULL);

  // Read file into memory and print out all of the found words.
  if (read_to_memory() == 0) {
    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
    printf("Time to read: %f\n", elapsedTime);
    find_word_in_wiki();

    gettimeofday(&t3, NULL);
    elapsedTime = (t3.tv_sec - t2.tv_sec) * 1000.0; //sec to ms
    elapsedTime += (t3.tv_usec - t2.tv_usec) / 1000.0; // us to ms
    printf("Time to search: %f\n", elapsedTime);

    print_results();
  }
  else {
    return;
  }
  gettimeofday(&t4, NULL);

  elapsedTime = (t4.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
  elapsedTime += (t4.tv_usec - t1.tv_usec) / 1000.0; // us to ms
  printf("DATA, %d, %s, %f\n", myVersion, getenv("NSLOTS"),  elapsedTime);
}
