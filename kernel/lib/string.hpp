#ifndef _string_h
#define _string_h

#include <inttypes.h>

inline int strcmp(const char *s1, const char *s2)
{
    while (*s1 != 0 && *s2 != 0) {
        if (*s1 != *s2) {
            break;
        }
        s1 += 1;
        s2 += 1;
    }

    return *s1 - *s2;
}

inline void *memcpy(void *dest, const void *src, uint32_t n)
{
    char *cdest;
    const char *csrc = (const char *) src;

    cdest = (char *) dest;
    while (n-- > 0) {
        *cdest++ = *csrc++;
    }

    return dest;
}

inline uint32_t strsplit(char *str, char c)
{
	int i=0;

	while(*str != '\0') {
		if (*str == c) {
			i++;
		}

		str++;
	}

	return i;
}

#endif