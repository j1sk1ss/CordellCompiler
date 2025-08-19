#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "regs.h"
#include "arrmem.h"
#include "varmem.h"
#include "token.h"
#include "dict.h"
#include "vars.h"
#include "str.h"

typedef struct tree {
    token_t*     token;
    struct tree* parent;
    struct tree* first_child;
    struct tree* next_sibling;
    int          variable_offset;
    int          variable_size;
} tree_t;

typedef struct {
    const char*   scope;
    tree_t*       r;
    varmem_ctx_t* vars;
    arrmem_ctx_t* arrs;
} syntax_ctx_t;

syntax_ctx_t* STX_create_ctx();
int STX_destroy_ctx(syntax_ctx_t* ctx);

/*
Preparing tokens list for parsing tree.
We mark every token by command / symbol ar value type.
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_mnemonics(token_t* head);

/*
Iterate throught tokens and mark variables. 
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_variables(token_t* head);

/*
Generate AST from tokens list.
Params: 
- head - Tokens list head.
- ctx - Parser context.

Return 0 if somehing goes wrong.
Return 1 if markup success.
*/
int STX_create(token_t* head, syntax_ctx_t* ctx);

/*
Create new tree node with token.
Note: Avoid token free before tree free.
Params:
- token - Pointer to token.

Return pointer to tree.
*/
tree_t* STX_create_node(token_t* token);

/*
Add clild tree node to parent.
Params:
- parent - Parent tree node.
- child - Tree node that will be added as child to parent node.

Return 1 if addition was success.
Return -1 if something goes wrong.
*/
int STX_add_node(tree_t* parent, tree_t* child);

/*
Remove clild tree node to parent.
Params:
- parent - Parent tree node.
- child - Tree node that will be removed from childs in parent node.

Return 1 if remove was success.
Return -1 if something goes wrong.
*/
int STX_remove_node(tree_t* parent, tree_t* child);

/*
Unload syntax tree with all childs and siblings.
Params:
- node - Tree head.

Return 1 if free success.
Return -1 if something goes wrong.
*/
int STX_unload(tree_t* node);

#endif