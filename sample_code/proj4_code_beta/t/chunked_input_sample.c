    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <mpi.h>


    #define MAX_LINE_SIZE 500000
    #define MAX_NUM_GENOMES 800
    #define MAX_GENOME_LENGTH 200000


    int NUM_THREADS;
    char curGenomes[MAX_NUM_GENOMES][MAX_GENOME_LENGTH];
    int curNumGenomes = 0;


    unsigned long totalSum = 0;
    unsigned long totalChars = 0;


    unsigned long mySum = 0;
    unsigned long myChars = 0;


    int loadGenomes(FILE* fp)
    {
    char buff[MAX_LINE_SIZE];
    strcpy(buff,"");
    curNumGenomes = 0;
    char tempGenome[MAX_GENOME_LENGTH];
    strcpy(tempGenome,"");
    while (fgets(buff, MAX_LINE_SIZE, fp) != NULL)
    {
    if (buff[0] == '>')
    {
    strcpy(curGenomes[curNumGenomes++],tempGenome);
    strcpy(tempGenome,"");
    if (curNumGenomes == MAX_NUM_GENOMES)
    return 0;
    }
    else if (buff[0] >= 65 && buff[0] <= 90)
    strcat(tempGenome,buff);
    strcpy(buff,"");
    }
    if (strlen(tempGenome) > 0 && curNumGenomes < MAX_NUM_GENOMES - 1)
    {
    strcpy(curGenomes[curNumGenomes++],tempGenome);
    strcpy(tempGenome,"");
    }
    printf("Reached end of file.\n");
    return 1;
    }


    void processChunk(int rank)
    {
    int i,j;
    int startPos = rank * (curNumGenomes / NUM_THREADS);
    int endPos = startPos + (curNumGenomes / NUM_THREADS);
    for (i = startPos; i < endPos; i++)
    {
    for (j = 0; j < strlen(curGenomes[i]); j++)
    {
    if (curGenomes[i][j] >= 65 && curGenomes[i][j] <= 90)
    {
    myChars++;
    mySum += (int) curGenomes[i][j];
    }
    }
    }
    curNumGenomes = 0;
    }


    int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    int rc, numtasks, rank;
    MPI_Status Status;
    if (fp == NULL)
    {
    printf("File could not be opened.\n");
    return 0;
    }
    rc = MPI_Init(&argc,&argv);
    if (rc != MPI_SUCCESS)
    {
    printf("Error starting MPI. Abort.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
    }


    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    NUM_THREADS = numtasks;
    printf("Numtasks = %d rank = %d\n", numtasks, rank);
    fflush(stdout);

    char dummy[MAX_LINE_SIZE];
    if (rank == 0)
    fgets(dummy, MAX_LINE_SIZE, fp);
    int done = 0;
    while (done != 1)
    {
    if (rank == 0)
    done = loadGenomes(fp);
    MPI_Bcast(&done, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(curGenomes, MAX_NUM_GENOMES * MAX_GENOME_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&curNumGenomes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    processChunk(rank);
    MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Reduce(&myChars, &totalChars, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mySum, &totalSum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
    processChunk(rank);
    printf("Number of characters counted: %lu\n",totalChars);
    printf("Sum of characters: %lu\n",totalSum);
    printf("Avg: %lf\n",(double)totalSum / (double)totalChars);
    printf("Average: %c\n",(char)((double)totalSum / (double)totalChars));
    printf("DONE!\n");
    }
    MPI_Finalize();
    return 0;
    }

