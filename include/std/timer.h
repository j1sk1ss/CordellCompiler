#ifndef TIMER_H_
#define TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <sys/time.h>

typedef struct ttimer {
    unsigned long total;
    int           operations;
} ttimer_t;

static inline void add_time_timer(long time, struct ttimer* t) {
    t->total += time;
    t->operations++;
}

static inline void reset_time_timer(struct ttimer* t) {
    t->operations = 0;
    t->total      = 0;
}

static inline double get_avg_timer_and_reset(struct ttimer* t) {
    if (!t->operations) return 0;
    double res = t->total / (double)t->operations;
    reset_time_timer(t);
    return res;
}

static inline long _current_time_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000L) + tv.tv_usec;
}

#define MEASURE_TIME_US(code) ({ \
    long _start = _current_time_us(); \
    code; \
    long _end = _current_time_us(); \
    _end - _start; \
})

#ifdef __cplusplus
}
#endif
#endif