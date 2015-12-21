/* common.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_COMMON_H
#define NHDRV_COMMON_H 1


#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>


#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define STRTOSZ(STR) (size_t)strtoull((STR), NULL, 0)
#define STRTOU(STR) (unsigned)strtoul((STR), NULL, 0)
#define COUNTOF(ARR) (sizeof (ARR) / sizeof (ARR)[0])


void *xmalloc(size_t sz);
uint64_t millisecs(void);
char hexchar(int d);
uint8_t fromhexdig(int d);

#endif /* NHDRV_COMMON_H */
