#ifndef WARNS_H_
#define WARNS_H_

#include <stdio.h>

#define SEMANTIC_ERROR(message, ...)   fprintf(stdout, "[ERROR]  " message "\n", ##__VA_ARGS__)
#define SEMANTIC_WARNING(message, ...) fprintf(stdout, "[WARNING]" message "\n", ##__VA_ARGS__)
#define SEMANTIC_INFO(message, ...)    fprintf(stdout, "[INFO]   " message "\n", ##__VA_ARGS__)

#endif
