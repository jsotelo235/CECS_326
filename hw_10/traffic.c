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
 int semid; // semaphore id
 key_t key; // key to pass to semget
 pid_t pid;

 key = ftok(".traffic", 'x');
 semid = semget(key, 1, IPC_CREAT | READ_WRITE);

 printf("Semaphore ID: %i\n", semid);
 
 static struct sembuf Wait   = {0, -1, SEM_UNDO};
 static struct sembuf Signal = {0, 1, SEM_UNDO};
 semctl(semid, 0, SETVAL, 1);

 pid = fork();
 
 for(int i = 0; i < 10; i++)
 {
  switch(pid)
  {
   case -1:
    printf("Creation of a child process was unsuccessful\n");
    return -1;
    break;
   case 0:
    // Child handles traffic in the N/S direction
    semop(semid, &Wait, 1);   // wait
    printf("N/S car entering intersection\n");
    sleep(1);
    printf("N/S car leaving intersection\n");
    semop(semid, &Signal, 1); // signal
    printf("\n");
    break;
   default:
    // Parent handles traffic in the E/W direction
    semop(semid, &Wait, 1); // wait
    printf("E/W car entering intersection\n");
    sleep(1);
    printf("E/W car leaving intersection\n");
    semop(semid, &Signal, 1); // signal
    printf("\n");
    break;
  }
 }
 return 0;
}
