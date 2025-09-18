#include <gen/x86_64_nasm_gnu/x86_64_gnu_nasm.h>

static ast_node_t* _find_variable(ast_node_t* n, const char* name) {
    for (ast_node_t* t = n; t && t->token; t = t->sibling) {
        if (!str_strncmp(t->token->value, name, TOKEN_MAX_SIZE)) {
            return t;
        }
    }

    return NULL;
}

int x86_64_generate_asm(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node) return 0;
    ast_node_t* h = node->child;
    for (; h->token; h = h->sibling);

    for (ast_node_t* l = h->child; l; l = l->sibling) {
        const char* line = l->token->value;
        const char* amp  = str_strchr(line, '&');
        if (!amp) iprintf(output, "%s\n", line);
        else {
            iprintf(output, "%.*s", (int)(amp - line), line);

            amp++;
            int i = 0;
            char varname[TOKEN_MAX_SIZE] = { 0 };
            while (*amp && *amp != ',' && !str_isspace(*amp) && i < TOKEN_MAX_SIZE - 1) {
                varname[i++] = *amp++;
            }

            ast_node_t* var_node = _find_variable(node->child, varname);
            if (var_node) iprintf(output, "%s", GET_ASMVAR(var_node));
            iprintf(output, "%s\n", amp);
        }
    }

    return 1;
}