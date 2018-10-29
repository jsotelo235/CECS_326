#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

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

 // buf.mtype = 1;

  while(1)
 {
  buf.mtype = 1;
  /* no special instructions, no flags */
  status = msgrcv(myqueue, (struct msgbuf *)&buf, 10, 0, 0);
  multiplicand = buf.message;

  if ( -1 == status) {
    printf("error in msgrcv");
    return 1;
  }
   
  status = msgrcv(myqueue, (struct msgbuf *)&buf, 10, 0, 0);
  multiplier = buf.message;

  product = multiplicand * multiplier;
  
  buf.mtype = 2;
  buf.message = product;
 
  msgsnd(myqueue, (struct msgbuf *)&buf, 10, 0);

  if ( -1 == status) 
  {
    printf("error in msgrcv");
    return 1;
  }
 } 
}
