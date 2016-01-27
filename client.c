/****************** CLIENT CODE ****************/
#include "util.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define KEY 3
#define MAX_SIZE 1028

int main(){
  int clientSocket;
  char bufferIn[MAX_SIZE];
  char bufferMes[MAX_SIZE];
  char bufferHash[MAX_SIZE];
  char bufferSig[MAX_SIZE];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  //Create the socket
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(9876);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  // Connect socket to server
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  //Write message to server
  printf("Please enter the message: ");
  fgets(bufferMes, MAX_SIZE-1, stdin);

  if ((send(clientSocket,bufferMes,MAX_SIZE,0))== -1) {
        fprintf(stderr, "Failure Sending Message\n");
        close(clientSocket);
        exit(1);
  }

  fflush(stdout);

  hash(bufferMes, bufferHash);
  encryption(bufferHash, KEY, bufferSig);

  //Send signature to server
  if ((send(clientSocket,bufferSig,MAX_SIZE,0))== -1) {
        fprintf(stderr, "Failure Sending Signature\n");
        close(clientSocket);
        exit(1);
  }

  fflush(stdout);

  // Read message from server into buffer
  recv(clientSocket, bufferIn, MAX_SIZE-1, 0);

  // Print received message
  fprintf(stdout, "Data received: %s", bufferIn);  

  return 0;
}