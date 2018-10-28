#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main () {
  int result;
  int fd_1,
      fd_2;

  int multiplicand = 0,
      multiplier   = 0,
      product      = 0;

  mkfifo("fifoServer.pipe",0600);
  mkfifo("fifo.pipe", 0600);

  for(int i = 0; i < 5; i++)
  {
   fd_1   = open("fifo.pipe",O_RDONLY);
   fd_2   = open("fifoServer.pipe", O_WRONLY);

   read(fd_1, &multiplicand, sizeof(multiplicand));
   read(fd_1, &multiplier,   sizeof(multiplier));

   product = multiplicand * multiplier;

   write(fd_2, &product, sizeof(product));
  }
   
  return 0;
};
