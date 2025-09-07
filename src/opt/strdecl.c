#include <strdecl.h>

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

static int _find_string(ast_node_t* root, stropt_ctx_t* ctx) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _find_string(t, ctx);
            continue;
        }

        if (
            VRS_isoperand(t->token) || 
            (VRS_isdecl(t->token) && (t->token->vinfo.ptr || !VRS_intext(t->token))) /* Is declaration and it's pointer or ntext */
        ) {
            _find_string(t, ctx);
            continue;
        }

        if (t->token->t_type == STRING_VALUE_TOKEN) {
            string_info_t info;
            if (_get_string(t->token->value, &info, ctx)) sprintf(t->token->value, "%s", info.name);
            else {
                _add_string(t->token->value, ctx);
                _get_string(t->token->value, &info, ctx);
                sprintf(t->token->value, "%s", info.name);
            }
            
            t->token->t_type = STR_VARIABLE_TOKEN;
            t->info.size     = VRS_variable_bitness(t->token, 1) / 8;
            t->token->vinfo.glob = 1;
            t->token->vinfo.ro   = 1;
        }

        if (
            t->token->t_type == FUNC_TOKEN || 
            t->token->t_type == CALL_TOKEN || 
            t->token->t_type == WHILE_TOKEN || 
            t->token->t_type == IF_TOKEN
        ) {
            _find_string(t, ctx);
        }
    }

    return 1;
}

static int _declare_strings(ast_node_t* root, stropt_ctx_t* ctx) {
    string_info_t* h = ctx->h;
    while (h) {
        ast_node_t* decl_root = AST_create_node(TKN_create_token(STR_TYPE_TOKEN, STR_VARIABLE, str_strlen(STR_VARIABLE), 0));
        if (!decl_root) return 0;

        ast_node_t* name_node = AST_create_node(TKN_create_token(STR_VARIABLE_TOKEN, h->name, str_strlen(h->name), 0));
        if (!name_node) {
            AST_unload(decl_root);
            return 0;
        }

        AST_add_node(decl_root, name_node);

        ast_node_t* value_node = AST_create_node(TKN_create_token(STRING_VALUE_TOKEN, h->body, str_strlen(h->body), 0));
        if (!value_node) {
            AST_unload(decl_root);
            return 0;
        }

        AST_add_node(decl_root, value_node);
        
        decl_root->token->vinfo.ro = 1;
        name_node->token->vinfo.ro = 1;
        name_node->info.size = VRS_variable_bitness(name_node->token, 1) / 8;

        ast_node_t* old = root->child;
        root->child = decl_root;
        decl_root->sibling = old;

        h = h->next;
    }

    return 1;
}

int OPT_strpack(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    stropt_ctx_t strctx = { .num = 0, .h = NULL };
    _find_string(ctx->r, &strctx);
    if (strctx.h) _declare_strings(ctx->r->child, &strctx);
    _unload_stringmap(&strctx);
    return 1;
}
