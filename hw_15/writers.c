#include <stdlib.h>	/* needed for fork*/
#include <stdio.h>	/* needed for printf*/
#include <stdbool.h>    /* Needed for bool, optional. */
#include <unistd.h>     /* Needed for fork and exit. */
#include <sys/types.h>  /* Needed for wait function*/
#include <sys/wait.h>   /* Needed for wait function*/
#include <sys/types.h>  /* Needed for semaphore*/
#include <sys/ipc.h>    /* Needed for semaphore*/
#include <sys/sem.h>	/* Needed for semaphore*/
#include <sys/shm.h>	/* Needed for share memory*/

#define READ_WRITE 0600
#define NUMBYTES 3

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
 int shm_id;			 // shared memory id
 char *shm;			 // shared memory addr returned by shmat 
 int  *addr;			 // used to acess addresses
 key_t writeKey;	         // read key 
 union wait waitstatus;

 writeKey  = ftok("test", 'q');
 
 // shmget retunrs an identifier to var myShare
 shm_id   = shmget(writeKey, NUMBYTES, IPC_CREAT | READ_WRITE);

 if(shm_id < 0)
 {
   printf("shmget error\n");
   exit(1);
 }

 // shmat is used to attach to shared memory
 shm = shmat(shm_id, 0, 0);

 if(shm == (char*)-1)
 {
   printf("shmat error\n");
   exit(1);
 }

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

 printf("Write semaphore id: %d\n",  semID);
 printf("Shared memory id  : %d\n\n",shm_id);
 
 // initialize the 3 bytes of shared memory to 1's
 addr = (int *)shm;
 addr[0] = 1;
 addr[1] = 1;
 addr[2] = 1;

 printf("Shared memory 1: %d\n", addr[0]);
 printf("Shared memory 2: %d\n", addr[1]);
 printf("Shared memory 3: %d\n", addr[2]);
 printf("\n");

 for(int i = 0; i < 5; i++)
 { 
  OpList[0] = Wait[0];		// lock reader
  OpList[1] = Wait[1];		// lock writer
  semop(semID, OpList, 2);

  // Critical Section
  addr[0]++; 
  printf("Increment first byte of shared memory :%3d\n", addr[0]);
  sleep(1);

  addr[1]++;
  printf("Increment second byte of shared memory:%3d\n", addr[1]);
  sleep(1);

  addr[2]++;
  printf("Increment third byte of shared memory :%3d\n", addr[2]);
  sleep(3);

  OpList[0] = Signal[1];	 // unlock writer
  OpList[1] = Signal[0];	 // unlock reader
  semop(semID, OpList, 2);

  printf("\n");
  sleep(8);
  fflush(stdout);
 }
 
 return 0;
}
