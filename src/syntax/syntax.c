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

static tree_t* _parser_dummy(               token_t**, syntax_ctx_t*);
static tree_t* _parse_import(               token_t**, syntax_ctx_t*);
static tree_t* _parse_variable_declaration( token_t**, syntax_ctx_t*);
static tree_t* _parse_array_declaration(    token_t**, syntax_ctx_t*);
static tree_t* _parse_binary_expression(    token_t**, syntax_ctx_t*, int);
static tree_t* _parse_primary(              token_t**, syntax_ctx_t*);
static tree_t* _parse_function_call(        token_t**, syntax_ctx_t*);
static tree_t* _parse_function_declaration( token_t**, syntax_ctx_t*);
static tree_t* _parse_return_declaration(   token_t**, syntax_ctx_t*);
static tree_t* _parse_switch_expression(    token_t**, syntax_ctx_t*);
static tree_t* _parse_condition_scope(      token_t**, syntax_ctx_t*);
static tree_t* _parse_syscall(              token_t**, syntax_ctx_t*);
static tree_t* _parse_expression(           token_t**, syntax_ctx_t*);
static tree_t* _parse_array_expression(     token_t**, syntax_ctx_t*);

static tree_t* (*_get_parser(token_type_t t_type))(token_t**, syntax_ctx_t*) {
    switch (t_type) {
        case LONG_TYPE_TOKEN:
        case INT_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:
        case STR_TYPE_TOKEN:        return _parse_variable_declaration;
        case SWITCH_TOKEN:          return _parse_switch_expression;
        case WHILE_TOKEN:
        case IF_TOKEN:              return _parse_condition_scope;
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case UNKNOWN_STRING_TOKEN:  return _parse_expression;
        case SYSCALL_TOKEN:         return _parse_syscall;
        case IMPORT_SELECT_TOKEN:   return _parse_import;
        case ARRAY_TYPE_TOKEN:      return _parse_array_declaration;
        case CALL_TOKEN:            return _parse_function_call;
        case FUNC_TOKEN:            return _parse_function_declaration;
        case RETURN_TOKEN:          return _parse_return_declaration;
        default: return _parser_dummy;
    }
}

static int _fill_variable(tree_t* val_node, syntax_ctx_t* ctx) {
    if (val_node->next_sibling) _fill_variable(val_node->next_sibling, ctx);
    if (val_node->first_child)  _fill_variable(val_node->first_child, ctx);

    variable_info_t varinfo;
    if (!VRM_get_info((char*)val_node->token->value, ctx->fname, &varinfo, ctx->vars)) return 0;
    
    val_node->variable_offset = varinfo.offset;
    val_node->variable_size   = varinfo.size;
    return 1;
}

tree_t* STX_create_node(token_t* token) {
    tree_t* node = mm_malloc(sizeof(tree_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(tree_t));
    node->token = token;
    return node;
}

int STX_add_node(tree_t* parent, tree_t* child) {
    if (!parent || !child) return 0;
    child->parent = parent;
    if (!parent->first_child) parent->first_child = child;
    else {
        tree_t* sibling = parent->first_child;
        while (sibling->next_sibling) sibling = sibling->next_sibling;
        sibling->next_sibling = child;
    }

    return 1;
}

int STX_remove_node(tree_t* parent, tree_t* child) {
    if (!parent || !child) return 0;

    tree_t* prev = NULL;
    tree_t* current = parent->first_child;
    while (current) {
        if (current == child) {
            if (prev) prev->next_sibling = current->next_sibling;
            else parent->first_child = current->next_sibling;
            break;
        }

        prev = current;
        current = current->next_sibling;
    }

    return 1;
}

static tree_t* _parser_dummy(token_t** curr, syntax_ctx_t* ctx) { return NULL; }

/*
Main parse function, that parse whole scope.
For parsing we have registered parsers for every token.

Arch:
scope
|_ exp1 (val | exp)
|_ exp2 (val | exp)
|_ ...
*/
static tree_t* _parse_scope(token_t** curr, syntax_ctx_t* ctx, token_type_t exit_token) {
    if (!curr || !*curr) return NULL;
    tree_t* scope_node = STX_create_node(NULL);
    if (!scope_node) return NULL;
    
    while ((*curr) && (*curr)->t_type != exit_token) {
        tree_t* node = _get_parser((*curr)->t_type)(curr, ctx);
        if (!node) *curr = (*curr)->next;
        else STX_add_node(scope_node, node);
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
static tree_t* _parse_import(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr || (*curr)->t_type != IMPORT_SELECT_TOKEN) return NULL;
    tree_t* import_node = STX_create_node(*curr);
    if (!import_node) return NULL;
    
    /* "from" statement -> "source path" */
    *curr = (*curr)->next;
    tree_t* source_node = STX_create_node(*curr); // example: src.cpl
    if (!source_node) {
        STX_unload(import_node);
        return NULL;
    }

    /* Skip "import" statement to list of functions */
    *curr = (*curr)->next->next;
    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        tree_t* fname = STX_create_node(*curr); // example: func1
        if (!fname) {
            STX_unload(import_node);
            STX_unload(source_node);
            return NULL;
        }

        STX_add_node(source_node, fname);
        *curr = (*curr)->next;
    }

    STX_add_node(import_node, source_node);
    return import_node;
}

/*
func1 arg1 arg2;
"func1" - call_node.
And then adding to scope arg1 and arg2.
Example: name(arg1, arg2, arg3, ...);

Arch:
call_token
|_ scope
   |_ arg1 (val | exp)
   |_ ...
*/
static tree_t* _parse_function_call(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr || (*curr)->t_type != CALL_TOKEN) return NULL;
    tree_t* call_node = STX_create_node(*curr);
    if (!call_node) return NULL;
    
    tree_t* args_node = STX_create_node(NULL);
    if (!args_node) {
        STX_unload(call_node);
        return NULL;
    }

    *curr = (*curr)->next;
    if (*curr && (*curr)->t_type == OPEN_BRACKET_TOKEN) {

        *curr = (*curr)->next;
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) { /* Skip comma */
                *curr = (*curr)->next;
                continue;
            }

            tree_t* arg = _parse_expression(curr, ctx);
            if (arg) STX_add_node(args_node, arg); /* Parse expressions between commas. Example a + b, c + y, ... */
        }
    }

    STX_add_node(call_node, args_node);
    return call_node;
}

/*
Parser for "return <exp>;" expression.
Same as for "end <exp>;".
Arch:
return_token
|_ ret_val (val | exp)
*/
static tree_t* _parse_return_declaration(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr || (*curr)->t_type != RETURN_TOKEN) return NULL;
    tree_t* return_node = STX_create_node(*curr);
    if (!return_node) return NULL;
    
    *curr = (*curr)->next;
    tree_t* exp_node = _parse_expression(curr, ctx);
    if (!exp_node) {
        STX_unload(return_node);
        return NULL;
    }

    STX_add_node(return_node, exp_node);
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
static tree_t* _parse_function_declaration(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr || (*curr)->t_type != FUNC_TOKEN) return NULL;
    tree_t* func_node = STX_create_node(*curr);
    if (!func_node) return NULL;
    
    *curr = (*curr)->next;
    tree_t* name_node = STX_create_node(*curr);
    if (!name_node) {
        STX_unload(func_node);
        return NULL;
    }

    STX_add_node(func_node, name_node);

    *curr = (*curr)->next;
    tree_t* args_node = STX_create_node(NULL);
    if (!args_node) {
        STX_unload(func_node);
        STX_unload(name_node);
        return NULL;
    }

    int temp_off = ctx->vars->offset;
    const char* temp_fname = ctx->fname;
    ctx->vars->offset = 0;
    ctx->fname = (char*)name_node->token->value;

    while (!*curr || (*curr)->t_type != OPEN_BLOCK_TOKEN) {
        if (VRS_isdecl((*curr))) {
            tree_t* param_node = _parse_variable_declaration(curr, ctx);
            if (!param_node) {
                STX_unload(func_node);
                STX_unload(name_node);
                STX_unload(args_node);
                return NULL;
            }

            STX_add_node(args_node, param_node);
        }

        *curr = (*curr)->next;
    }

    STX_add_node(func_node, args_node);
    tree_t* body_node = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
    if (!body_node) {
        STX_unload(func_node);
        STX_unload(name_node);
        STX_unload(args_node);
        return NULL;
    }

    (*curr) = (*curr)->next;
    STX_add_node(func_node, body_node);
    ctx->fname = temp_fname;
    ctx->vars->offset = temp_off;
    return func_node;
}

/*
Arch:
type_token
|_ name (arch)
|_ decl_val (val | exp)
*/
static tree_t* _parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx) {
    token_t* type_token = *curr;
    token_t* name_token = type_token->next;
    if (!type_token || !name_token) return NULL;
    
    token_t* assign_token = name_token->next;
    if (!assign_token) return NULL;
    token_t* value_token = assign_token->next;
    if (!value_token) return NULL;
    tree_t* decl_node = STX_create_node(type_token);
    if (!decl_node) return NULL;
    
    tree_t* name_node = STX_create_node(name_token);
    if (!name_node) {
        STX_unload(decl_node);
        return NULL;
    }
    
    if (
        VRS_one_slot(name_token) &&  /* Pointer or variable (String and array occupie several stack slots) */
        VRS_intext(decl_node->token) /* Global and RO variables placed not in stack */
    ) {
        int var_size = VRS_variable_bitness(name_token, 1) / 8;
        decl_node->variable_offset = VRM_add_info((char*)name_node->token->value, var_size, ctx->fname, ctx->vars);
        decl_node->variable_size   = var_size;
        _fill_variable(name_node, ctx);
    }

    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        (*curr) = (*curr)->next;
    }

    STX_add_node(decl_node, name_node);
    if (!assign_token || assign_token->t_type != ASSIGN_TOKEN) {
        return decl_node;
    }

    tree_t* value_node = _parse_expression(&value_token, ctx);
    if (!value_node) {
        STX_unload(decl_node);
        STX_unload(name_node);
        return NULL;
    }

    if (
        type_token->t_type == STR_TYPE_TOKEN && 
        VRS_intext(decl_node->token) /* Global and RO strings placed in .rodata or .data section */
    ) {
        decl_node->variable_size   = ALIGN(str_strlen((char*)value_node->token->value));
        decl_node->variable_offset = VRM_add_info((char*)name_node->token->value, decl_node->variable_size, ctx->fname, ctx->vars);
        ARM_add_info((char*)name_node->token->value, ctx->fname, 1, decl_node->variable_size, ctx->arrs);
        _fill_variable(name_node, ctx);
    }

    STX_add_node(decl_node, value_node);
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
static tree_t* _parse_array_declaration(token_t** curr, syntax_ctx_t* ctx) {
    token_t* arr_token       = *curr;
    token_t* name_token      = arr_token->next;
    token_t* size_token      = name_token->next;
    token_t* elem_size_token = size_token->next;
    token_t* assign_token    = elem_size_token->next;
    
    if (
        !size_token || !elem_size_token || !name_token || !assign_token ||
        size_token->t_type != UNKNOWN_NUMERIC_TOKEN || 
        name_token->t_type != ARR_VARIABLE_TOKEN ||
        assign_token->t_type != ASSIGN_TOKEN
    ) {
        return NULL;
    }
    
    tree_t* arr_node  = STX_create_node(arr_token);
    if (!arr_node) return NULL;
    
    int array_size = str_atoi((char*)size_token->value);
    tree_t* size_node = STX_create_node(size_token);
    if (!size_node) {
        STX_unload(arr_node);
        return NULL;
    }
    
    int el_size = 1;
    switch (elem_size_token->t_type) {
        case SHORT_TYPE_TOKEN: el_size = 2; break;
        case INT_TYPE_TOKEN:   el_size = 4; break;
        case LONG_TYPE_TOKEN:  el_size = 8; break;
        default: break;
    }
    
    tree_t* elem_size_node = STX_create_node(elem_size_token);
    if (!elem_size_node) {
        STX_unload(arr_node);
        STX_unload(size_node);
        return NULL;
    }

    tree_t* name_node = STX_create_node(name_token);
    if (!name_node) {
        STX_unload(arr_node);
        STX_unload(size_node);
        STX_unload(name_node);
        return NULL;
    }
    
    STX_add_node(arr_node, size_node);
    STX_add_node(arr_node, elem_size_node);
    STX_add_node(arr_node, name_node);

    int arr_size = 0;
    token_t* val_token = assign_token->next;
    if (val_token && val_token->t_type == OPEN_BLOCK_TOKEN) {
        val_token = val_token->next;
        while (val_token && val_token->t_type != CLOSE_BLOCK_TOKEN) {
            if (val_token->t_type == COMMA_TOKEN) {
                val_token = val_token->next;
                continue;
            }

            tree_t* arg = _parse_expression(&val_token, ctx);
            if (arg) STX_add_node(arr_node, arg);
            array_size = MAX(array_size, ++arr_size);
        }

        val_token = val_token->next;
    }
    
    ARM_add_info((char*)name_token->value, ctx->fname, el_size, array_size, ctx->arrs);
    if (!arr_token->ro && !arr_token->glob) {
        arr_node->variable_size   = ALIGN(array_size * el_size);
        name_node->variable_size  = arr_node->variable_size;
        arr_node->variable_offset = VRM_add_info((char*)name_token->value, arr_node->variable_size, ctx->fname, ctx->vars);
    }

    *curr = val_token;
    return arr_node;
}

/*
Arch:
operator_token
|_ left_var (val | exp)
|_ right_var (val | exp)
*/
static tree_t* _parse_binary_expression(token_t** curr, syntax_ctx_t* ctx, int min_priority) {
    tree_t* left = _parse_primary(curr, ctx);
    if (!left) return NULL;

    while (*curr) {
        int priority = VRS_token_priority(*curr);
        if (priority < min_priority || priority == -1) break;

        token_t* op_token = *curr;
        *curr = (*curr)->next;

        int next_min_priority = priority + 1;
        if ((*curr)->t_type == ASSIGN_TOKEN) next_min_priority = priority;

        tree_t* right = _parse_binary_expression(curr, ctx, next_min_priority);
        if (!right) {
            STX_unload(left);
            return NULL;
        }

        tree_t* op_node = STX_create_node(op_token);
        if (!op_node) {
            STX_unload(left);
            STX_unload(right);
            return NULL;
        }

        STX_add_node(op_node, left);
        STX_add_node(op_node, right);
        left = op_node;
    }

    return left;
}

static tree_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr) return NULL;
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        *curr = (*curr)->next;
        tree_t* node = _parse_binary_expression(curr, ctx, 0);
        if (!node || !*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            STX_unload(node);
            return NULL;
        }

        *curr = (*curr)->next;
        return node;
    }

    if (
        (*curr)->t_type == ARR_VARIABLE_TOKEN || 
        (*curr)->t_type == STR_VARIABLE_TOKEN || 
        (*curr)->ptr
    )                                           return _parse_array_expression(curr, ctx);
    else if ((*curr)->t_type == CALL_TOKEN)     return _parse_function_call(curr, ctx);
    else if ((*curr)->t_type == SYSCALL_TOKEN)  return _parse_syscall(curr, ctx);

    tree_t* node = STX_create_node(*curr);
    _fill_variable(node, ctx);
    *curr = (*curr)->next;
    return node;
}

static tree_t* _parse_expression(token_t** curr, syntax_ctx_t* ctx) {
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
static tree_t* _parse_array_expression(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr) return NULL;
    tree_t* arr_name_node = STX_create_node(*curr);
    if (!arr_name_node) return NULL;
    _fill_variable(arr_name_node, ctx);
    
    *curr = (*curr)->next;
    if ((*curr)->t_type == OPEN_INDEX_TOKEN) {
        token_t* offset_token = (*curr)->next;
        tree_t* offset_exp = _parse_expression(&offset_token, ctx);
        if (!offset_exp) {
            STX_unload(arr_name_node);
            return NULL;
        }

        while (*curr && (*curr)->t_type != CLOSE_INDEX_TOKEN) (*curr) = (*curr)->next;
        STX_add_node(arr_name_node, offset_exp);
        *curr = (*curr)->next;
    }

    token_t* assign_token = *curr;
    if (!assign_token || assign_token->t_type == DELIMITER_TOKEN) {
        return arr_name_node;
    }

    tree_t* assign_node = STX_create_node(assign_token);
    if (!assign_node) {
        STX_unload(arr_name_node);
        return NULL;
    }

    *curr = (*curr)->next;
    tree_t* right = _parse_expression(curr, ctx);
    if (!right) {
        STX_unload(arr_name_node);
        STX_unload(assign_node);
        return NULL;
    }

    STX_add_node(assign_node, arr_name_node);
    STX_add_node(assign_node, right);
    return assign_node;
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
static tree_t* _parse_switch_expression(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr) return NULL;
    tree_t* body_node = STX_create_node(*curr);
    if (!body_node) return NULL;

    *curr = (*curr)->next;
    tree_t* stmt = _parse_expression(curr, ctx);
    if (!stmt) {
        STX_unload(body_node);
        return NULL;
    }

    STX_add_node(body_node, stmt);
    tree_t* cases_scope =  STX_create_node(NULL);
    if (!cases_scope) {
        STX_unload(body_node);
        return NULL;
    }

    *curr = (*curr)->next;
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        *curr = (*curr)->next;
        while ((*curr)->t_type == CASE_TOKEN || (*curr)->t_type == DEFAULT_TOKEN) {
            tree_t* case_stmt = NULL;
            if ((*curr)->t_type == CASE_TOKEN) {
                *curr = (*curr)->next;
                case_stmt = _parse_expression(curr, ctx);
                case_stmt->token->t_type = CASE_TOKEN;
            }
            else {
                *curr = (*curr)->next;
                case_stmt = STX_create_node(TKN_create_token(DEFAULT_TOKEN, NULL, 0, 0));
            }
            
            if (!case_stmt) {
                STX_unload(body_node);
                return NULL;
            }

            *curr = (*curr)->next;
            tree_t* case_body = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
            if (!case_body) {
                STX_unload(case_stmt);
                STX_unload(body_node);
                return NULL;
            }

            case_stmt->token->glob = 0;
            case_stmt->token->ro   = 0;

            STX_add_node(case_stmt, case_body);
            STX_add_node(cases_scope, case_stmt);
            *curr = (*curr)->next;
        }
    }

    *curr = (*curr)->next;
    STX_add_node(body_node, cases_scope);
    return body_node;
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
static tree_t* _parse_condition_scope(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr) return NULL;
    tree_t* body_node = STX_create_node(*curr);
    if (!body_node) return NULL;
    
    *curr = (*curr)->next;
    tree_t* cond = _parse_expression(curr, ctx);
    if (!cond) {
        STX_unload(body_node);
        return NULL;
    }

    STX_add_node(body_node, cond);

    *curr = (*curr)->next;
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        *curr = (*curr)->next;
        tree_t* true_branch = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
        if (!true_branch) {
            STX_unload(body_node);
            return NULL;
        }

        STX_add_node(body_node, true_branch);
        if (*curr && (*curr)->t_type == CLOSE_BLOCK_TOKEN) *curr = (*curr)->next;
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) {
        *curr = (*curr)->next->next;
        tree_t* false_branch = _parse_scope(curr, ctx, CLOSE_BLOCK_TOKEN);
        if (!false_branch) {
            STX_unload(body_node);
        }

        STX_add_node(body_node, false_branch);
        if (*curr && (*curr)->t_type == CLOSE_BLOCK_TOKEN) *curr = (*curr)->next;
    }
    
    return body_node;
}

/*
Arch:
syscall_token
|_ arg1 (val | exp)
|_ ...
*/
static tree_t* _parse_syscall(token_t** curr, syntax_ctx_t* ctx) {
    if (!curr || !*curr || (*curr)->t_type != SYSCALL_TOKEN) return NULL;
    tree_t* syscall_node = STX_create_node(*curr);
    if (!syscall_node) return NULL;

    *curr = (*curr)->next;
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        *curr = (*curr)->next;
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                *curr = (*curr)->next;
                continue;
            }

            tree_t* arg = _parse_expression(curr, ctx);
            if (arg) STX_add_node(syscall_node, arg);
        }
    }

    return syscall_node;
}

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = STX_create_node(NULL);
    if (!ctx->r) return 0;

    /* Parse all code before start keyword. It can be import, defines, functions and constants */
    tree_t* prefix_node = _parse_scope(&curr_head, ctx, START_TOKEN);
    if (!prefix_node) {
        print_error("Prefix code parse error! Head=%p, head_type=%i", head, head->t_type);
        STX_unload(ctx->r);
        return 0;
    }

    /* Parse main program body */
    tree_t* main_node = _parse_scope(&curr_head, ctx, EXIT_TOKEN);
    if (!main_node) print_warn("Program without body. Is it correct behaviour?");
    else {
        tree_t* exit_node = STX_create_node(TKN_create_token(EXIT_TOKEN, NULL, 0, curr_head->line_number));
        if (!exit_node) {
            print_error("Exit node parse error! Maybe memory manager hit it's memlim? curr_head=%p", curr_head);
            STX_unload(prefix_node);
            STX_unload(ctx->r);
            return 0;
        }

        /* Parse expression from end <exp> */
        token_t* exit_token = curr_head->next;
        tree_t* exit_exp = _parse_expression(&exit_token, ctx);
        if (!exit_exp) {
            print_error("Exit expression parse error!");
            STX_unload(exit_node);
            STX_unload(prefix_node);
            STX_unload(ctx->r);
            return 0;
        }

        /* Add exit expression to exit node, exit node to main node */
        STX_add_node(exit_node, exit_exp);
        STX_add_node(main_node, exit_node);
    }

    /* Complete parsing via linking prefix and main nodes to root */
    STX_add_node(ctx->r, prefix_node);
    STX_add_node(ctx->r, main_node);
    return 1;
}

int STX_unload(tree_t* node) {
    if (!node) return 0;
    STX_unload(node->first_child);
    STX_unload(node->next_sibling);
    mm_free(node);
    return 1;
}
