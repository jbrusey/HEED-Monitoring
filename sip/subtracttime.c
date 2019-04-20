#include "subtracttime.h"
#include <limits.h>
/**
 * subtract_time
 * find time difference between two times
 * assumes time_t wraps after reaching TIME_MAX 
 */
time_t subtract_time(time_t current_time, time_t prev_time)
{
  if (current_time < prev_time) // deal with overflow
    return ((TIME_MAX - prev_time) + current_time + 1);
  else
    return (current_time - prev_time);
}

