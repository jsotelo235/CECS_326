#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argc, char* argv[]) 
{
  int     src_fd;	/*source file discriptor*/
  int     dest_fd;      /*destination file discriptor*/
  char    filebuf[256]; /*a small buffer used to copy the file (1-byte)*/
  ssize_t readSize;	/*signed size_t*/
  
  // argc specifies the count of total command line arguments
  // argv is a pointer to an array of strings
  // argv[0] will be the name of the program
  // argv[1] will be txt file
  // xfer.tmp will be the name of the destination file

  printf("The real user ID is     : %d\n", getuid());
  printf("The effective user ID is: %d\n", geteuid());

  src_fd  = open(argv[1], O_RDONLY);
  dest_fd = open("../home/013969681/xfer.tmp", O_RDONLY);
  
  if(src_fd < 0)
  {
    printf("Failed to open the file.\nNo such file exist with that name in the directory\n");
    return 0;
  }
  else if(dest_fd >= 0)
  {
    printf("Warning: File %s already exists.\nExiting program.\n", argv[2]);
    return 0;
  }
  else
  {
   dest_fd = open("../home/013969681/xfer.tmp", O_CREAT|O_WRONLY, 0777);
   printf("Created file xfer.tmp");
      
   while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
   {
     // I turn it into a string only because I'm using printf;
     // making a copy using "write" would not need this.
     
     write(dest_fd, filebuf, readSize);
     //filebuf[readSize] = '\0';
     //printf("%s",filebuf);
   }
  }
     
  close(src_fd);
  close(dest_fd); 
  return 0;
}
