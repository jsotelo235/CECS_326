#include <stdlib.h>	/* needed for fork*/
#include <stdio.h>	/* needed for printf*/
#include <stdbool.h>    /* Needed for bool, optional. */
#include <unistd.h>     /* Needed for fork and exit. */
#include <sys/types.h>  /* Needed for wait function*/
#include <sys/wait.h>   /* Needed for wait function*/
#include <sys/types.h>  /* Needed for semaphore*/
#include <sys/ipc.h>    /* Needed for semaphore*/
#include <sys/sem.h>	/* Needed for semaphore*/
#include <sys/shm.h> 	/* Needed for shared memory*/

#define READ_WRITE 0600
#define NUMBYTES 3

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
 int shm_id;			 // shared memory id
 char *shm;			 // shared memory addr returned by shmat
 int *addr;			 // used to access addresses
 key_t readKey;			 // read key 
 union wait waitstatus;

 readKey  = ftok("test", 'q');

 // shmget retunrs an identifier to var myShare
 shm_id  = shmget(readKey, NUMBYTES, IPC_CREAT | READ_WRITE);
 
 if(shm_id < 0)
 {
   printf("shmget error\n");
   exit(1);
 }
 
 shm = shmat(shm_id, 0, 0);

 if(shm == (char*)-1)
 {
   printf("shmat error\n");
   exit(1);
 }

 
 if((semID = semget(readKey, 4, IPC_CREAT | IPC_EXCL | READ_WRITE)) != -1) {
  semctl(semID, 0, SETVAL, 1);
  semctl(semID, 1, SETVAL, 1);
  semctl(semID, 2, SETVAL, 1);
  semctl(semID, 3, SETVAL, 1);
 }
 else
 {
  semID = semget(readKey, 4, READ_WRITE);  
 }
 
 printf("Reads semaphore id: %i\n\n", semID);
 
 addr = (int*)shm; 	   // cast shm to an int

 for(int i = 0; i < 5; i++)
 {
  OpList[0] = Wait[0];	   // lock reader
  OpList[1] = Wait[2];	   // lock counter 
  semop(semID, OpList, 2); // perform the operations

  if(semctl(semID, 2, GETVAL, 1) == 0)
  {
   OpList[0] = Wait[1];		// lock writer
   semop(semID, OpList, 1);	// perform the operation
  }

  OpList[0] = Signal[2];        // counter
  semop(semID, OpList, 1);	// increment counter

  OpList[0] = Signal[2];	// unlock counter
  OpList[1] = Signal[0];	// unlock reader
  semop(semID, OpList, 2);	// perform the operation

  // Critical Section
  int firstByte  = addr[0]; 	// read the first byte of shared memory
  sleep(1);
  int secondByte = addr[1];	// read the second byte of shared memory
  sleep(1);
  int thirdByte  = addr[2];	// read the third byte of shared memory
  printf("Byte 1:%3d  Byte 2:%3d  Byte 3:%3d\n", firstByte, secondByte, thirdByte); 
  sleep(2);
  printf("\n"); 

  OpList[0] = Wait[2];		// lock counter
  OpList[1] = Wait[2];		// get counter ready to decrement
  semop(semID, OpList, 2);      // perform lock and decrement

  if(semctl(semID, 2, GETVAL, 1) == 0)
  {
   OpList[0] = Signal[1];	// unlock writer
   semop(semID, OpList, 1);	// perform the operation
  }

  OpList[0] = Signal[2];	// unlock counter
  semop(semID, OpList, 1);	// perform the operation

  sleep(4);
  fflush(stdout);
 }
 return 0;
}
