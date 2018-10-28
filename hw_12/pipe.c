#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() 
{
  int toParent[2], 
      toChild[2];

  char eval;

  int multiplicand = 0,
      multiplier   = 0,
      result = 0;

  pipe(toParent); 
  pipe(toChild);

  if (fork()==0)
  {
   while(1)
   {
     close(toParent[0]); // closing the read in the parent
     close(toChild[1]);	 // closing the write in the child
       
     read(toChild[0], &multiplicand, 20);
     eval = read(toChild[0], &multiplier, 20);
    
     if(eval == 0) break;
     else
     {
      result = (multiplicand * multiplier);
      write(toParent[1], &result, 20);
     }
   }
  }
  else 
  {	
   close(toChild[0]);  // close the read in the child	
   close(toParent[1]); // close the write in the parent

   for(int i = 0; i < 5; i++)
   {
    printf("Enter multiplicand #%d: ", i);
    scanf("%d", &multiplicand);
    write(toChild[1], &multiplicand, 20);

    printf("Enter multiplier   #%d: ", i);
    scanf("%d", &multiplier);
    write(toChild[1], &multiplier, 20);
    
    read(toParent[0], &result, 20); // read the result from the write coming from child
    printf("Result: %d\n", result);
    printf("\n");
   } 
   
  }

  return 0;
}

