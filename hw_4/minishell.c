/*
   The parser piece for the minishell assignment
   Replace the part that prints the parsed command
   with you code that does the fork, exec, wait.
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>


int main()
{  
  char line[40];
  char *nargs[10];
  char *space = " ";	// delimiter
  char *where;		// token
  pid_t pid;
  pid = fork();

  while(1)
  {
    int i = 0;
    printf("\nYour command please: ");
    fgets(line,sizeof(line),stdin);	// reads a line from the specified stream and stores it in a pointer to an array
    line[strlen(line)-1] = '\0';	// line[39] is set to a terminating null
    where = strtok(line,space);		// splits line[40] by looking for the space delimiter
    nargs[i++] = where;

    pid = fork();
    
    // Keep printing tokens while one of the
    // delimiters present in line[]
    while (where != NULL)
    {
      where = strtok(NULL, space);
      nargs[i++] = where;		// save the string where position into nargs
     
      switch(pid)
      {
        case -1:
         printf("An error occurred. Can not create a child process\n");
         break;
        case 0:
         printf("Child Process\n\n");
         if( execvp(*nargs, nargs) < 0)
         {
          printf("ERROR: exec failed\n");
          exit(1);
         }
         break;
        default:
         printf("Parent Process\n");
         wait3(NULL,0,NULL);
         break;
      }
    }
  }

};

