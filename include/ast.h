#ifndef AST_H_
#define AST_H_

#include "token.h"

typedef struct {
    short s_id;
    int   offset;
    int   size;
} syntax_info_t;

typedef struct ast_node {
    token_t*         token;
    struct ast_node* parent;
    struct ast_node* child;
    struct ast_node* sibling;
    syntax_info_t    info;
} ast_node_t;

/*
Create new tree node with token.
Note: Avoid token free before tree free.
Params:
- token - Pointer to token.
          Note: Can be NULL.

Return pointer to tree.
*/
ast_node_t* AST_create_node(token_t* token);

/*
Cope ast noe.
Params:
- n - Source target node
- sp - Save parent link
- sib - Copy siblings
- chld - Copy childrens

Return pointer to new deep copied node.
*/
ast_node_t* AST_copy_node(ast_node_t* n, int sp, int sib, int chld);

/*
Add clild tree node to parent.
Params:
- parent - Parent tree node.
- child - Tree node that will be added as child to parent node.

Return 1 if addition was success.
Return -1 if something goes wrong.
*/
int AST_add_node(ast_node_t* parent, ast_node_t* child);

/*
Remove clild tree node to parent.
Params:
- parent - Parent tree node.
- child - Tree node that will be removed from childs in parent node.

Return 1 if remove was success.
Return -1 if something goes wrong.
*/
int AST_remove_node(ast_node_t* parent, ast_node_t* child);

/*
Unload syntax tree with all childs and siblings.
Params:
- node - Tree head.

Return 1 if free success.
Return -1 if something goes wrong.
*/
int AST_unload(ast_node_t* node);

#endif