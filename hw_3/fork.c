#include <stdlib.h>	/* Needed for fork */
#include <stdio.h>	/* Needed for printf. */
#include <stdbool.h>	/* Needed for bool, optional. */
#include <unistd.h>	/* Needed for fork and exit. */
#include <sys/types.h>	/* Needed for wait function*/
#include <sys/wait.h>	/* Needed for wait function*/

int sum;
int main (int argc, char* argv[]) {
  int i;
  pid_t pid;
  
  pid = fork();  /* create a new process */
  sum = 0;

  // fork fails then returns -1
  // fork returns 0 if its a child
  // Positive val returns the parent

  for(int i = 0; i < 10; i++)
  {
   switch(pid)
   {
    case -1:
      printf("Creation of a child process was unsuccessful\n");
      return -1;
     break;
    case 0:
      sum += i;
      printf("C sum is: %i\n",sum);
      sleep(2);
      fflush(stdout);
     break;
    default:
      sum += i;
      printf("P sum is: %i\n", sum);
      sleep(1);
      fflush(stdout);
   }
  }

  if(argc > 1)
  {
   wait(0);
  }
  else
  {
    return 0;
  }

  return 0;
};
