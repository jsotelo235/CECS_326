#include <stdlib.h>	/* Needed for fork */
#include <stdio.h>	/* Needed for printf. */
#include <stdbool.h>	/* Needed for bool, optional. */
#include <unistd.h>	/* Needed for fork and exit. */
#include <sys/types.h>	/* Needed for wait function*/
#include <sys/wait.h>	/* Needed for wait function*/
#include <pthread.h>    /* Needed for making threds*/


/* Procedure function */
void *childThread(void *ptr);

/* Global Variables */
int sum;
int i;
int input_char = 0;
int th_identifier;
pthread_t th;		// thread
pthread_attr_t ta;	// thread attributes

int main (int argc, char* argv[]) 
{
  // thread needs to be told it's attributes
  // initialize the attributes using pthread_attr_t variable
  (void) pthread_attr_init(&ta);

  // to avoid issues with joining after the thread is done
  // modify the state attribute to "detached"
  (void) pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
  

  printf("Sleeping\n");
  sleep(5);

  // Create a thread to run a procedure
  th_identifier = pthread_create(&th, &ta, (void * (*)(void *))childThread, NULL);
  

  if(argc > 1)
  {
   wait(0);
  }
  else
  {
   return 0;
  }

 pthread_exit(NULL);
};



void *childThread(void *ptr)
{ 
 // int i;
  for(i = 0; i < 10; i++)
  {
    sum += i; 
    input_char = getchar();
    if(th_identifier) 
    {
      fprintf(stderr, "Error-pthread_create() return code: %d\n",th_identifier);
      exit(EXIT_FAILURE);
    }
   
    printf("pthread_create() sum is: %2i %2c\n", sum, input_char);
    sleep(2);
    fflush(stdout);
  }
 return NULL;
}
