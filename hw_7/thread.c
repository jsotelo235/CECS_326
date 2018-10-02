#include <stdlib.h>	/* Needed for fork */
#include <stdio.h>	/* Needed for printf. */
#include <stdbool.h>	/* Needed for bool, optional. */
#include <unistd.h>	/* Needed for fork and exit. */
#include <sys/types.h>	/* Needed for wait function*/
#include <sys/wait.h>	/* Needed for wait function*/
#include <pthread.h>    /* Needed for making threds*/

/* Global Variables */
int sum;
int input_char;
//int i;

/* Procedure Function*/
void *childThread(void *arg)
{ 
  int i;
  for(i = 0; i < 10; i++)
  {
    sum += i; 
    input_char = getchar();
    printf("2nd pthread_create() sum is: %2i %2c\n", sum, input_char);
    sleep(2);
    fflush(stdout);
  }
}

int main (int argc, char* argv[]) 
{
  int i;
  pthread_t th;        // thread
  pthread_attr_t ta;   // thread attributes

  // thread needs to be told it's attributes
  // initialize the attributes using pthread_attr_t variable
  (void) pthread_attr_init(&ta);

  // to avoid issues with joining after the thread is done
  // modify the state attribute to "detached"
  (void) pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
  

  printf("Sleeping\n");
  sleep(5);

  // Create a thread to run a procedure
  pthread_create(&th, &ta, (void * (*)(void *))childThread, NULL);
  
  for(i = 0; i < 10; i++)
  {
    sum += i; 
    input_char = getchar();
    printf("1st pthread_create() sum is: %2i %2c\n", sum, input_char);
    sleep(2);
    fflush(stdout);
  }

  if(argc > 1)
  {
   wait(0);
  }
};




