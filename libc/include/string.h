#ifndef NAME_H
#define NAME_H

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(uint8_t *dest, uint8_t val, uint32_t len);
int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(char *src);

#endif
