/* intlog.h - (C) 2015, Timo Buhrmester
 * nhdrv - nethack driver
 * See README for contact-, COPYING for license information. */

#ifndef NHDRV_INTLOG_H
#define NHDRV_INTLOG_H 1


#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>


#define MOD_CORE 0
#define MOD_COMMON 1
#define MOD_DRIVER 2
#define MOD_SCRAPE 3
#define MOD_SCREEN 4
#define MOD_DRV_NOOP 5
#define MOD_TASK_LOGIN 6
#define MOD_TASK_MORE 7
#define MOD_TASK_PROMPT 8
#define MOD_UNKNOWN 9
#define NUM_MODS 10 /* when adding modules, don't forget intlog.c's `modnames' */

/* our two higher-than-debug custom loglevels */
#define LOG_CRIT 2
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7
#define LOG_VIVI 8
#define LOG_TRACE 9

#ifndef LOG_MODULE
# define LOG_MODULE MOD_UNKNOWN
#endif

//[TVDINWE](): log with Trace, Vivi, Debug, Info, Notice, Warn, Error severity.
//[TVDINWE]E(): similar, but also append ``errno'' message
//C(), CE(): as above, but also call exit(EXIT_FAILURE)

// ----- logging interface -----

#define V(...)                                                                 \
 nhd_log_log(LOG_MODULE,LOG_VIVI,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define VE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_VIVI,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define D( ...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_DEBUG,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define DE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_DEBUG,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define I(...)                                                                 \
 nhd_log_log(LOG_MODULE,LOG_INFO,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define IE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_INFO,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define N(...)                                                                 \
 nhd_log_log(LOG_MODULE,LOG_NOTICE,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define NE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_NOTICE,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define W(...)                                                                 \
 nhd_log_log(LOG_MODULE,LOG_WARNING,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define WE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_WARNING,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define E(...)                                                                 \
 nhd_log_log(LOG_MODULE,LOG_ERR,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define EE(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_ERR,errno,__FILE__,__LINE__,__func__,__VA_ARGS__)

#define C(...) do {                                                            \
 nhd_log_log(LOG_MODULE,LOG_CRIT,-1,__FILE__,__LINE__,__func__,__VA_ARGS__);   \
 exit(EXIT_FAILURE); } while (0)

#define CE(...) do {                                                           \
 nhd_log_log(LOG_MODULE,LOG_CRIT,errno,__FILE__,__LINE__,__func__,__VA_ARGS__);\
 exit(EXIT_FAILURE); } while (0)

/* special: always printed, never decorated */
#define A(...)                                                                 \
    nhd_log_log(-1,INT_MIN,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

/* tracing */
#if NOTRACE
# define T(...) do{}while(0)
# define TC(...) do{}while(0)
# define TR(...) do{}while(0)
#else
# define T(...)                                                                \
 nhd_log_log(LOG_MODULE,LOG_TRACE,-1,__FILE__,__LINE__,__func__,__VA_ARGS__)

# define TC(...)                                                               \
 do{                                                                           \
 nhd_log_log(LOG_MODULE,LOG_TRACE,-1,__FILE__,__LINE__,__func__,__VA_ARGS__);  \
 nhd_log_tcall();                                                              \
 } while (0)

# define TR(...)                                                               \
 do{                                                                           \
 nhd_log_tret();                                                               \
 nhd_log_log(LOG_MODULE,LOG_TRACE,-1,__FILE__,__LINE__,__func__,__VA_ARGS__);  \
 } while (0)
#endif

// ----- logger control interface -----

void nhd_log_setlvl_all(int lvl);
void nhd_log_setlvl(int mod, int lvl);
int  nhd_log_getlvl(int mod);

void nhd_log_setfancy(bool fancy);
bool nhd_log_getfancy(void);

void nhd_log_tret(void);
void nhd_log_tcall(void);

// ----- backend -----
void nhd_log_log(int mod, int lvl, int errn, const char *file, int line,
    const char *func, const char *fmt, ...)
#ifdef __GNUC__
    __attribute__ ((format (printf, 7, 8)))
#endif
    ;

void nhd_log_init(void);


#endif /* NHDRV_INTLOG_H */
