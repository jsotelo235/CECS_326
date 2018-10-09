/*
   The parser piece for the scheduler assignment
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
/* You will need the integer queue */
#include "IQueue.c"

int main(int argc, char* argv[])
{
  int fd;
  FILE *f;
  char line[80];
  int start_time, duration;
  int i;
  int elapsed_time = 0;
  int sum = 0;
  int pid = 0;

  struct process_struct
  {
    int process_id;
    int start_time;
    int duration;
    int time_left;
  };

  struct process_struct process_list[80];
  int process_count;
 
  if (argc !=2)
  {
    printf("usage a.out file_name\n");
    return 0;
  };
  f = fopen(argv[1],"r");
  process_count = 0;

  while(fgets(line,sizeof(line),f) != NULL)
  {
    sscanf(line, "%d %d", &start_time, &duration);
    process_list[process_count].process_id = process_count;
    process_list[process_count].start_time = start_time;
    process_list[process_count].duration = duration;
    process_list[process_count].time_left = duration;
    process_count++ ;
  }



 for (i = 0 ; i < process_count ; i++)
 {
  sum += process_list[i].duration;
 }

 printf("Time	    Action\n");

 
 while(elapsed_time < sum)  
 {
  for(int j =0; j < process_count; j++)
  {
   if(elapsed_time == process_list[j].start_time)
   {
     IQueueStore(process_list[j].process_id);
   }
  }

  if(IQueueSize() == 0)
  {
   printf("%i	    idle\n", elapsed_time);
   sum += 5;
  }
  else
  {
   pid = IQueueRetrieve();
   process_list[pid].time_left -= 5;
   printf("%i	    Process %i\n", elapsed_time, pid);
   if(process_list[pid].time_left > 0)
   {
    IQueueStore(pid);
   }
  }
  
  elapsed_time += 5;
 }
 
 printf("DONE\n");
 close(fd);
};
