#ifndef ASTHELPER_H_
#define ASTHELPER_H_
#include <ast/astgen.h>
const char* fmt_tkn_type(token_t* t) {
    if (!t) return "";
    if (t->flags.ptr) {
        switch (t->t_type) {
            case I0_TYPE_TOKEN:  return "i0*";
            case I8_TYPE_TOKEN:  return "i8*";
            case U8_TYPE_TOKEN:  return "u8*";
            case I16_TYPE_TOKEN: return "i16*";
            case U16_TYPE_TOKEN: return "u16*";
            case I32_TYPE_TOKEN: return "i32*";
            case U32_TYPE_TOKEN: return "u32*";
            case F32_TYPE_TOKEN: return "f32*";
            case I64_TYPE_TOKEN: return "i64*";
            case U64_TYPE_TOKEN: return "u64*";
            case F64_TYPE_TOKEN: return "f64*";
            case STR_TYPE_TOKEN: return "str*";
            default: return "";
        }
    }
    switch (t->t_type) {
        case I0_TYPE_TOKEN:  return "i0";
        case I8_TYPE_TOKEN:  return "i8";
        case U8_TYPE_TOKEN:  return "u8";
        case I16_TYPE_TOKEN: return "i16";
        case U16_TYPE_TOKEN: return "u16";
        case I32_TYPE_TOKEN: return "i32";
        case U32_TYPE_TOKEN: return "u32";
        case F32_TYPE_TOKEN: return "f32";
        case I64_TYPE_TOKEN: return "i64";
        case U64_TYPE_TOKEN: return "u64";
        case F64_TYPE_TOKEN: return "f64";
        case STR_TYPE_TOKEN: return "str";
        default: return "";
    }
}

static inline int print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("   ");
    if (node->token && node->token->t_type != SCOPE_TOKEN) {
        printf(
            "[%s%s] (t=%d,%s%s%sv_id=%i, s_id=%i%s%s%s%s)\n",
            node->token->flags.neg ? "{not} " : "",
            node->token->body->body, node->token->t_type, 
            node->token->flags.ptr ? " ptr, " : " ",
            node->token->flags.ref ? "ref, " : "",
            node->token->flags.dref ? "dref, " : "",
            node->sinfo.v_id, node->sinfo.s_id,
            node->token->flags.ro ? ", ro" : "",
            node->token->flags.ext ? ", ext" : "",
            node->token->flags.glob ? ", glob" : "",
            node->token->flags.heap ? ", heap" : ""
        );
    }
    else if (node->token && node->token->t_type == SCOPE_TOKEN) {
        printf("{ scope, id=%i }\n", node->sinfo.s_id);
    }
    else {
        printf("[ block ]\n");
    }
    
    ast_node_t* child = node->child;
    while (child) {
        print_ast(child, depth + 1);
        child = child->sibling;
    }
    
    return 1;
}
#endif