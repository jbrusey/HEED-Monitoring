#ifndef _SUBTRACTTIME_H
#define _SUBTRACTTIME_H

#include <limits.h>

typedef unsigned long time_t;
#define TIME_MAX ULONG_MAX

time_t subtract_time(time_t current_time, time_t prev_time);
#endif
