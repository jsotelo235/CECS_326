#include <stdlib.h>	/* needed for fork*/
#include <stdio.h>	/* needed for printf*/
#include <stdbool.h>    /* Needed for bool, optional. */
#include <unistd.h>     /* Needed for fork and exit. */
#include <sys/types.h>  /* Needed for wait function*/
#include <sys/wait.h>   /* Needed for wait function*/
#include <sys/types.h>  /* Needed for semaphore*/
#include <sys/ipc.h>    /* Needed for semaphore*/
#include <sys/sem.h>	/* Needed for semaphore*/

#define READ_WRITE 0600

int main(int argc, char* argv[])
{
  static struct sembuf Wait[4]  
  = {
     {0, -1, SEM_UNDO}, // lock read
     {1, -1, SEM_UNDO}, // lock write
     {2, -1, SEM_UNDO}, // lock counter
     {3, -1, SEM_UNDO}
    };

 static struct sembuf Signal[4] 
  = {
     {0, 1, SEM_UNDO}, // unlock read
     {1, 1, SEM_UNDO}, // unlock write
     {2, 1, SEM_UNDO}, // unlock counter
     {3, 1, SEM_UNDO}
    };

 struct sembuf OpList[4];

 int semID;			 // semaphore id
 key_t readKey;			 // read key 
 union wait waitstatus;
 int counter = 0;

 readKey  = ftok("Readers", 'a');

 semID = semget(readKey,  4, IPC_CREAT | READ_WRITE); 

 printf("Reads semaphore id: %i\n", semID);
 
 semctl(semID, 0, SETVAL, 1);
 semctl(semID, 2, SETVAL, 1);

 for(int i = 0; i < 5; i++)
 {
  OpList[0] = Wait[0];	  // lock reader
  OpList[1] = Wait[2];	  // lock counter 
  semop(semID, OpList, 2); // perform the operations

  if(counter == 0)
  {
   OpList[0] = Wait[1];		// lock writer
   semop(semID, OpList, 1);	// perform the operation
  }

  counter++;			// increment count

  OpList[0] = Signal[2];	// unlock counter
  OpList[1] = Signal[0];	// unlock reader
  semop(semID, OpList, 2);	// perform the operation

  printf("Reading\n");
  sleep(2);
  printf("Done reading\n");
  sleep(4);
  printf("\n");
 
  OpList[0] = Wait[2];		// lock counter
  semop(semID, OpList, 1);	// perform the operation
  
  counter--;			// decrement counter

  if(counter == 0)
  {
   OpList[0] = Signal[1];	// unlock writer
   semop(semID, OpList, 1);	// perform the operation
  }

  OpList[0] = Signal[2];	// unlock counter
  semop(semID, OpList, 1);	// perform the operation
  fflush(stdout);
 }
 return 0;
}
