#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  // write program that opens and memory maps that file
  // 1) ask the user which position in the file they want to change
  // 2) ask the user what the new letter is for that position
  // 3) modify the file using mmap
  
  int fd;
  int fileSize;
  int position;
  char *addr;

  char newLetter;
  struct stat buf;	// struct used to obtain info about a file

  // open the txt
  // the txt was previously created and contains sentences 
  fd = open("mmap.txt", O_RDWR);

  // file descriptor error checking
  if(fd < 0)
  {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }
 
  // On success, zero is returned
  // On failure, it returns a -1
  if(fstat(fd, &buf) < 0)
  {
   perror("stat failed");
   exit(EXIT_FAILURE);
  }
  
  // retrieving the size of the file
  fileSize = buf.st_size;


  for(int i = 0; i < 3; i++)
  {
    // file is ready to be mmapped
    addr = mmap(0, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
  
    // On success, mmap() returns a ptr to the mapped area
    // On failure, it returns a -1
    if(addr == MAP_FAILED)
    {
      perror("mmap error");
      close(fd);
      exit(EXIT_FAILURE);
    }
  
    printf("Enter the position in the file do you want to change: ");
    scanf("%d", &position);

    printf("Enter the new letter for that position: "); 
    scanf(" %c", &newLetter);
    printf("\n");
 
    addr[position] = newLetter;

    // On success, munmap() returns 0.
    // On failure, it returns -1
    if(munmap(addr, fileSize) < 0)
    {
      perror("mumap error");
      close(fd);
      exit(EXIT_FAILURE);
    }
  }
   
  close(fd);
  return 0;
}


