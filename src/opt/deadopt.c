#include <optimization.h>

typedef struct code_node {
    ast_node_t*       start;
    ast_node_t*       end;
    struct code_node* parent;
    struct code_node* child;
    struct code_node* sibling;
    int               is_reachable;
    int               is_function;
} code_node_t;

static code_node_t* _code_block_h = NULL;

typedef struct func_code_node {
    char                   name[TOKEN_MAX_SIZE];
    code_node_t*           head;
    struct func_code_node* next;
} func_code_node_t;

static func_code_node_t* _func_code_block_h = NULL;

static code_node_t* _create_code_node(ast_node_t* start) {
    if (!start) return NULL;
    code_node_t* node = (code_node_t*)mm_malloc(sizeof(code_node_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(code_node_t));
    node->start = start;
    return node;
}

static int _add_child_to_block(code_node_t* p, code_node_t* c) {
    if (!p || !c) return 0;
    c->parent = p;
    if (!p->child) p->child = c;
    else {
        code_node_t* sibling = p->child;
        while (sibling->sibling) sibling = sibling->sibling;
        sibling->sibling = c;
    }

    return 1;
}

static int _unload_code_block(code_node_t* node) {
    if (!node) return 0;
    _unload_code_block(node->child);
    _unload_code_block(node->sibling);
    mm_free(node);
    return 1;
}

static func_code_node_t* _create_func_code_block(char* name, code_node_t* head) {
    if (!name || !head) return NULL;
    func_code_node_t* node = (func_code_node_t*)mm_malloc(sizeof(func_code_node_t));
    if (!node) return NULL;

    str_strncpy(node->name, name, TOKEN_MAX_SIZE);
    node->head = head;
    node->next = NULL;
    return node;
}

static int _add_func_code_block(char* name, code_node_t* head) {
    func_code_node_t* node = _create_func_code_block(name, head);
    if (!node) return 0;

    if (!_func_code_block_h) {
        _func_code_block_h = node;
    }
    else {
        func_code_node_t* h = _func_code_block_h;
        while (h->next) h = h->next;
        h->next = node;
    }

    return 1;
}

static func_code_node_t* _find_func_block(char* name) {
    func_code_node_t* h = _func_code_block_h;
    while (h) {
        if (!str_strncmp(h->name, name, TOKEN_MAX_SIZE)) return h;
        h = h->next;
    }

    return NULL;
}

static int _unload_func_node_map() {
    while (_func_code_block_h) {
        func_code_node_t* n = _func_code_block_h->next;
        _unload_code_block(_func_code_block_h->head);
        mm_free(_func_code_block_h);
        _func_code_block_h = n;
    }

    return 1;
}


static code_node_t* _generate_blocks(ast_node_t* curr) {
    if (!curr) return NULL;
    code_node_t* head = _create_code_node(curr);
    if (!head) return NULL;
    
    /* We don't go deeper, and only catch changes in flow. */
    ast_node_t* curr_node = curr;
    for (ast_node_t* t = curr; t; t = t->sibling, curr_node = curr_node->sibling) {
        if (!t->token) continue;
        switch (t->token->t_type) {
            /* Generating block and linking to function name. */
            case FUNC_TOKEN:
                ast_node_t* name_node   = t->child;
                ast_node_t* params_node = name_node->sibling;
                ast_node_t* body_node   = params_node->sibling;
                code_node_t* func_blocks = _generate_blocks(body_node);
                if (func_blocks) {
                    _add_func_code_block((char*)name_node->token->value, func_blocks);
                }
            break;

            /* Flow go deeper without flow changing. */
            case SWITCH_TOKEN:
            case WHILE_TOKEN:
            case IF_TOKEN: break;
            case CALL_TOKEN:
                _add_child_to_block(head, _find_func_block((char*)t->token->value)->head);
            break;

            /* Flow kill point. */
            case RETURN_TOKEN:
            case EXIT_TOKEN: goto dead_flow;
            default: break;
        }
    }

dead_flow:
    head->end = curr_node;
    return head;
}

int deadcode_optimization(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    code_node_t* program = _generate_blocks(ctx->r->child);
    _unload_func_node_map();
    _unload_code_block(program);
    return 1;
}
