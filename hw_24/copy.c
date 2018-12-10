#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/des_crypt.h>

int main (int argc, char* argv[]) 
{
  int     src_fd;	/*source file discriptor*/
  int     dest_fd;      /*destination file discriptor*/
  char    filebuf[64];
  ssize_t readSize;	/*signed size_t*/
  char *key = "secret";
  int result;

 
  // argc specifies the count of total command line arguments
  // argv is a pointer to an array of strings
  // argv[0] will be the name of the program
  // argv[1] will be txt file
  // argv[2] will be the name of the destination file

  src_fd  = open(argv[1], O_RDONLY);
  dest_fd = open(argv[2], O_RDONLY);
  
  if(src_fd < 0)
  {
    printf("Failed to open the file.\nNo such file exist with that name in the directory\n");
    return 0;
  }
  else
  {
   if(argc == 4)
   {
     switch(argv[3][0])
     {
       case 'e':
        printf("encrypting...\n");
	dest_fd = open(argv[2], O_CREAT|O_WRONLY, 0777);
        printf("Created file %s\n", argv[2]);
        while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
        { 
          // I turn it into a string only because I'm using printf;
          // making a copy using "write" would not need this.
          result = ecb_crypt(key, filebuf, sizeof(filebuf)-1, DES_ENCRYPT);
          printf("Result: %d\n", result);
          write(dest_fd, filebuf, readSize);
        }
        break;
       case 'd':
        printf("decrypting...\n");
        dest_fd = open(argv[2], O_RDONLY);

        while (( readSize = read(dest_fd, filebuf, sizeof(filebuf)-1)) > 0)
        { 
          result = ecb_crypt(key, filebuf, sizeof(filebuf)-1, DES_ENCRYPT);
          printf("Result: %d\n", result);
        }       
        
        for(int i = 0; i < sizeof(filebuf)-1; i++)
        {
          printf("%c", filebuf[i]);
        }
        printf("\n");
        break;
     }
   }
   else
   {
     dest_fd = open(argv[2], O_CREAT|O_WRONLY, 0777);
     printf("Created file %s\n", argv[2]);
       
     while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
     {
      // I turn it into a string only because I'm using printf;
      // making a copy using "write" would not need this.
      result = ecb_crypt(key, filebuf, sizeof(filebuf)-1, DES_ENCRYPT);
      printf("Result: %d\n", result);
      write(dest_fd, filebuf, readSize);
     }

      for(int i = 0; i < sizeof(filebuf)-1; i++)
      {
        printf("%x", filebuf[i]);
      }
      printf("\n");
      
     result = ecb_crypt(key, filebuf, sizeof(filebuf)-1, DES_DECRYPT);
     printf("Result: %d\n", result);

     for(int i = 0; i < sizeof(filebuf)-1; i++)
     {
       printf("%c", filebuf[i]);
     }
     
     printf("\n");
   }
 }
     
  close(src_fd);
  close(dest_fd); 
  return 0;
}
