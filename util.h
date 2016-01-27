#ifndef __UTIL_H__
#define __UTIL_H__

void clear(char* arr);
void hash(char* message, char* output);
void encryption(char* message, int key, char* output);
void decryption(char* message, int key, char* output);

#endif //__UTIL_H__