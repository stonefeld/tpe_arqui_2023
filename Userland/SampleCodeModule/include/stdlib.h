#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

uint32_t gets(char* buff, uint32_t size);
uint8_t getchar();
void puts(char* str);
void putchar(char c);

uint64_t strlen(char* buff);
uint32_t strtok(char* buff, uint8_t token, char** args, uint32_t size);
uint32_t strcmp(char* s1, char* s2);

uint32_t clear();
uint64_t get_ticks();
#endif
