#include "util.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int welcomeSocket, newSocket;
  char bufferMes[1024];
  char bufferHash[256];
  char bufferHash2[256];
  char bufferSig[256];
  char bufferOut[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  // Create socket
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(9876);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  // Bind address struct to socket
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  // Listen to socket
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  // Accept call creates a new socket for the incoming connection
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  int num;

  //Recieve message from client
  if ((num = recv(newSocket, bufferMes, 1023,0))== -1) {
        perror("recv");
        exit(1);
  }   
  else if (num == 0) {
        printf("Connection closed\n");
        return 0;
  }
  bufferMes[num] = '\0';
  printf("Message received: %s\n", bufferMes);

  //Recieve sig from client
  if ((num = recv(newSocket, bufferSig, 255,0))== -1) {
        perror("recv");
        exit(1);
  }   
  else if (num == 0) {
        printf("Connection closed\n");
        return 0;
  }
  bufferSig[num] = '\0';
  printf("Signature received: %s\n", bufferSig);

  hash(bufferMes, bufferHash);
  decryption(bufferSig, 3, bufferHash2);

  char *toSend;
  if(strcmp(bufferHash, bufferHash2) == 0)
      bufferOut[0] = "True\n";
  else
      bufferOut[0] = "False\n";

  // Send message to client
  //strcpy(bufferOut,"Hello World\n");
  if((send(newSocket,bufferOut,strlen(bufferOut),0)) == -1) {
    fprintf(stderr, "Failure Sending Message\n");
    close(newSocket);
    exit(1);
  }

  close(newSocket);
  return 0;
}