/* task.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack task
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_BEHAVIOR_H
#define NHDRV_BEHAVIOR_H 1


#include <stddef.h>
#include <stdbool.h>

#include "task_login.h"
#include "task_more.h"
#include "task_prompt.h"

typedef int (*task_fn)(char *resp, size_t respsz);



#endif
