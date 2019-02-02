#ifndef gettime_h
#define gettime_h

#ifndef __APPLE__


#include <time.h>

// #define CLOCK_REALTIME  0
// #define CLOCK_MONOTONIC = 1;
// #define CLOCK_PROCESS_CPUTIME_ID =  2;
// #define CLOCK_THREAD_CPUTIME_ID = 3;
// typedef  int clockid_t;
int clock_gettime( clockid_t clk_id, struct timespec *tp);
#endif

#endif
