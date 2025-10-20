#ifndef TKNZ_H_
#define TKNZ_H_

#include <fcntl.h>
#include <ctype.h>
#include <stddef.h>
#include <unistd.h>
#include <std/mm.h>
#include <std/str.h>
#include <std/list.h>
#include <prep/token_types.h>

#define BUFFER_SIZE    8192

/*
Allocate and create token.
Params:
    - type - Token type.
    - value - Token content.
    - len - Value variable size.
    - line - Token line.

Return pointer to token, or NULL if malloc error.
*/
token_t* TKN_create_token(token_type_t type, const char* value, size_t len, int line);

/*
Tokenize input file by provided FD.
Params:
    - fd - File descriptor of target file.
    - tkn - Location of tokens list.

Return NULL or pointer to linked list of tokens.
Note: Function don't close file.
*/
int TKN_tokenize(int fd, list_t* tkn);

#endif