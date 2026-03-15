#ifndef WARNS_H_
#define WARNS_H_

#include <stdio.h>

typedef enum {
    ATTENTION_UNKNOWN_LEVEL, /* Unknown attention level. Will fire a warning only with all warns enabled  */
    ATTENTION_LOW_LEVEL,     /* Low level of an attantion. Will fire a warning with a low level enabled   */
    ATTENTION_MEDIUM_LEVEL,  /* Mid level of an attantion. Will fire a warning with a mid level enabled   */
    ATTENTION_HIGH_LEVEL,    /* High level of an attantion. Will fire a warning with a high level enabled */
    ATTENTION_BLOCK_LEVEL    /* Will block the code compilation if it fires                               */
} attention_level_t;

#define SEMANTIC_ERROR(message, ...)   fprintf(stdout, "[ERROR]  " message "\n", ##__VA_ARGS__)
#define SEMANTIC_WARNING(message, ...) fprintf(stdout, "[WARNING]" message "\n", ##__VA_ARGS__)
#define SEMANTIC_INFO(message, ...)    fprintf(stdout, "[INFO]   " message "\n", ##__VA_ARGS__)

#endif
