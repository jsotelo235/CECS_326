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
    
  for(int i = 0; i < 5; i++)
  {
   fd_1 = open("fifo.pipe",O_WRONLY);
   fd_2 = open("fifoServer.pipe", O_RDONLY);

   printf("Enter the multiplicant #%d : ", i);
   scanf("%d", &multiplicand);
    
   printf("Enter the multiplier   #%d : ", i);
   scanf("%d", &multiplier);

   write(fd_1, &multiplicand, sizeof(multiplicand));
   write(fd_1, &multiplier,   sizeof(multiplier));
   
   // read back the answer from the server
   read(fd_2, &product, sizeof(product));
   printf("Product: %d\n", product);    
   printf("\n"); 
  }
  return 0;
};
