#ifndef TPRINTF_H
#define TPRINTF_H

#define tprintf(args...) do { \
        struct timeval tv;     \
        gettimeofday(&tv, 0); \
        printf("%s:%d:%s \t", __FILE__,__LINE__, __func__);\
        printf("%lu:\t", tv.tv_sec * 1000 + tv.tv_usec / 1000);\
        printf(args);   \
        } while (0);
#endif
