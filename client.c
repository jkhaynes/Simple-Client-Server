/****************** CLIENT CODE ****************/
#include "util.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define KEY 3

int main(){
  int clientSocket;
  char bufferIn[256];
  char bufferMes[1024];
  char bufferHash[256];
  char bufferSig[256];
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
  fgets(bufferMes, 1023, stdin);
  if ((send(clientSocket,bufferMes,strlen(bufferMes),0))== -1) {
        fprintf(stderr, "Failure Sending Message\n");
        close(clientSocket);
        exit(1);
  }
  else {
        printf("Message being sent: %s",bufferMes);
  }

  hash(bufferMes, bufferHash);
  encryption(bufferHash, KEY, bufferSig);

  //Send signature to server
  //strcpy(bufferSig, "Test sig");
  if ((send(clientSocket,bufferSig,strlen(bufferSig),0))== -1) {
        fprintf(stderr, "Failure Sending Signature\n");
        close(clientSocket);
        exit(1);
  }
  else {
        printf("Signature being sent: %s",bufferSig);
  }

  clear(bufferIn);

  // Read message from server into buffer
  recv(clientSocket, bufferIn, 255, 0);

  // Print received message
  printf("Data received: %s",bufferIn);   

  return 0;
}