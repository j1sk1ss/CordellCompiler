#ifndef TESTING_H_
#define TESTING_H_
#include <stdio.h>
#define assert(x, msg)                             \
    if (!(x)) {                                    \
        fprintf(stderr, "[UNIT ERROR] " msg "\n"); \
        return 1;                                  \
    }
#endif
