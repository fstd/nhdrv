/* driver.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_DRIVER


#include "driver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "intlog.h"


struct drivertype {
	char name[32];
	driver_attach_fn attach_fn;
};

#define REG(NAME) void drv_ ## NAME ## _attach(struct driver_if *ifc);
#include "drivers.h"
#undef REG

static struct drivertype s_drivertype[] = {
	#define REG(NAME) { #NAME, drv_ ## NAME ## _attach },
	#include "drivers.h"
	#undef REG
};

static struct driver_if s_driverif;


bool
driver_attach(const char *drivername)
{
	for (size_t i = 0; i < COUNTOF(s_drivertype); i++) {
		if (strcmp(s_drivertype[i].name, drivername) == 0) {
			s_drivertype[i].attach_fn(&s_driverif);
			return true;
		}
	}

	E("could not attach `%s`", drivername);
	return false;
}

bool
driver_action(char *resp, size_t respsz)
{
	return s_driverif.action_fn(resp, respsz);
}

void
driver_dumpnames(void)
{
	for (size_t i = 0; i < COUNTOF(s_drivertype); i++)
		puts(s_drivertype[i].name);
}
