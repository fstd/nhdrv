/* task_login.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_TASK_LOGIN_H
#define NHDRV_TASK_LOGIN_H 1

#include <stddef.h>

int task_login(char *resp, size_t respsz);
void task_login_init(const char *user, const char *pass);

#endif

