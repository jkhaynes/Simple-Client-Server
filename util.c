#include "util.h"

#include <stdio.h>
#include <string.h>

#define N 256

void clear(char* arr){
	int i;
	for(i = 0; i < N; i++)
		arr[i] = 0;
	arr[N] = '\0';
}

void hash(char* message, char* output)
{

	clear(output);
	int i = 0;
    while (i < strlen(message)){
  		output[i % N] += message[i++];
  	}
  	output[N] = '\0';
 
}

void encryption(char* message, int key, char* output)
{
	clear(output);

	int i = 0;
	while (i < strlen(message)-1){
		output[i] = message[i] + key;
		i++;
	}
	output[N] = '\0';
}

void decryption(char* message, int key, char* output)
{
	encryption(message, -key, output);
}

