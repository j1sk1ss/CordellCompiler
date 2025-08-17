#include <optimization.h>

typedef struct string_info {
    char                body[TOKEN_MAX_SIZE];
    char                name[TOKEN_MAX_SIZE];
    struct string_info* next;
} string_info_t;

typedef struct {
    int            num;
    string_info_t* h;
} stropt_ctx_t;

static string_info_t* _create_string_info(const char* body, stropt_ctx_t* ctx) {
    string_info_t* node = (string_info_t*)mm_malloc(sizeof(string_info_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(string_info_t));
    str_strncpy(node->body, body, TOKEN_MAX_SIZE);
    sprintf(node->name, "str_%d", ctx->num++);
    return node;
}

static int _add_string(const char* body, stropt_ctx_t* ctx) {
    string_info_t* node = _create_string_info(body, ctx);
    if (!node) return 0;
    
    if (!ctx->h) {
        ctx->h = node;
        return 1;
    }

    string_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = node;
    return 1;
}

static int _get_string(const char* body, string_info_t* info, stropt_ctx_t* ctx) {
    string_info_t* h = ctx->h;
    while (h) {
        if (!str_strcmp((char*)h->body, body)) {
            if (info) str_memcpy(info, h, sizeof(string_info_t));
            return 1;
        }

        h = h->next;
    }

    return 0;
}

static int _unload_stringmap(stropt_ctx_t* ctx) {
    string_info_t* h = ctx->h;
    while (h) {
        string_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}


static int _find_string(tree_t* root, stropt_ctx_t* ctx) {
    if (!root) return 0;
    for (tree_t* t = root->first_child; t; t = t->next_sibling) {
        if (!t->token) {
            _find_string(t, ctx);
            continue;
        }

        switch (t->token->t_type) {
            case CALL_TOKEN:
            case SYSCALL_TOKEN:
            case RETURN_TOKEN:  
            case IF_TOKEN:      _find_string(t, ctx); continue;
            case DEFAULT_TOKEN:
            case CASE_TOKEN:    _find_string(t->first_child, ctx); continue;
            case SWITCH_TOKEN:  _find_string(t->first_child->next_sibling, ctx); continue;
            case WHILE_TOKEN:   _find_string(t->first_child->next_sibling, ctx); continue;
            case FUNC_TOKEN:    _find_string(t->first_child->next_sibling->next_sibling, ctx); continue;
            default: break;
        }
        
        if (t->token->t_type == STRING_VALUE_TOKEN) {
            string_info_t info;
            if (_get_string((char*)t->token->value, &info, ctx)) {
                sprintf((char*)t->token->value, "%s", info.name);
            }
            else {
                _add_string((char*)t->token->value, ctx);
                _get_string((char*)t->token->value, &info, ctx);
                sprintf((char*)t->token->value, "%s", info.name);
            }
            
            t->token->t_type = STR_VARIABLE_TOKEN;
            t->token->ro = 1;
        }
    }

    return 1;
}

static int _declare_strings(tree_t* root, stropt_ctx_t* ctx) {
    string_info_t* h = ctx->h;
    while (h) {
        tree_t* decl_root = STX_create_node(TKN_create_token(STR_TYPE_TOKEN, (unsigned char*)STR_VARIABLE, str_strlen(STR_VARIABLE), 0));
        if (!decl_root) return 0;

        tree_t* name_node = STX_create_node(TKN_create_token(STR_VARIABLE_TOKEN, (unsigned char*)h->name, str_strlen(h->name), 0));
        if (!name_node) {
            STX_unload(decl_root);
            return 0;
        }

        tree_t* value_node = STX_create_node(TKN_create_token(STRING_VALUE_TOKEN, (unsigned char*)h->body, str_strlen(h->body), 0));
        if (!value_node) {
            STX_unload(decl_root);
            STX_unload(name_node);
            return 0;
        }

        STX_add_node(decl_root, name_node);
        STX_add_node(decl_root, value_node);
        
        name_node->token->ro = 1;
        decl_root->token->ro = 1;

        tree_t* old = root->first_child;
        root->first_child = decl_root;
        decl_root->next_sibling = old;

        h = h->next;
    }

    return 1;
}

int OPT_strpack(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    tree_t* program_body = ctx->r->first_child;
    tree_t* prestart     = program_body;
    tree_t* main_body    = prestart->next_sibling;

    stropt_ctx_t strctx = { .num = 0, .h = NULL };
    _find_string(prestart, &strctx);
    _find_string(main_body, &strctx);
    if (strctx.h) _declare_strings(prestart, &strctx);
    _unload_stringmap(&strctx);

    return 1;
}
