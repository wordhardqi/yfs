// safe assertions.

#ifndef verify_client_h
#define verify_client_h

#include <stdlib.h>
#include <assert.h>
// #include <conio.h>

#ifdef NDEBUG
#define VERIFY(expr) do { if (!(expr)) abort(); } while (0)
#else
#define VERIFY(expr) assert(expr)
#endif

#define Dprintf(fmt, ...) printf("\33[1;34m DEBUG ./%s:%d,%s "  fmt"\33[0m\n", __FILE__, __LINE__,__func__, ##__VA_ARGS__)
 
#endif
