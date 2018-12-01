/* to compile: gcc -Wall do_crypt.c -lcrypt */

#include <stdio.h> /* for the printf */

/* for the crypt */
#include <unistd.h>
#include <stdlib.h>
#include <crypt.h>

int main(int argc, char* argv[])
{
  char key[64];
  char block[64];
  int i;
  for (i=0 ; i < 64 ; i++)
  {
    key[i] = i%2;
  }
  for (i = 0 ; i < 64 ; i += 4)
  {
    block[i] = 0;
    block[i+1] = 1;
    block[i+2] = 1;
    block[i+3] = 1;
  }
  /* print original */
  for (i=0 ; i < 64 ; i++)
  {
    printf("%x",block[i]);
  }
  printf("\n");

  setkey(key);

  /* encrypt and print the block */
  encrypt(block,0);
  for (i=0 ; i < 64 ; i++)
  {
    printf("%x",block[i]);
  }
  printf("\n");

  /* decrypt and print the block */
  encrypt(block,1);
  for (i=0 ; i < 64 ; i++)
  {
    printf("%x",block[i]);
  }
  printf("\n");
  return 0;
}
