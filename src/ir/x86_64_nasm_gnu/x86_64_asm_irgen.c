#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

static ast_node_t* _find_variable(ast_node_t* n, const char* name) {
    for (ast_node_t* t = n; t && t->token; t = t->sibling) {
        if (!str_strncmp(t->token->value, name, TOKEN_MAX_SIZE)) {
            return t;
        }
    }

    return NULL;
}

int IR_generate_asmblock(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node) return 0;
    ast_node_t* h = node->child;
    for (; h->token; h = h->sibling);

    for (ast_node_t* l = h->child; l; l = l->sibling) {
        const char* line = l->token->value;
        char buf[1024] = { 0 };
        char* out = buf;
        const char* p = line;

        while (*p && (out - buf) < (int)sizeof(buf) - 1) {
            if (*p == '{') {
                p++;
                char varname[TOKEN_MAX_SIZE] = { 0 };
                int i = 0;
                while (*p && *p != '}' && i < TOKEN_MAX_SIZE - 1) {
                    varname[i++] = *p++;
                }

                ast_node_t* var_node = _find_variable(node->child, varname);
                if (var_node) {
                    const char* asmvar = GET_ASMVAR(var_node);
                    size_t len = str_strlen(asmvar);
                    if ((out - buf) + len < sizeof(buf) - 1) {
                        str_memcpy(out, asmvar, len);
                        out += len;
                    }
                } 
                else {
                    size_t len = str_strlen(varname);
                    if ((out - buf) + len < sizeof(buf) - 1) {
                        str_memcpy(out, varname, len);
                        out += len;
                    }
                }
            } 
            else {
                *out++ = *p++;
            }
        }

        *out = 0;
        IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, buf));
    }

    return 1;
}
