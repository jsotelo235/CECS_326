#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <string.h>


int main (int argc, char* argv[]) 
{
  int     src_fd;	/*source file discriptor*/
  int     dest_fd;      /*destination file discriptor*/
  char    filebuf[64]; /*a small buffer used to copy the file (1-byte)*/
  ssize_t readSize;	/*signed size_t*/

  // Digest variables
  EVP_MD_CTX md_context;
  const EVP_MD *md = EVP_md5();
  unsigned int md_length;
  unsigned char md_value[EVP_MAX_MD_SIZE];
 
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
  else if(dest_fd >= 0)
  {
    printf("Warning: File %s already exists.\nExiting program.\n", argv[2]);
    return 0;
  }
  else
  {
   dest_fd = open(argv[2], O_CREAT|O_WRONLY, 0777);
   printf("Created file %s\n", argv[2]);
   
   // compute the md5 hash
   EVP_DigestInit(&md_context, md);				// init the digest 
   
   // pad the file with zeros
   memset(filebuf, 0, sizeof(filebuf));

   while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
   {
     // I turn it into a string only because I'm using printf;
     // making a copy using "write" would not need this.
     
     write(dest_fd, filebuf, readSize);
     EVP_DigestUpdate(&md_context, filebuf, strlen(filebuf));	// update the digest by feeding data into the digest
   }

   // print the md5 hash
   EVP_DigestFinal(&md_context, md_value, &md_length);		// get the digest and it's length

   for(int i = 0; i < md_length; i++)
   {
     printf("hash: %x\n", md_value[i]);
   }

   printf("\n");
 }
     
  close(src_fd);
  close(dest_fd); 
  return 0;
}
