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
#define NEWLINE "\n"
#define ANSI_COLOR_BOLD_RED 	"\033[1;31m"
#define ANSI_COLOR_RESET   	"\033[0m"

int main(int argc, char* argv[])
{
 int semid; // semaphore id
 key_t key; // key to pass to semget
 pid_t pid;

 key = ftok("lightWeight", 'l');
 
 if((semid = semget(key, 1, IPC_CREAT | IPC_EXCL | READ_WRITE)) != -1)
 {
  // creator only
  semctl(semid, 0, SETVAL, 1);
 }
 else
 {
  // non creators failed
  semid = semget(key,1, READ_WRITE);
 }
 
 printf(ANSI_COLOR_BOLD_RED);
 printf("Semaphore ID: %i\n", semid);
 printf(ANSI_COLOR_RESET);
 printf(NEWLINE);
 
 static struct sembuf Wait   = {0, -1, SEM_UNDO};
 static struct sembuf Signal = {0,  1, SEM_UNDO};
 
 for(int i = 0; i < 5; i++)
 {
   semop(semid, &Wait, 1);   		// wait
   printf("LightWeight Starting\n");
   sleep(4);
   printf("LightWeight Ending\n");
   semop(semid, &Signal, 1);		// signal
   sleep(8);
   printf("\n");
   fflush(stdout);
 }
 return 0;
}
