/* drv_noop.c - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#define LOG_MODULE MOD_DRV_NOOP


#include "drv_noop.h"

#include "driver.h"
#include "intlog.h"


bool
drv_noop_action(char *resp, size_t respsz)
{
	I("action");
	resp[0] = '\0';
	return true;
}

void
drv_noop_attach(struct driver_if *ifc)
{
	ifc->action_fn = drv_noop_action;
	N("attached");
}
