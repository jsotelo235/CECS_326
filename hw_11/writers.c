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
     {2, -1, SEM_UNDO}, // lock count
     {3, -1, SEM_UNDO}
    };

 static struct sembuf Signal[4] 
  = {
     {0, 1, SEM_UNDO}, // unlock read
     {1, 1, SEM_UNDO}, // unlock write
     {2, 1, SEM_UNDO}, // unlock count
     {3, 1, SEM_UNDO}
    };

 struct sembuf OpList[4];

 int semID;			 // semaphore id
 key_t writeKey;	         // read key 
 union wait waitstatus;

 writeKey  = ftok("test", 'q');
 
 if((semID = semget(writeKey, 4, IPC_CREAT | IPC_EXCL | READ_WRITE)) != -1) {
  semctl(semID, 0, SETVAL, 1);
  semctl(semID, 1, SETVAL, 1);
  semctl(semID, 2, SETVAL, 1);
  semctl(semID, 3, SETVAL, 1);
 }
 else
 {
  semID = semget(writeKey,4, READ_WRITE);  
 }

 printf("Write semaphore id: %i\n\n", semID);

 int temp;

 temp = semctl(semID, 0, GETVAL, 1);

 printf("Value sema-0: %d \n", temp);

 temp = semctl(semID, 1, GETVAL, 1);

 printf("Value sema-1: %d \n", temp);

 temp = semctl(semID, 2, GETVAL, 1);

 printf("Value sema-2: %d \n", temp);

 temp = semctl(semID, 3, GETVAL, 1);

 printf("Value sema-3: %d \n", temp);
 for(int i = 0; i < 5; i++)
 { 
  OpList[0] = Wait[0];		// lock reader
  OpList[1] = Wait[1];		// lock writer
  semop(semID, OpList, 2);

  printf("Writing\n");
  sleep(4);
  printf("Done writing\n");  

  OpList[0] = Signal[1];	 // unlock writer
  OpList[1] = Signal[0];	 // unlock reader
  semop(semID, OpList, 2);

  printf("\n");
  sleep(8);
  fflush(stdout);
 }
 
 return 0;
}
