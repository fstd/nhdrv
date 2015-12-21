/* scrape.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_SCRAPE


#include "scrape.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "intlog.h"
#include "common.h"
#include "screen.h"


static FILE *s_file;
static bool s_screeninit;
static size_t s_width;
static size_t s_height;

bool
scrape_init(const char *tncpath, const char *srv)
{
	char cmd[256];
	snprintf(cmd, sizeof cmd, "%s -t xterm %s", tncpath, srv);
	if (!(s_file = popen(cmd, "r+"))) {
		EE("popen '%s'", cmd);
		return false;
	}

	if (setvbuf(s_file, (char *)NULL, _IOLBF, 0))
		WE("setvbuf");

	return true;
}

void
scrape_update(void)
{
	char *line = NULL;
	size_t linesize = 0;
	ssize_t linelen;
	if ((linelen = getline(&line, &linesize, s_file)) == -1)
		CE("getline");

	if (line[0] != 'I')
		C("panic: desync (line: '%s')", line);

	uint64_t ts;
	size_t w, h, x, y;
	if (sscanf(line, "I %"SCNu64" %zu %zu %zu %zu", &ts, &w, &h, &x, &y) != 5)
		C("scanf");

	I("w: %zu, h: %zu, x: %zu, y: %zu, ts: %"PRIu64, w, h, x, y, ts);

	if (!s_screeninit) {
		screen_init(w, h);
		s_screeninit = true;
		s_width = w;
		s_height = h;
	} else if (w != s_width || h != s_height)
		C("panic: terminal resized"); //XXX deal with this later


	for (size_t i = 0; i < h; i++) {
		if ((linelen = getline(&line, &linesize, s_file)) == -1)
			CE("getline");

		char tmp[8];
		snprintf(tmp, sizeof tmp, "L\t%zu\t", i);
		if (strncmp(line, tmp, strlen(tmp)) != 0)
			C("panic: desync (line: '%s')", line);

		char *lp = line + strlen(tmp);
		screen_goto(0, i);
		for (size_t j = 0; j < w; j++) {
			uint8_t attr = fromhexdig(lp[4*j]);
			uint8_t fgcol = fromhexdig(lp[4*j+1]);
			uint8_t bgcol = fromhexdig(lp[4*j+2]);
			screen_setattr(attr, fgcol, bgcol);
			screen_putchar(lp[4*j+3]);
		}
	}

	screen_goto(x, y);
}

void
scrape_input(const char *s)
{
	if (!*s)
		return;

	if (!s_file)
		C("put where?!");

	if (fputs(s, s_file) != 0)
		CE("fputs");

	if (fflush(s_file) != 0)
		CE("fflush");
}
