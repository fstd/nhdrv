/* task_login.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_TASK_LOGIN


#include "task_login.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "intlog.h"
#include "screen.h"


static bool s_done;

static const char *s_user;
static const char *s_pass;


int
task_login(char *resp, size_t respsz)
{
	if (s_done)
		return 0;

	if (screen_findstr("p) Play NetHack 3.6.0", NULL, NULL)) {
		snprintf(resp, respsz, "p");
		s_done = 1;
	} else if (screen_findstr("1) Go to NetHack 3.6.0 menu", NULL, NULL)) {
		snprintf(resp, respsz, "1");
	} else if (screen_findstr("Please enter your password.", NULL, NULL)) {
		snprintf(resp, respsz, "%s\r\n", s_pass);
	} else if (screen_findstr("Please enter your username.", NULL, NULL)) {
		snprintf(resp, respsz, "%s\r\n", s_user);
	} else if (screen_findstr("Not logged in.", NULL, NULL)) {
		snprintf(resp, respsz, "l");
	} else {
		return 0;
	}

	return 1;
}

void
task_login_init(const char *user, const char *pass)
{
	s_user = strdup(user);
	s_pass = strdup(pass);
}
