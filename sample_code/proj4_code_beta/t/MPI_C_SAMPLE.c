/*
 * Open Systems Lab
 * http://www.lam-mpi.org/tutorials/
 * Indiana University
 *
 * MPI Tutorial
 * The cannonical ring program
 *
 * Mail questions regarding tutorial material to mpi@lam-mpi.org
 */

#include <stdio.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
  MPI_Status status;
  int num, rank, size, tag, next, from;

  /* Start up MPI */

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
 
  /* Arbitrarily choose 201 to be our tag.  Calculate the */
  /* rank of the next process in the ring.  Use the modulus */
  /* operator so that the last process "wraps around" to rank */
  /* zero. */

  tag = 201;
  next = (rank + 1) % size;
  from = (rank + size - 1) % size;

  /* If we are the "console" process, get a integer from the */
  /* user to specify how many times we want to go around the */
  /* ring */

  if (rank == 0) {
    printf("Enter the number of times around the ring: ");
    scanf("%d", &num);
    --num;

    printf("Process %d sending %d to %d\n", rank, num, next);
    MPI_Send(&num, 1, MPI_INT, next, tag, MPI_COMM_WORLD); 
  }

  /* Pass the message around the ring.  The exit mechanism works */
  /* as follows: the message (a positive integer) is passed */
  /* around the ring.  Each time is passes rank 0, it is decremented. */
  /* When each processes receives the 0 message, it passes it on */
  /* to the next process and then quits.  By passing the 0 first, */
  /* every process gets the 0 message and can quit normally. */

  while (1) {

    MPI_Recv(&num, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
    printf("Process %d received %d\n", rank, num);

    if (rank == 0) {
      num--;
      printf("Process 0 decremented num\n");
    }

    printf("Process %d sending %d to %d\n", rank, num, next);
    MPI_Send(&num, 1, MPI_INT, next, tag, MPI_COMM_WORLD);

    if (num == 0) {
      printf("Process %d exiting\n", rank);
      break;
    }
  }

  /* The last process does one extra send to process 0, which needs */
  /* to be received before the program can exit */

  if (rank == 0)
    MPI_Recv(&num, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);

  /* Quit */

  MPI_Finalize();
  return 0;
}
