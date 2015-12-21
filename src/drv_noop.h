/* drv_noop.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_DRV_NOOP_H
#define NHDRV_DRV_NOOP_H 1


#include "driver.h"


bool drv_noop_action(char *resp, size_t respsz);

void drv_noop_attach(struct driver_if *ifc);


#endif /* NHDRV_DRV_NOOP_H */
