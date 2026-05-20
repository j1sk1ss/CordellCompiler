#ifndef HIR_HELPER_H_
#define HIR_HELPER_H_
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/func.h>
#include <hir/loop.h>
#include <hir/hir_types.h>
#include <hir/dump.h>

static const char* _hir_op_to_string(hir_operation_t op) {
    switch(op) {
        case HIR_SETPOS:       return "HIR_SETPOS";
        case HIR_PHI:          return "PHI";
        case HIR_STARGLD:      return "STARGLD";
        case HIR_FARGLD:       return "FARGLD";
        case HIR_UFCLL:        return "HIR_UFCLL";
        case HIR_STORE_UFCLL:  return "HIR_STORE_UFCLL";
        case HIR_FCLL:         return "FCLL";
        case HIR_STORE_FCLL:   return "HIR_STORE_FCLL";
        case HIR_ECLL:         return "ECLL";
        case HIR_STORE_ECLL:   return "HIR_STORE_ECLL";
        case HIR_STRT:         return "STRT";
        case HIR_SYSC:         return "SYSC";
        case HIR_STORE_SYSC:   return "HIR_STORE_SYSC";
        case HIR_FRET:         return "FRET";
        case HIR_MKLB:         return "MKLB";
        case HIR_FDCL:         return "FDCL";
        case HIR_FEND:         return "FEND";
        case HIR_FEXT:         return "FEXT";
        case HIR_OEXT:         return "OEXT";
        case HIR_BREAK:        return "HIR_BREAK";
        case HIR_JMP:          return "JMP";
        case HIR_iADD:         return "iADD";
        case HIR_iSUB:         return "iSUB";
        case HIR_iMUL:         return "iMUL";
        case HIR_iDIV:         return "iDIV";
        case HIR_iMOD:         return "iMOD";
        case HIR_iLRG:         return "iLRG";
        case HIR_iLGE:         return "iLGE";
        case HIR_iLWR:         return "iLWR";
        case HIR_iLRE:         return "iLRE";
        case HIR_iCMP:         return "iCMP";
        case HIR_iNMP:         return "iNMP";
        case HIR_iAND:         return "iAND";
        case HIR_iOR:          return "iOR";
        case HIR_iBLFT:        return "iBLFT";
        case HIR_iBRHT:        return "iBRHT";
        case HIR_bAND:         return "bAND";
        case HIR_bOR:          return "bOR";
        case HIR_bXOR:         return "bXOR";
        case HIR_RAW:          return "RAW";
        case HIR_IFOP2:        return "IFOP2";
        case HIR_NOT:          return "NOT";
        case HIR_STORE:        return "STORE";
        case HIR_VARDECL:      return "VARDECL";
        case HIR_ARRDECL:      return "ARRDECL";
        case HIR_VRUSE:        return "VRUSE";
        case HIR_STRDECL:      return "STRDECL";
        case HIR_STASM:        return "STASM";
        case HIR_ENDASM:       return "ENDASM";
        case HIR_GDREF:        return "GDREF";
        case HIR_LDREF:        return "LDREF";
        case HIR_REF:          return "REF";
        case HIR_EXITOP:       return "EXITOP";
        case HIR_STEND:        return "HIR_STEND";
        case HIR_MKSCOPE:      return "MKSCOPE";
        case HIR_ENDSCOPE:     return "ENDSCOPE";
        case HIR_TF64:         return "HIR_TF64";
        case HIR_TF32:         return "HIR_TF32"; 
        case HIR_TI64:         return "HIR_TI64";
        case HIR_TI32:         return "HIR_TI32";
        case HIR_TI16:         return "HIR_TI16";
        case HIR_TI8:          return "HIR_TI8";  
        case HIR_TPTR:         return "HIR_TPTR";
        case HIR_TU64:         return "HIR_TU64";
        case HIR_TU32:         return "HIR_TU32";
        case HIR_TU16:         return "HIR_TU16";
        case HIR_TU8:          return "HIR_TU8";
        case HIR_PHI_PREAMBLE: return "PHI_PREAMBLE";
        case HIR_BREAKPOINT:   return "BREAKPOINT";
        default: return "";
    }
}

/* https://dreampuf.github.io/GraphvizOnline/?engine=dot */
static void _dump_domtree_dot_rec(cfg_block_t* b) {
    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        printf("  B%li -> B%li;\n", b->id, c->id);
        _dump_domtree_dot_rec(c);
    }
}

static void _dump_domtree_dot(cfg_func_t* func) {
    printf("digraph DomTree {\n");
    printf("  rankdir=TB;\n");

    foreach (cfg_block_t* cb, &func->blocks) {
        printf("  B%li [label=\"B%li\"];\n", cb->id, cb->id);
    }

    foreach (cfg_block_t* cb, &func->blocks) {
        if (!cb->sdom) {
            _dump_domtree_dot_rec(cb);
        }
    }

    printf("}\n");
}

static void _dump_all_dom_dot(cfg_func_t* func) {
    printf("digraph AllDom {\n");
    printf("  rankdir=TB;\n");

    foreach (cfg_block_t* cb, &func->blocks) {
        printf("  B%li [label=\"B%li\"];\n", cb->id, cb->id);
    }

    foreach (cfg_block_t* cb, &func->blocks) {
        set_foreach (cfg_block_t* d, &cb->dom) {
            if (d == cb) continue;
            printf("  B%li -> B%li;\n", d->id, cb->id);
        }
    }

    printf("}\n");
}

static void _print_set_int(FILE* out, set_t* s) {
    int first = 1;
    fprintf(out, "{");
    set_foreach (long v, s) {
        if (!first) fprintf(out, ",");
        fprintf(out, "%ld", v);
        first = 0;
    }

    fprintf(out, "}");
}

int export_dot_func_hir(cfg_func_t* f) {
    printf("digraph CFG_func%ld {\n", f->id);
    printf("  rankdir=TB;\n");
    printf("  node [shape=box, fontname=\"monospace\"];\n");

    int ishead = 1;
    foreach (cfg_block_t* cb, &f->blocks) {
        printf("  B%ld [label=\"B%ld:\\nentry=%s%i\\nexit=%s%s",
               cb->id, cb->id,
               cb->hmap.entry ? _hir_op_to_string(cb->hmap.entry->op) : "NULL", 
               (cb->hmap.entry && cb->hmap.entry->op == HIR_MKLB) ? (int)cb->hmap.entry->farg->id : -1,
               cb->hmap.exit  ? _hir_op_to_string(cb->hmap.exit->op)  : "NULL",
               ishead ? "\\nHEAD" : "");

        ishead = 0;
        printf("\\nIN=");  _print_set_int(stdout, &cb->curr_in);
        printf("\\nDEF="); _print_set_int(stdout, &cb->def);
        printf("\\nUSE="); _print_set_int(stdout, &cb->use);
        printf("\\nOUT="); _print_set_int(stdout, &cb->curr_out);
        printf("\\nPREDS=%i", set_size(&cb->pred)); 

        printf("\"];\n");

        if (cb->l)   printf("  B%ld -> B%ld [label=\"fall\"];\n", cb->id, cb->l->id);
        if (cb->jmp) printf("  B%ld -> B%ld [label=\"jump\"];\n", cb->id, cb->jmp->id);
    }
    
    printf("}\n");
    return 1;
}

static void cfg_print(cfg_ctx_t* ctx) {
    printf("==== CFG DUMP ====\n");
    foreach (cfg_func_t* fb, &ctx->funcs) {
        printf("==== CFG DOT (HIR) ====\n");
        export_dot_func_hir(fb);
        printf("==== DOM DOT ====\n");
        _dump_all_dom_dot(fb);
        printf("==== SDOM DOT ====\n");
        _dump_domtree_dot(fb);
    }

    printf("==================\n");
}

static void call_graph_print_dot(call_graph_t* cg) {
    if (!cg) {
        printf("[call_graph_print_dot] Empty call graph.\n");
        return;
    }

    printf("digraph CALL_GRAPH {\n");
    printf("  rankdir=LR;\n");
    printf("  node [shape=ellipse, fontname=\"monospace\"];\n");

    map_foreach (call_graph_node_t* node, &cg->verts) {
        set_foreach (call_graph_node_t* callee, &node->edges) {
            printf("  F%ld -> F%ld;\n", node->f_id, callee->f_id);
        }
    }

    printf("}\n");
}
#endif
