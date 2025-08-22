/* LL(1) parser */
#include <syntax.h>

static inline int _forward_token(token_t** tkn, int steps) {
    while (steps-- > 0) {
        if ((*tkn)->next) *tkn = (*tkn)->next;
        else return 0;
    }

    return 1;
}

static ast_node_t* _parse_import(              token_t**, syntax_ctx_t*);
static ast_node_t* _parse_variable_declaration(token_t**, syntax_ctx_t*);
static ast_node_t* _parse_array_declaration(   token_t**, syntax_ctx_t*);
static ast_node_t* _parse_binary_expression(   token_t**, syntax_ctx_t*, int);
static ast_node_t* _parse_primary(             token_t**, syntax_ctx_t*);
static ast_node_t* _parse_function_call(       token_t**, syntax_ctx_t*);
static ast_node_t* _parse_function_declaration(token_t**, syntax_ctx_t*);
static ast_node_t* _parse_return_declaration(  token_t**, syntax_ctx_t*);
static ast_node_t* _parse_switch_expression(   token_t**, syntax_ctx_t*);
static ast_node_t* _parse_condition_scope(     token_t**, syntax_ctx_t*);
static ast_node_t* _parse_syscall(             token_t**, syntax_ctx_t*);
static ast_node_t* _parse_expression(          token_t**, syntax_ctx_t*);
static ast_node_t* _parse_array_expression(    token_t**, syntax_ctx_t*);
static ast_node_t* _parse_scope(               token_t**, syntax_ctx_t*);
static ast_node_t* _dummy_parser(token_t** curr, syntax_ctx_t* ctx) { return NULL; }

/* Get parser for token type */
static ast_node_t* (*_get_parser(token_type_t t_type))(token_t**, syntax_ctx_t*) {
    switch (t_type) {
        case OPEN_BLOCK_TOKEN:
        case CLOSE_BLOCK_TOKEN:    return _parse_scope;
        case STR_TYPE_TOKEN:
        case INT_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:
        case LONG_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:      return _parse_variable_declaration;
        case SWITCH_TOKEN:          return _parse_switch_expression;
        case IF_TOKEN:              
        case WHILE_TOKEN:           return _parse_condition_scope;
        case INT_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case UNKNOWN_STRING_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN: return _parse_expression;
        case SYSCALL_TOKEN:         return _parse_syscall;
        case IMPORT_SELECT_TOKEN:   return _parse_import;
        case ARRAY_TYPE_TOKEN:      return _parse_array_declaration;
        case CALL_TOKEN:            return _parse_function_call;
        case FUNC_TOKEN:            return _parse_function_declaration;
        case EXIT_TOKEN:
        case RETURN_TOKEN:          return _parse_return_declaration;
        default:                    return _dummy_parser;
    }
}

/* Save variable to ctx varmem list */
static int _var_update(
    ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size
) {
    if (!node) return 0;
    node->info.offset = VRM_add_info(name, size, scope_id_top(&ctx->scope.stack), ctx->vars);
    node->info.size   = size;
    return 1;
}

/* Lookup variable from vartable in ctx varmem list, save offset and size */
static int _var_lookup(ast_node_t* node, syntax_ctx_t* ctx) {
    if (!node) return 0;
    _var_lookup(node->sibling, ctx);
    _var_lookup(node->child, ctx);

    if (!node->token) return 0;
    variable_info_t varinfo = { .offset = -1 };
    for (int s = ctx->scope.stack.top; s >= 0; s--) {
        int s_id = ctx->scope.stack.data[s].id;
        if (VRM_get_info(node->token->value, s_id, &varinfo, ctx->vars)) {
            break;
        }
    }
    
    if (varinfo.offset == -1) return 0;
    node->info.offset = varinfo.offset;
    node->info.size   = varinfo.size;
    node->info.s_id   = varinfo.scope;
    return 1;
}

/*
Arch:
scope
|_ exp1 (val | exp)
|_ exp2 (val | exp)
|_ ...
|_ exit_token (Not included)
*/
static ast_node_t* _parse_block(token_t** curr, syntax_ctx_t* ctx, token_type_t ex) {
    ast_node_t* node = AST_create_node(NULL);
    if (!node) return NULL;
    
    while (*curr && (*curr)->t_type != ex) {
        ast_node_t* block = _get_parser((*curr)->t_type)(curr, ctx);
        if (block) AST_add_node(node, block);
        else {
            if (!_forward_token(curr, 1)) break;
        }
    }
    
    return node;
}

/* 
Handle only scopes. Don't handle scopes included to special structs like if, while, function and etc.
*/
static ast_node_t* _parse_scope(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = NULL;
    if ((*curr)->t_type == OPEN_BLOCK_TOKEN) {
        _forward_token(curr, 1);
        scope_push(&ctx->scope.stack, ++ctx->scope.s_id, ctx->vars->offset);
        node = _parse_block(curr, ctx, CLOSE_BLOCK_TOKEN);
        if (node) {
            node->token = TKN_create_token(SCOPE_TOKEN, NULL, 0, 0);
            node->info.s_id = scope_id_top(&ctx->scope.stack);
            _forward_token(curr, 1);
        }
    }

    scope_elem_t el;
    scope_pop_top(&ctx->scope.stack, &el);
    ctx->vars->offset = el.offset;

    return node;
}

/*
from "src.cpl" import func1 func2;
Where "from" - import_node.
"src.cpl" - source_node.
And then adding to source_node func1 and func2.

Arch:
import_token
|_ src
   |_ fname1 (str)
   |_ fname2 (str)
   |_ ...
*/
static ast_node_t* _parse_import(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    /* "from" statement -> "source path" */
    _forward_token(curr, 1);
    ast_node_t* source_node = AST_create_node(*curr); // example: src.cpl
    if (!source_node) {
        AST_unload(node);
        return NULL;
    }

    /* Skip "import" statement to list of functions */
    _forward_token(curr, 2);
    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        ast_node_t* fname = AST_create_node(*curr); // example: func1
        if (!fname) {
            AST_unload(node);
            AST_unload(source_node);
            return NULL;
        }

        AST_add_node(source_node, fname);
        _forward_token(curr, 1);
    }

    AST_add_node(node, source_node);
    return node;
}

/*
func1 arg1 arg2;
"func1" - call_node.
And then adding to scope arg1 and arg2.
Example: name(arg1, arg2, arg3, ...);

Arch:
call_token
|_ arg1 (val | exp)
|_ ...
*/
static ast_node_t* _parse_function_call(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    _forward_token(curr, 1);
    if (*curr && (*curr)->t_type == OPEN_BRACKET_TOKEN) {
        _forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) { /* Skip comma */
                _forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = _parse_expression(curr, ctx);
            if (arg) AST_add_node(node, arg); /* Parse expressions between commas. Example a + b, c + y, ... */
        }
    }

    return node;
}

/*
Parser for "return <exp>;" expression.
Same as for "end <exp>;".
Arch:
return_token
|_ ret_val (val | exp)
*/
static ast_node_t* _parse_return_declaration(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    _forward_token(curr, 1);
    ast_node_t* exp_node = _parse_expression(curr, ctx);
    if (!exp_node) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, exp_node);
    return node;
}

/*
Arch:
func_token
|_ name
|_ scope
|  |_ arg1
|  |  |_ type
|  |    |_ name (str)
|  |_ ...
|_ scope
   |_ body1 (exp)
   |_ ...
*/
static ast_node_t* _parse_function_declaration(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    _forward_token(curr, 1);
    
    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, name_node);
    _forward_token(curr, 1);

    /* Manual stack push instead parser */
    scope_push(&ctx->scope.stack, ++ctx->scope.s_id, ctx->vars->offset); /* Function uniqe scope ID before arguments */
    ctx->vars->offset = 0;

    ast_node_t* args_node = AST_create_node(NULL);
    if (!args_node) {
        AST_unload(node);
        return NULL;
    }

    _forward_token(curr, 1);
    while (!*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
        if ((*curr)->t_type == COMMA_TOKEN) {
            _forward_token(curr, 1);
            continue;
        }

        if (VRS_isdecl((*curr))) {
            ast_node_t* arg = _parse_variable_declaration(curr, ctx);
            if (!arg) {
                AST_unload(node);
                AST_unload(args_node);
                return NULL;
            }

            AST_add_node(args_node, arg);
        }
        else {
            _forward_token(curr, 1);
        }
    }

    AST_add_node(node, args_node);
    _forward_token(curr, 1);

    ast_node_t* body_node = _parse_scope(curr, ctx);
    if (!body_node) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, body_node);

    scope_elem_t el;
    scope_pop_top(&ctx->scope.stack, &el); /* Restore scope and offset */
    ctx->vars->offset = el.offset;

    return node;
}

/*
Arch:
type_token
|_ name (arch)
|_ decl_val (val | exp)
*/
static ast_node_t* _parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    _forward_token(curr, 1);
    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    if (
        VRS_one_slot(name_node->token) &&  /* Pointer or variable (String and array occupie several stack slots) */
        VRS_intext(node->token)            /* Global and RO variables placed not in stack */
    ) {
        int var_size = VRS_variable_bitness(name_node->token, 1) / 8;
        _var_update(node, ctx, name_node->token->value, var_size);
        _var_lookup(name_node, ctx);
    }

    _forward_token(curr, 1);
    AST_add_node(node, name_node);

    if (!*curr || (*curr)->t_type != ASSIGN_TOKEN) { /* If this is uninitilized variable */
        return node;
    }

    _forward_token(curr, 1);
    ast_node_t* value_node = _parse_expression(curr, ctx);
    if (!value_node) {
        AST_unload(node);
        return NULL;
    }
    
    if (
        node->token->t_type == STR_TYPE_TOKEN && 
        VRS_intext(node->token) /* Global and RO strings placed in .rodata or .data section */
    ) {
        _var_update(node, ctx, name_node->token->value, ALIGN(str_strlen(value_node->token->value)));
        ARM_add_info(name_node->token->value, scope_id_top(&ctx->scope.stack), 1, node->info.size, ctx->arrs);
        _var_lookup(name_node, ctx);
    }

    AST_add_node(node, value_node);
    return node;
}

/*
Arch:
arr_token
|_ size (int)
|_ el_size (int)
|_ name (str)
|_ element (val | exp)
|_ ...
*/
static ast_node_t* _parse_array_declaration(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    _forward_token(curr, 1);

    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    _forward_token(curr, 1);

    int array_size = 0;
    int el_size = 1;

    if ((*curr)->t_type == OPEN_INDEX_TOKEN) {
        _forward_token(curr, 1);

        array_size = str_atoi((*curr)->value);
        ast_node_t* size_node = AST_create_node(*curr);
        if (!size_node) {
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, size_node);
        _forward_token(curr, 2);

        el_size = VRS_variable_bitness(*curr, 1) / 8;
        ast_node_t* elem_size_node = AST_create_node(*curr);
        if (!elem_size_node) {
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, elem_size_node);
        _forward_token(curr, 3);
    }

    int act_size = 0;
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        _forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BLOCK_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                _forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = _parse_expression(curr, ctx);
            if (arg) AST_add_node(node, arg);
            array_size = MAX(array_size, ++act_size);
        }

        _forward_token(curr, 1);
    }
    
    ARM_add_info(name_node->token->value, scope_id_top(&ctx->scope.stack), el_size, array_size, ctx->arrs);
    if (VRS_intext(node->token)) {
        node->info.size = ALIGN(array_size * el_size);
        name_node->info.size = node->info.size;
        node->info.offset = VRM_add_info(
            name_node->token->value, node->info.size, scope_id_top(&ctx->scope.stack), ctx->vars
        );
    }

    return node;
}

/*
Arch:
operator_token
|_ left_var (val | exp)
|_ right_var (val | exp)
*/
static ast_node_t* _parse_binary_expression(token_t** curr, syntax_ctx_t* ctx, int min_priority) {
    ast_node_t* left = _parse_primary(curr, ctx);
    if (!left) return NULL;

    while (*curr) {
        int priority = VRS_token_priority(*curr);
        if (priority < min_priority || priority == -1) break;

        token_t* op_token = *curr;
        _forward_token(curr, 1);

        int next_min_priority = priority + 1;
        if ((*curr)->t_type == ASSIGN_TOKEN) {
            next_min_priority = priority;
        }

        ast_node_t* right = _parse_binary_expression(curr, ctx, next_min_priority);
        if (!right) {
            AST_unload(left);
            return NULL;
        }

        ast_node_t* op_node = AST_create_node(op_token);
        if (!op_node) {
            AST_unload(left);
            AST_unload(right);
            return NULL;
        }

        AST_add_node(op_node, left);
        AST_add_node(op_node, right);
        left = op_node;
    }

    return left;
}

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx) {
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        _forward_token(curr, 1);
        ast_node_t* node = _parse_binary_expression(curr, ctx, 0);
        if (!node || !*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            AST_unload(node);
            return NULL;
        }

        _forward_token(curr, 1);
        return node;
    }
    
    if (VRS_isptr(*curr))                      return _parse_array_expression(curr, ctx);
    else if ((*curr)->t_type == CALL_TOKEN)    return _parse_function_call(curr, ctx);
    else if ((*curr)->t_type == SYSCALL_TOKEN) return _parse_syscall(curr, ctx);

    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    _var_lookup(node, ctx);
    _forward_token(curr, 1);
    return node;
}

static ast_node_t* _parse_expression(token_t** curr, syntax_ctx_t* ctx) {
    return _parse_binary_expression(curr, ctx, 0);
}

/*
Arch (with indexing, assign):
assign_op
|_ arr_name (str)
|  |_ offset (val | exp)
|_ assign_val (val | exp)

Arch (without indexing, assign):
assign_op
|_ arr_name (str)
|_ assign_val (val | exp)

Arch (no assign):
name / 
name
|_ offset (val | exp)
*/
static ast_node_t* _parse_array_expression(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    _var_lookup(node, ctx);
    
    _forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_INDEX_TOKEN) { /* Indexing? */
        token_t* offset_token = (*curr)->next;
        ast_node_t* offset_exp = _parse_expression(&offset_token, ctx);
        if (!offset_exp) {
            AST_unload(node);
            return NULL;
        }

        while (*curr && (*curr)->t_type != CLOSE_INDEX_TOKEN) _forward_token(curr, 1);
        AST_add_node(node, offset_exp);
        _forward_token(curr, 1);
    }

    if (VRS_isclose(*curr)) { /* End of expression? */
        return node;
    }

    ast_node_t* opnode = AST_create_node(*curr);
    if (!opnode) {
        AST_unload(node);
        return NULL;
    }

    _forward_token(curr, 1);
    ast_node_t* right = _parse_expression(curr, ctx);
    if (!right) {
        AST_unload(node);
        AST_unload(opnode);
        return NULL;
    }

    AST_add_node(opnode, node);
    AST_add_node(opnode, right);
    return opnode;
}

/*
Arch:
switch_token
|_ stmt (val | exp)
|_ cases (scope)
   |_ case_stmt (val | exp | default)
      |_ case_body (scope)
         |_ ...
*/
static ast_node_t* _parse_switch_expression(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    _forward_token(curr, 1);
    ast_node_t* stmt = _parse_expression(curr, ctx);
    if (!stmt) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, stmt);
    ast_node_t* cases_scope = AST_create_node(NULL);
    if (!cases_scope) {
        AST_unload(node);
        return NULL;
    }

    _forward_token(curr, 1);
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        _forward_token(curr, 1);
        while ((*curr)->t_type == CASE_TOKEN || (*curr)->t_type == DEFAULT_TOKEN) {
            ast_node_t* case_stmt = NULL;
            if ((*curr)->t_type != CASE_TOKEN) {
                case_stmt = AST_create_node(TKN_create_token(DEFAULT_TOKEN, NULL, 0, 0));
            }
            else {
                _forward_token(curr, 1);
                case_stmt = _parse_expression(curr, ctx);
                case_stmt->token->t_type = CASE_TOKEN;
            }
            
            if (!case_stmt) {
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            _forward_token(curr, 1);
            ast_node_t* case_body = _parse_scope(curr, ctx);
            if (!case_body) {
                AST_unload(case_stmt);
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            case_stmt->token->glob = 0;
            case_stmt->token->ro   = 0;

            AST_add_node(case_stmt, case_body);
            AST_add_node(cases_scope, case_stmt);
        }
    }

    AST_add_node(node, cases_scope);
    _forward_token(curr, 1);
    return node;
}

/*
Arch:
cond_node_token
|_ condition (val | exp)
|_ true_branch (scope)
|  |_ ...
|_ false_branch (scope)
   |_ ...
*/
static ast_node_t* _parse_condition_scope(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    _forward_token(curr, 1);
    ast_node_t* cond = _parse_expression(curr, ctx);
    if (!cond) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    _forward_token(curr, 1);

    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) { /* Main branch scope */
        ast_node_t* branch = _parse_scope(curr, ctx);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) { /* Else branch scope */
        /* Separated scope for block */
        _forward_token(curr, 1);
        ast_node_t* branch = _parse_scope(curr, ctx);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}

/*
Arch:
syscall_token
|_ arg1 (val | exp)
|_ ...
*/
static ast_node_t* _parse_syscall(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    _forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        _forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                _forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = _parse_expression(curr, ctx);
            if (arg) AST_add_node(node, arg);
        }
    }

    return node;
}

syntax_ctx_t* STX_create_ctx() {
    syntax_ctx_t* ctx = (syntax_ctx_t*)mm_malloc(sizeof(syntax_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(syntax_ctx_t));
    return ctx;
}

int STX_destroy_ctx(syntax_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = _parse_block(&curr_head, ctx, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
