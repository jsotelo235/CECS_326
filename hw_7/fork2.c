#include <stdlib.h>	/* Needed for fork */
#include <stdio.h>	/* Needed for printf. */
#include <stdbool.h>	/* Needed for bool, optional. */
#include <unistd.h>	/* Needed for fork and exit. */
#include <sys/types.h>	/* Needed for wait function*/
#include <sys/wait.h>	/* Needed for wait function*/

int sum;

int main (int argc, char* argv[]) {
  int i;
  int input_char = 0;
  int count = 0;
  pid_t pid;
  
  sum = 0;
  printf("Sleeping\n");
  sleep(10);
  
  pid = fork();  /* create a new process */

  // fork fails then returns -1
  // fork returns 0 if its a child
  // Positive val returns the parent
  
  for(int i = 0; i < 10; i++)
  {
   sum += i;
   switch(pid)
   {
    case -1:
      printf("Creation of a child process was unsuccessful\n");
      return -1;
     break;
    case 0:
      input_char = getchar();
      printf("C sum is: %2i %2c\n", sum, input_char);
      sleep(2);
      fflush(stdout);
     break;
    default:
      input_char = getchar();
      printf("P sum is: %2i %2c\n", sum, input_char);
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
