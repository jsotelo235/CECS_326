#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 7

int main() {
  int sd; 	// socket descriptor
  //char *msg = "Hello from cecs 326 server";
  char *msg;
  char message[1024];
  struct sockaddr_in      srv_addr;

  /* Get a socket. */
  sd = socket(PF_INET, SOCK_STREAM, 0);

  if(sd < 0)
  {
    perror("socket descriptor failed\n");
    exit(EXIT_FAILURE);
  }

  /* Put server's address into a socket structure */
  //memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_addr.s_addr = inet_addr("134.139.248.2"); // connect to the server's address
  srv_addr.sin_family = AF_INET;			 // IPv4 Internet Protocols
  srv_addr.sin_port = htons(PORT);		         // connect to server on port 7

  /* Request the connection to the server */
  connect(sd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
  
  printf("Enter a message to send to server: ");
  fgets(msg, sizeof(message), stdin);
  printf("\n");
  printf("sending message to server...\n"); 
  write(sd, msg, sizeof(message));
  
  printf("echoing message from server...\n\n");
  read(sd, message, sizeof(message));
  printf("%s",message);
  printf("\n");
  close(sd);
  return 0;
}
