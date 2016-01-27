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
  int welcomeSocket, newSocket;
  char bufferMes[MAX_SIZE];
  char bufferHash[MAX_SIZE];
  char bufferHash2[MAX_SIZE];
  char bufferSig[MAX_SIZE];
  char bufferOut[MAX_SIZE];
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
  if ((num = recv(newSocket, bufferMes, MAX_SIZE,0))== -1) {
        perror("recv");
        exit(1);
  }   
  else if (num == 0) {
        printf("Connection closed\n");
        return 0;
  }
  //bufferMes[num] = '\0';
  printf("Message received: %s\n", bufferMes);

  //Recieve sig from client
  if ((num = recv(newSocket, bufferSig, MAX_SIZE,0))== -1) {
        perror("recv");
        exit(1);
  }   
  else if (num == 0) {
        printf("Connection closed\n");
        return 0;
  }
  //bufferSig[num] = '\0';
  printf("Signature received: %s\n", bufferSig);

  hash(bufferMes, bufferHash);
  decryption(bufferSig, KEY, bufferHash2);

  char *toSend;
  if(strcmp(bufferHash, bufferHash2) == 0)
      toSend = "True\n" + '\0';
  else
      toSend = "False\n" + '\0';

  // Send message to client
  if((send(newSocket,toSend,strlen(toSend),0)) == -1) {
    fprintf(stderr, "Failure Sending Message\n");
    close(newSocket);
    exit(1);
  }


  //close(newSocket);
  return 0;
}