#ifndef _STRING_INC
#define _STRING_ING
#include<stddef.h>

void* memset(void *s, int c, size_t n);
void itoa(int n, char s[]);
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
int atoi(char *p);
int stoi(char* str);
#endif
