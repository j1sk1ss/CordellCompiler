#ifndef UTILS_H_
#define UTILS_H_

typedef unsigned long      hash_t;   // hash values
typedef unsigned long long big_id_t; // any very big value

/* If you have a case option which is used only under a
   condition, this macro will simplify it */
#define CONDITIONAL_CASE(option, condition, ...) \
    case (option):                               \
        if ((condition)) {                       \
            __VA_ARGS__                          \
        } else {                                 \
            break;                               \
        }

#define ACTION_ON_BITNESS(val, c, uc, s, us, i, ui, l, ul) \
    do {                                                   \
        if (val <= CHAR_MAX)       { c  }                  \
        else if (val <= UCHAR_MAX) { uc }                  \
        else if (val <= SHRT_MAX)  { s  }                  \
        else if (val <= USHRT_MAX) { us }                  \
        else if (val <= INT_MAX)   { i  }                  \
        else if (val <= UINT_MAX)  { ui }                  \
        else if (val <= LONG_MAX)  { l  }                  \
        else                       { ul }                  \
    } while (0);

#endif