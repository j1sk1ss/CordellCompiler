#ifndef UTILS_H_
#define UTILS_H_

/* If you have a case option which is used only under a
   condition, this macro will simplify it */
#define CONDITIONAL_CASE(option, condition, ...) \
    case (option):                               \
        if ((condition)) {                       \
            __VA_ARGS__                          \
        } else {                                 \
            break;                               \
        }

#endif