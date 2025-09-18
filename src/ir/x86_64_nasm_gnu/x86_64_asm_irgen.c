#include <ir/x86_64_gnu_nasm/x86_64_gnu_nasm_irgen.h>

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
        const char* amp  = str_strchr(line, '&');
        if (!amp) IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, line));
        else {
            // TODO:
            // iprintf(output, "%.*s", (int)(amp - line), line);

            // amp++;
            // int i = 0;
            // char varname[TOKEN_MAX_SIZE] = { 0 };
            // while (*amp && *amp != ',' && !str_isspace(*amp) && i < TOKEN_MAX_SIZE - 1) {
            //     varname[i++] = *amp++;
            // }

            // ast_node_t* var_node = _find_variable(node->child, varname);
            // if (var_node) iprintf(output, "%s", GET_ASMVAR(var_node));
            // iprintf(output, "%s\n", amp);
        }
    }

    return 1;
}