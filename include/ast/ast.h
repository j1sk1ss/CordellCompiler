#ifndef AST_H_
#define AST_H_

#include <std/mm.h>
#include <std/stack.h>
#include <symtab/symtab_id.h>
#include <prep/token.h>
#include <prep/token_types.h>

typedef struct {
    long        s_id;  /* Scope ID    */
    symbol_id_t v_id;  /* Variable ID */
} syntax_info_t;

typedef struct ast_node {
    token_t*             t;  /* Linked AST token                           */
                             /* Note: This token mustn't be freed manualy! */
    token_t*             bt; /* Backup AST token                           */
                             /* Note: If it isn't the 'NULL' value,        */ 
                             /* must be freed manualy!                     */
    struct ast_node*     p;  /* Parent pointer                             */
    struct ast_node*     c;  /* Child pointer                              */
    struct {
        struct ast_node* n;  /* Siblings next pointer                      */
        struct ast_node* t;  /* Siblings list tail                         */
    } siblings;
    syntax_info_t    sinfo;  /* Scope information for the current node     */
} ast_node_t;

typedef struct {
    ast_node_t*  r;      /* AST root.                                */
    const char*  fentry; /* Name for entry function. [Arch depended] */
    struct {
        int      s_id;   /* Current scope id.                        */
        sstack_t stack;  /* Scope id stack.                          */
    } scopes;
    sstack_t     annots; /* Annotations                              */
    struct {
        void*    ptr;    /* Parent function pointer                  */
    } carry;
} ast_ctx_t;

/*
Create new tree node with a token.
Note: Avoid token free before tree free.
Params:
    - `token` - Pointer to token.
                Note: Can be NULL.

Return a pointer to the node.
*/
ast_node_t* AST_create_node(token_t* token);

/*
Create new tree node with a token.
Note: Avoid token free before tree free.
Note 2: In comparison with the 'AST_create_node' function,
        this function preserves the 'tkn' in the 'bt' field.
Params:
    - `token` - Pointer to token.
                Note: Can be NULL.

Returns a pointer to the node.
*/
ast_node_t* AST_create_node_bt(token_t* tkn);

/*
Cope ast noe.
Params:
    - `n` - Source target node
    - `sp` - Save parent link
    - `sib` - Copy siblings
    - `chld` - Copy childrens

Return pointer to new deep copied node.
*/
ast_node_t* AST_copy_node(ast_node_t* n, int sp, int sib, int chld);

/*
Add clild tree node to parent.
Params:
    - `parent` - Parent tree node.
    - `child` - Tree node that will be added as child to parent node.

Return 1 if addition was success.
Return -1 if something goes wrong.
*/
int AST_add_node(ast_node_t* parent, ast_node_t* child);

/*
Remove clild tree node to parent.
Params:
    - `parent` - Parent tree node.
    - `child` - Tree node that will be removed from childs in parent node.

Return 1 if remove was success.
Return -1 if something goes wrong.
*/
int AST_remove_node(ast_node_t* parent, ast_node_t* child);

/*
Unload syntax tree with all childs and siblings.
Params:
    - `node` - Tree head.

Return 1 if free success.
Return -1 if something goes wrong.
*/
int AST_unload(ast_node_t* node);

/*
Hash an ast-node with the crc64 hash function.
Params:
    - `node` - AST-node.

Return hash sum of the AST-node.
*/
unsigned long AST_hash_node(ast_node_t* node);

/*
Hash an ast-node with the crc64 hash function.
Params:
    - `node` - AST-node.
    - `stp` - Stop token type.

Return hash sum of the AST-node.
*/
unsigned long AST_hash_node_stop(ast_node_t* node, token_type_t stp);

#endif
