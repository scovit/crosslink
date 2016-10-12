#ifndef HEX_H
#define HEX_H

#include <stddef.h>

void bin2hex(char *dest, const void *source, const size_t n);
int hex2bin(void *dest, const char *source);

#endif
