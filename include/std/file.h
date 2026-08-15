#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <std/str.h>
#include <std/mem.h>
#include <sys/stat.h>

int is_same_file(string_t* a, string_t* b);
int is_same_dir(string_t* a, string_t* b);

#endif
