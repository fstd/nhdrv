/* scrape.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_SCRAPE_H
#define NHDRV_SCRAPE_H 1


#include <stdbool.h>


bool scrape_init(const char *tncpath, const char *srv);

void scrape_input(const char *s);

void scrape_update(void);

#endif
