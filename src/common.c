/* common.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_DRIVER


#include "common.h"

#include <stdio.h>

#include <unistd.h>
#include <sys/time.h>

#include "intlog.h"


void *
xmalloc(size_t sz)
{
	void *r = malloc(sz);
	if (!r)
		CE("malloc");
	return r;
}

uint64_t
millisecs(void)
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) != 0)
		CE("gettimeofday failed");

	return tv.tv_sec * 1000ull + tv.tv_usec / 1000u;
}

char
hexchar(int d)
{
	if (d < 0 || d >= 16)
		C("out of range: %d", d);

	return d + (d < 10 ? '0' : 'A' - 10);
}

uint8_t
fromhexdig(int d)
{
	if (d >= '0' && d <= '9')
		return d - '0';

	if (d >= 'A' && d <= 'Z')
		return d - 'A' + 10;

	if (d >= 'a' && d <= 'z')
		return d - 'a' + 10;

	C("illegal argument: %d (%c)", d, d);
}
