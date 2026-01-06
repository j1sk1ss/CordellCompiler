#ifndef RESTORE_H_
#define RESTORE_H_

#include <stdio.h>
#include <std/set.h>
#include <prep/dict.h>
#include <prep/token_types.h>
#include <ast/ast.h>

#define UNDERSCORE_OPEN  "\e[4m"
#define UNDERSCORE_CLOSE "\e[0m"

/*
Restore the token's type.
Note: Will return an empty string if the provided
      token isn't a type token.
Params:
    - `t` - Token.

Returns the resored string of the provided token.
*/
const char* RST_restore_type(token_t* t);

/*
Print to the FD the restored AST `nd` node.
Note: We need this function to restore any AST node to a
      code. This helps a lot when we're having deal with
      debugging, static analysis, etc.
Params:
    - `fd` - Target file descriptor for the output.
    - `nd` - Source AST node for the printing.
    - `u` - Element's set to underscore.
    - `indent` - Initial indent.

Returns 1 if succeeds.
*/
int RST_restore_code(FILE* fd, ast_node_t* nd, set_t* u, int indent);

#endif