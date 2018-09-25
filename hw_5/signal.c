#include <signal.h>	// software interrupti
#include <stdio.h>
#include <unistd.h>

void catch (int sig)
{
  printf("Got signal %d\n",sig);
}

void catchRingAlarm(int sig)
{
  printf("RING RING %d\n", sig);
}

int main ()
{
  int i;
  // first arg is an integer, signal
  // secong arg is a pointer function handler
  signal(SIGINT, catch);			// interrupted when user uses Ctrl-C (SIGINT = 2)
  signal(SIGALRM, catchRingAlarm);		// SIGALRM indicates the expiration of timers, returns func handler
  alarm(6);

  for (i=0;i<20;i++) {
    printf("%d\n",i);
    sleep(1);
  }
  return 0;
}

/* control c prints Got signal 2 */
/* after 6 seconds the program stops (Alarm clock) */
