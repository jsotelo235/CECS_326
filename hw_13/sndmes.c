#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main () 
{
  int myqueue;
  key_t mykey;
  int status;
  int multiplicand; 
  int multiplier;
  int product;  

  struct Mybuf 
  {
    long mtype;
    int message;
  };


  struct Mybuf buf;

  mykey = ftok( "test" , 'a' );

  myqueue = msgget(mykey, IPC_CREAT | 0600 );

  if ( -1 == myqueue) {
    printf("error in msgget");
    return 1;
  }

  /* no special instructions */
 // buf.mtype = 1;
 
  for(int i = 0; i < 5; i++)
  {
   buf.mtype = 1;
   printf("Enter the multiplicand: ");
   scanf("%d", &multiplicand);
	
   printf("Enter the multiplier  : ");
   scanf("%d", &multiplier);

   buf.message = multiplicand;   
   status = msgsnd(myqueue, (struct msgbuf *)&buf, 10, 0);  

   if( -1 == status)
   {
    printf("error in msgsnd %s \n", strerror(errno) );
   }
 
   buf.message = multiplier; 
   status = msgsnd(myqueue, (struct msgbuf *)&buf, 10, 0);

   if ( -1 == status) 
   {
    printf("error in msgsnd %s \n", strerror(errno) );
   }
  
   buf.mtype = 69;
   msgrcv(myqueue, (struct msgbuf *)&buf, 10, 69 , 0);
   product = buf.message;

   printf("Product is: %d\n", product);
   printf("\n");
  }
}
