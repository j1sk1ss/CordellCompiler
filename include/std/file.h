#ifndef FILE_H_
#define FILE_H_

#include <std/str.h>
#include <std/mem.h>
#include <sys/stat.h>
#include <stdio.h>

int is_same_file(string_t* a, string_t* b);
int is_same_dir(string_t* a, string_t* b);

#endif
