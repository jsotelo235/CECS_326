/* You need to switch from 2 to 5 semaphores.
   Initialize the 5 semaphores to unlocked.
   You still only need two semaphores in the OpList.
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

int main () {
  struct sembuf Wait[5] 
   = { 
	{0,-1,SEM_UNDO}, 
	{1,-1,SEM_UNDO},
        {2,-1,SEM_UNDO},
        {3,-1,SEM_UNDO},
        {4,-1,SEM_UNDO} 
     };

  struct sembuf Signal[5]
   = { 
	{0,1,SEM_UNDO}, 
	{1,1,SEM_UNDO},
        {2,1,SEM_UNDO},
        {3,1,SEM_UNDO},
        {4,1,SEM_UNDO}
     };
 
  int ch, i, pid;
  key_t mykey;
  int semid;
  union wait waitstatus;
  struct sembuf OpList[5];

  mykey = ftok("philosophers",'a');
  semid = semget(mykey, 5, IPC_CREAT | 0600 ); // returns id

  semctl(semid, 0, SETVAL, 1);
  semctl(semid, 1, SETVAL, 1);
  semctl(semid, 2, SETVAL, 1);
  semctl(semid, 3, SETVAL, 1);
  semctl(semid, 4, SETVAL, 1);

  for (i = 0 ; i < 5 ; i++ )
  {
    pid = fork();
    if (pid == 0) break;
  }
  if (pid == 0)
  {
    pid = getpid();
    /* do the philosopher thing */
    
    for(int i = 0; i < 5; i++)
    { 
      OpList[i] = Wait[i];
      semop(semid, OpList, 1);

      OpList[i] = Wait[(i+1) % 5];
      semop(semid, OpList, 1);
     
      printf("Philosopher %d eating\n", pid);
      sleep(1);

      OpList[i] = Signal[i];
      semop(semid, OpList, 1);
      
      OpList[i] = Signal[(i+1) % 5];
      semop(semid, OpList, 1);
           
      printf("Philosopher %d thinking\n", pid);
      printf("\n");
      sleep(4);
    }   
  
    return 0;
  }
  else
  {
    for (i = 0 ; i < 5 ; i++){
      wait3(&ch,0,NULL);
    }
    /* remove the semaphore */
    semctl(semid, 0, IPC_RMID, 0);
    semctl(semid, 1, IPC_RMID, 0);
    semctl(semid, 2, IPC_RMID, 0); 
    semctl(semid, 3, IPC_RMID, 0);
    semctl(semid, 4, IPC_RMID, 0);
    return 0;
  }
}
