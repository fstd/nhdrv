/* nhdrv.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_CORE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "intlog.h"
#include "common.h"
#include "scrape.h"
#include "screen.h"
#include "driver.h"
#include "task.h"


static const char *s_tncpath = "/usr/local/bin/telnetcat";
static const char *s_srv = "localhost";
static const char *s_driver = "noop";
static const char *s_user;
static const char *s_pass;

static void process_args(int *argc, char ***argv);
static void init(int *argc, char ***argv);
static void usage(FILE *str, const char *a0, int ec);
static void update_logger(int verb, int fancy);


static void
process_args(int *argc, char ***argv)
{
	char *a0 = (*argv)[0];

	for(int ch; (ch = getopt(*argc, *argv, "cvqt:s:d:u:p:h")) != -1;) {
		switch (ch) {
		case 't':
			s_tncpath = strdup(optarg);
			break;
		case 'd':
			s_driver = strdup(optarg);
			break;
		case 'u':
			s_user = strdup(optarg);
			break;
		case 'p':
			s_pass = strdup(optarg);
			break;
		case 's':
			s_srv = strdup(optarg);
			break;
		case 'c':
			update_logger(0, 1);
			break;
		case 'v':
			update_logger(1, -1);
			break;
		case 'q':
			update_logger(-1, -1);
			break;
		case 'h':
			usage(stdout, a0, EXIT_SUCCESS);
			break;
		case '?':
		default:
			usage(stderr, a0, EXIT_FAILURE);
		}
	}

	*argc -= optind;
	*argv += optind;

	if (!s_user || !s_pass)
		C("need -u <pass> and -p <pass>");
}


static void
init(int *argc, char ***argv)
{
	nhd_log_init();
	process_args(argc, argv);

	if (!driver_attach(s_driver))
		C("could not attach driver '%s'", s_driver);

	if (!scrape_init(s_tncpath, s_srv))
		C("could not init scrape");

	task_login_init(s_user, s_pass);

	N("Initialized");
}


static void
usage(FILE *str, const char *a0, int ec)
{
	#define U(STR) fputs(STR "\n", str)
	U("===================================");
	U("== nhdrv - nethack driver v"PACKAGE_VERSION" ==");
	U("===================================");
	fprintf(str, "usage: %s [-h]\n", a0);
	U("");
	U("\t-h: Display brief usage statement and terminate");
	U("");
	U("(C) 2015, Timo Buhrmester (contact: #fstd on irc.freenode.org)");
	#undef U
	exit(ec);
}


static void
update_logger(int verb, int fancy)
{
	int v = nhd_log_getlvl(MOD_CORE) + verb;
	if (v < 0)
		v = 0;

	if (fancy == -1)
		fancy = nhd_log_getfancy();

	nhd_log_setfancy(fancy);
	nhd_log_setlvl_all(v);
}


int
main(int argc, char **argv)
{
	init(&argc, &argv);
	scrape_input("\r\n"); //why do we need this?

	task_fn tasks[] = {
		task_login,
		task_more,
		task_prompt,
	};

	for (;;) {
		char resp[256] = {0};

		scrape_update();
		screen_output(true); //debug

		bool taskdone = false;
		for (size_t i = 0; i < COUNTOF(tasks); i++) {
			int r = tasks[i](resp, sizeof resp);
			if (r < 0)
				C("task %zu panic", i);

			if (r > 0) {
				N("task %zu did its thing", i);
				taskdone = true;
				break;
			}
		}

		if (!taskdone && !driver_action(resp, sizeof resp))
			C("panic: driver doesn't know what to do");

		scrape_input(resp);
	}

	return EXIT_SUCCESS;
}
