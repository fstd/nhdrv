/* driver.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_DRIVER_H
#define NHDRV_DRIVER_H 1


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>


struct driver_if;

typedef bool (*driver_action_fn)(char *resp, size_t respsz);
typedef void (*driver_attach_fn)(struct driver_if *ifc);

struct driver_if {
	driver_action_fn action_fn;
};


bool driver_attach(const char *drivername);
bool driver_action(char *resp, size_t respsz);

void driver_dumpnames(void);

#endif
