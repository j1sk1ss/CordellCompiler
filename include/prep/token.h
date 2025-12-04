#ifndef TKNZ_H_
#define TKNZ_H_

#include <fcntl.h>
#include <ctype.h>
#include <stddef.h>
#include <unistd.h>
#include <std/mm.h>
#include <std/str.h>
#include <std/hash.h>
#include <std/list.h>
#include <prep/token_types.h>

#define BUFFER_SIZE 8192

/*
Allocate and create toke + copy data from src token.
Params:
    - src - Source token data.

Return NULL or copied token.
*/
token_t* TKN_copy_token(token_t* src);

/*
Allocate and create token.
Params:
    - type - Token type.
    - value - Token content.
    - len - Value variable size.
    - line - Token line.

Return pointer to token, or NULL if malloc error.
*/
token_t* TKN_create_token(token_type_t type, const char* value, int line);

/*
Tokenize input file by provided FD.
Params:
    - fd - File descriptor of target file.
    - tkn - Location of tokens list.

Return NULL or pointer to linked list of tokens.
Note: Function don't close file.
*/
int TKN_tokenize(int fd, list_t* tkn);

/*
Hash a token with the crc64 hash function.
Params:
    - t - Token.

Return hash sum.
*/
unsigned long TKN_hash_token(token_t* t);

/*
Deallocated space that was claimed by the token.
Params:
    - t - Token.

Return 1 if allocation was succeed, otherwise return 0.
*/
int TKN_unload_token(token_t* t);

#endif