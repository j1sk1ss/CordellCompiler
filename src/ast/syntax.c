#include <syntax.h>

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

static inline int _forward_token(token_t** tkn, int steps) {
    while (steps-- > 0) *tkn = (*tkn)->next;
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

static ast_node_t* _dummy_parser(token_t** curr, syntax_ctx_t* ctx) { return NULL; }

static ast_node_t* (*_get_parser(token_type_t t_type))(token_t**, syntax_ctx_t*) {
    switch (t_type) {
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
        case UNKNOWN_STRING_TOKEN:  return _parse_expression;
        case SYSCALL_TOKEN:         return _parse_syscall;
        case IMPORT_SELECT_TOKEN:   return _parse_import;
        case ARRAY_TYPE_TOKEN:      return _parse_array_declaration;
        case CALL_TOKEN:            return _parse_function_call;
        case FUNC_TOKEN:            return _parse_function_declaration;
        case RETURN_TOKEN:          return _parse_return_declaration;
        default:                    return _dummy_parser;
    }
}

static int _fill_variable(ast_node_t* val_node, syntax_ctx_t* ctx) {
    if (val_node->next_sibling) _fill_variable(val_node->next_sibling, ctx);
    if (val_node->first_child)  _fill_variable(val_node->first_child, ctx);

    variable_info_t varinfo;
    if (!VRM_get_info((char*)val_node->token->value, ctx->scope, &varinfo, ctx->vars)) {
        return 0;
    }
    
    val_node->variable_offset = varinfo.offset;
    val_node->variable_size   = varinfo.size;
    return 1;
}

/*
Main parse function, that parse whole scope.
For parsing we have registered parsers for every token.

Arch:
scope
|_ exp1 (val | exp)
|_ exp2 (val | exp)
|_ ...
*/
static ast_node_t* _parse_scope(token_t** curr, syntax_ctx_t* ctx, token_type_t exit_token) {
    if (!curr || !*curr) return NULL;
    ast_node_t* scope_node = AST_create_node(NULL);
    if (!scope_node) return NULL;
    
    while ((*curr) && (*curr)->t_type != exit_token) {
        ast_node_t* node = _get_parser((*curr)->t_type)(curr, ctx);
        if (!node) _forward_token(curr, 1);
        else AST_add_node(scope_node, node);
    }
    
    return scope_node;
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
    if (!curr || !*curr || (*curr)->t_type != IMPORT_SELECT_TOKEN) return NULL;
    ast_node_t* import_node = AST_create_node(*curr);
    if (!import_node) return NULL;
    
    /* "from" statement -> "source path" */
    _forward_token(curr, 1);
    ast_node_t* source_node = AST_create_node(*curr); // example: src.cpl
    if (!source_node) {
        AST_unload(import_node);
        return NULL;
    }

    /* Skip "import" statement to list of functions */
    _forward_token(curr, 2);
    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        ast_node_t* fname = AST_create_node(*curr); // example: func1
        if (!fname) {
            AST_unload(import_node);
            AST_unload(source_node);
            return NULL;
        }

        AST_add_node(source_node, fname);
        _forward_token(curr, 1);
    }

    AST_add_node(import_node, source_node);
    return import_node;
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
    if (!curr || !*curr || (*curr)->t_type != RETURN_TOKEN) return NULL;
    ast_node_t* return_node = AST_create_node(*curr);
    if (!return_node) return NULL;
    
    _forward_token(curr, 1);
    ast_node_t* exp_node = _parse_expression(curr, ctx);
    if (!exp_node) {
        AST_unload(return_node);
        return NULL;
    }

    AST_add_node(return_node, exp_node);
    return return_node;
}

/*
Arch:
func_token
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
    if (!curr || !*curr || (*curr)->t_type != FUNC_TOKEN) return NULL;
    ast_node_t* func_node = AST_create_node(*curr);
    if (!func_node) return NULL;
    
    _forward_token(curr, 1);
    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(func_node);
        return NULL;
    }

    AST_add_node(func_node, name_node);

    _forward_token(curr, 1);
    ast_node_t* args_node = AST_create_node(NULL);
    if (!args_node) {
        AST_unload(func_node);
        return NULL;
    }

    int outer_off = ctx->vars->offset;
    const char* outer_scope = ctx->scope;
    ctx->vars->offset = 0;
    ctx->scope = name_node->token->value;

    while (!*curr || (*curr)->t_type != OPEN_BLOCK_TOKEN) {
        if (VRS_isdecl((*curr))) {
            ast_node_t* param_node = _parse_variable_declaration(curr, ctx);
            if (!param_node) {
                AST_unload(func_node);
                AST_unload(args_node);
                return NULL;
            }

            AST_add_node(args_node, param_node);
        }

        _forward_token(curr, 1);
    }

    AST_add_node(func_node, args_node);
    ast_node_t* body_node = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
    if (!body_node) {
        AST_unload(func_node);
        return NULL;
    }

    _forward_token(curr, 1);
    AST_add_node(func_node, body_node);

    ctx->scope = outer_scope;
    ctx->vars->offset = outer_off;
    return func_node;
}

/*
Arch:
type_token
|_ name (arch)
|_ decl_val (val | exp)
*/
static ast_node_t* _parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx) {
    token_t* type_token = *curr;
    token_t* name_token = type_token->next;
    if (!type_token || !name_token) return NULL;
    
    token_t* assign_token = name_token->next;
    if (!assign_token) return NULL;
    token_t* value_token = assign_token->next;
    if (!value_token) return NULL;
    ast_node_t* decl_node = AST_create_node(type_token);
    if (!decl_node) return NULL;
    
    ast_node_t* name_node = AST_create_node(name_token);
    if (!name_node) {
        AST_unload(decl_node);
        return NULL;
    }
    
    if (
        VRS_one_slot(name_token) &&  /* Pointer or variable (String and array occupie several stack slots) */
        VRS_intext(decl_node->token) /* Global and RO variables placed not in stack */
    ) {
        int var_size = VRS_variable_bitness(name_token, 1) / 8;
        decl_node->variable_offset = VRM_add_info((char*)name_node->token->value, var_size, ctx->scope, ctx->vars);
        decl_node->variable_size   = var_size;
        _fill_variable(name_node, ctx);
    }

    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        _forward_token(curr, 1);
    }

    AST_add_node(decl_node, name_node);
    if (!assign_token || assign_token->t_type != ASSIGN_TOKEN) {
        return decl_node;
    }

    ast_node_t* value_node = _parse_expression(&value_token, ctx);
    if (!value_node) {
        AST_unload(decl_node);
        AST_unload(name_node);
        return NULL;
    }

    if (
        type_token->t_type == STR_TYPE_TOKEN && 
        VRS_intext(decl_node->token) /* Global and RO strings placed in .rodata or .data section */
    ) {
        decl_node->variable_size   = ALIGN(str_strlen((char*)value_node->token->value));
        decl_node->variable_offset = VRM_add_info((char*)name_node->token->value, decl_node->variable_size, ctx->scope, ctx->vars);
        ARM_add_info((char*)name_node->token->value, ctx->scope, 1, decl_node->variable_size, ctx->arrs);
        _fill_variable(name_node, ctx);
    }

    AST_add_node(decl_node, value_node);
    return decl_node;
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
    
    int array_size = str_atoi((*curr)->value);
    ast_node_t* size_node = AST_create_node(*curr);
    if (!size_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, size_node);
    _forward_token(curr, 1);

    int el_size = VRS_variable_bitness(*curr, 1) / 8;
    ast_node_t* elem_size_node = AST_create_node(*curr);
    if (!elem_size_node) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, elem_size_node);
    _forward_token(curr, 1);

    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    _forward_token(curr, 2); /* Skip assign token */

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
    
    ARM_add_info(name_node->token->value, ctx->scope, el_size, array_size, ctx->arrs);
    if (VRS_intext(node->token)) {
        node->variable_size = ALIGN(array_size * el_size);
        name_node->variable_size = node->variable_size;
        node->variable_offset = VRM_add_info(name_node->token->value, node->variable_size, ctx->scope, ctx->vars);
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
        if ((*curr)->t_type == ASSIGN_TOKEN) next_min_priority = priority;

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

    if (
        (*curr)->t_type == ARR_VARIABLE_TOKEN || 
        (*curr)->t_type == STR_VARIABLE_TOKEN || 
        (*curr)->ptr
    )                                          return _parse_array_expression(curr, ctx);
    else if ((*curr)->t_type == CALL_TOKEN)    return _parse_function_call(curr, ctx);
    else if ((*curr)->t_type == SYSCALL_TOKEN) return _parse_syscall(curr, ctx);

    ast_node_t* node = AST_create_node(*curr);
    _fill_variable(node, ctx);
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
    _fill_variable(node, ctx);
    
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
            _forward_token(curr, 1);

            ast_node_t* case_stmt = NULL;
            if ((*curr)->t_type != CASE_TOKEN) case_stmt = AST_create_node(TKN_create_token(DEFAULT_TOKEN, NULL, 0, 0));
            else {
                case_stmt = _parse_expression(curr, ctx);
                case_stmt->token->t_type = CASE_TOKEN;
            }
            
            if (!case_stmt) {
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            _forward_token(curr, 1);
            ast_node_t* case_body = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
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
            _forward_token(curr, 1);
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
        _forward_token(curr, 1);
        ast_node_t* branch = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, branch);
        if (*curr && (*curr)->t_type == CLOSE_BLOCK_TOKEN) _forward_token(curr, 1);
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) { /* Else branch scope */
        _forward_token(curr, 2);
        ast_node_t* branch = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, branch);
        if (*curr && (*curr)->t_type == CLOSE_BLOCK_TOKEN) _forward_token(curr, 1);
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

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = AST_create_node(NULL);
    if (!ctx->r) return 0;

    /* Parse all code before start keyword. It can be import, defines, functions and constants */
    ast_node_t* prefix_node = _parse_scope(&curr_head, ctx, START_TOKEN);
    if (!prefix_node) {
        print_error("Prefix code parse error! Head=%p, head_type=%i", head, head->t_type);
        AST_unload(ctx->r);
        return 0;
    }

    /* Parse main program body */
    ast_node_t* main_node = _parse_scope(&curr_head, ctx, EXIT_TOKEN);
    if (!main_node) print_warn("Program without body. Is it correct behaviour?");
    else {
        ast_node_t* exit_node = AST_create_node(TKN_create_token(EXIT_TOKEN, NULL, 0, curr_head->lnum));
        if (!exit_node) {
            print_error("Exit node parse error! Maybe memory manager hit it's memlim? curr_head=%p", curr_head);
            AST_unload(prefix_node);
            AST_unload(ctx->r);
            return 0;
        }

        /* Parse expression from end <exp> */
        token_t* exit_token = curr_head->next;
        ast_node_t* exit_exp = _parse_expression(&exit_token, ctx);
        if (!exit_exp) {
            print_error("Exit expression parse error!");
            AST_unload(exit_node);
            AST_unload(prefix_node);
            AST_unload(ctx->r);
            return 0;
        }

        /* Add exit expression to exit node, exit node to main node */
        AST_add_node(exit_node, exit_exp);
        AST_add_node(main_node, exit_node);
    }

    /* Complete parsing via linking prefix and main nodes to root */
    AST_add_node(ctx->r, prefix_node);
    AST_add_node(ctx->r, main_node);
    return 1;
}