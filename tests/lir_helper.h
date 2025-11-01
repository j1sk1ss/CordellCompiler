#ifndef LIR_HELPER_H_
#define LIR_HELPER_H_

#include <lir/lir.h>
#include <lir/lir_types.h>

static int _lir_depth = 0;
static const char* lir_op_to_fmtstring(lir_operation_t op, int state) {
    switch(op) {
        case LIR_ECLL: 
        case LIR_FCLL:  return "call %s;\n";
        case LIR_STRT:  return "start {\n";
        case LIR_STEND: return "}\n";
        case LIR_SYSC:  return "syscall;\n";
        case LIR_FRET:  return "return %s;\n";
        case LIR_MKLB:  return "%s:\n";
        case LIR_FDCL:  return "fn %s\n";
        case LIR_FEND:  return "\n";
        case LIR_OEXT:  return "extern %s;\n";

        case LIR_JE:    return "je %s;\n";
        case LIR_JMP:   return "jmp %s;\n";
        case LIR_iMOV:  return "%s = %s;\n";

        case LIR_STARGLD: return "%s = strt_loadarg();\n";
        case LIR_STARGRF: return "%s = strt_ref_loadarg();\n";

        case LIR_VRDEALL: return "kill(%s);\n";
        case LIR_STRDECL: return "%s = str_alloc(%s);\n";
        case LIR_ARRDECL: return "%s = arr_alloc(X);\n";

        case LIR_STFARG:   return "stparam(%s);\n";
        case LIR_LOADFARG: return "%s = ldparam();\n";
        case LIR_LOADFRET: return "%s = fret();\n";

        case LIR_TF64: return "%s = %s as f64;\n";
        case LIR_TF32: return "%s = %s as f32;\n";
        case LIR_TI64: return "%s = %s as i64;\n";
        case LIR_TI32: return "%s = %s as i32;\n";
        case LIR_TI16: return "%s = %s as i16;\n";
        case LIR_TI8:  return "%s = %s as i8;\n";
        case LIR_TU64: return "%s = %s as u64;\n";
        case LIR_TU32: return "%s = %s as u32;\n";
        case LIR_TU16: return "%s = %s as u16;\n";
        case LIR_TU8:  return "%s = %s as u8;\n";

        case LIR_NOT: return "%s = !%s;\n";

        case LIR_IFOP2:  return "if %s == %s;\n";
        case LIR_iBLFT: return "%s = %s << %s;\n";
        case LIR_iBRHT: return "%s = %s >> %s;\n";
        case LIR_iLWR:  return "%s = %s < %s;\n";
        case LIR_iLRE:  return "%s = %s <= %s;\n";
        case LIR_iLRG:  return "%s = %s > %s;\n";
        case LIR_iLGE:  return "%s = %s >= %s;\n";
        case LIR_iCMP:  return "%s = %s == %s;\n";
        case LIR_iNMP:  return "%s = %s != %s;\n";
        case LIR_iOR:   return "%s = %s || %s;\n";
        case LIR_iAND:  return "%s = %s && %s;\n";
        case LIR_bOR:   return "%s = %s | %s;\n";
        case LIR_bXOR:  return "%s = %s ^ %s;\n";
        case LIR_bAND:  return "%s = %s & %s;\n";
        case LIR_iMOD:  return "%s = %s %% %s;\n";
        case LIR_iSUB:  return "%s = %s - %s;\n";
        case LIR_iDIV:  return "%s = %s / %s;\n";
        case LIR_iMUL:  return "%s = %s * %s;\n";
        case LIR_iADD:  return "%s = %s + %s;\n";

        case LIR_REF:   return "%s = &(%s);\n";
        case LIR_GDREF: return "%s = *(%s);\n";
        case LIR_LDREF: return "*(%s) = %s;\n";

        case LIR_RAW:        return "[raw] (link: %s);\n";
        case LIR_BREAKPOINT: return "== == brk == ==\n";

        case LIR_MKSCOPE:  return "{\n";
        case LIR_ENDSCOPE: return "}\n";
    
        case LIR_EXITOP: return "exit %s;\n";
    }
}

static char* sprintf_lir_subject(char* dst, lir_subject_t* s, sym_table_t* smt) {
    if (!s) return dst;
    switch (s->t) {
        case LIR_VARIABLE: dst += sprintf(dst, "%%%i", s->storage.var.v_id);        break;
        case LIR_NUMBER:   dst += sprintf(dst, "num: %s", s->storage.num.value);    break;
        case LIR_CONSTVAL: dst += sprintf(dst, "cnst: %ld", s->storage.cnst.value); break;

        case LIR_LABEL: dst += sprintf(dst, "lb%d", s->storage.lb.lb_id); break;
        case LIR_RAWASM:
        case LIR_STRING: {
            str_info_t si;
            if (STTB_get_info_id(s->storage.str.sid, &si, &smt->s)) {
                dst += sprintf(dst, "str(%s)", si.value);
            }

            break;
        }

        case LIR_FNAME: {
            func_info_t fi;
            if (FNTB_get_info_id(s->storage.str.sid, &fi, &smt->f)) {
                dst += sprintf(dst, "%s(", fi.name);
            }

            if (fi.args) {
                for (ast_node_t* t = fi.args->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
                    ast_node_t* type = t;
                    ast_node_t* name = t->child;
                    dst += sprintf(dst, "%s %s", fmt_tkn_type(type->token), name->token->value);
                    if (t->sibling && t->sibling->token->t_type != SCOPE_TOKEN) dst += sprintf(dst, ", ");
                }
            }

            dst += sprintf(dst, ")");
            if (fi.rtype) {
                dst += sprintf(dst, " -> %s", fmt_tkn_type(fi.rtype->token));
            }

            break;
        }

        default: dst += sprintf(dst, "unknw"); break;
    }

    return dst;
}

void print_lir_block(const lir_block_t* block, int ud, sym_table_t* smt) {
    if (!block) return;

    char arg1[256] = { 0 };
    char arg2[256] = { 0 };
    char arg3[256] = { 0 };

    int args = 3;
    if (block->farg) {
        sprintf_lir_subject(arg1, block->farg, smt);
        args--;
    }

    if (block->sarg) {
        sprintf_lir_subject(arg2, block->sarg, smt);
        args--;
    }

    if (block->targ) {
        sprintf_lir_subject(arg3, block->targ, smt);
        args--;
    }

    char line[512] = { 0 };
    if (
        block->op == LIR_ENDSCOPE ||
        // block->op == HIR_FEND     ||
        block->op == LIR_STEND
    ) _lir_depth--;

    if (ud) for (int i = 0; i < _lir_depth; i++) printf("    ");
    if (block->unused) printf("[unused] ");
    const char* fmt = lir_op_to_fmtstring(block->op, args);
    sprintf(line, fmt, arg1, arg2, arg3);

    if (
        block->op == LIR_MKSCOPE ||
        // block->op == HIR_FDCL    ||
        block->op == LIR_STRT
    ) _lir_depth++;

    fprintf(stdout, "%s", line);
}

// static int _export_dot_func_lir(cfg_func_t* f, sym_table_t* smt) {
//     printf("digraph CFG_func%d {\n", f->id);
//     printf("  rankdir=TB;\n");
//     printf("  node [shape=box, fontname=\"monospace\"];\n");

//     int ishead = 1;

//     list_iter_t bit;
//     list_iter_hinit(&f->blocks, &bit);
//     cfg_block_t* cb;
//     while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
//         printf("  B%ld [label=\"B%ld:\\n", cb->id, cb->id);
//         lir_block_t* cmd = cb->lmap.entry;
//         while (cmd) {
//             const char* fmt_cmd = print_lir_block(cmd, 0, smt);
//             if (fmt_cmd) printf("%s\\n", fmt_cmd);
//             if (cmd == cb->lmap.exit) break;
//             cmd = cmd->next;
//         }

//         printf("\"];\n");

//         if (cb->l)   printf("  B%ld -> B%ld [label=\"fall\"];\n", cb->id, cb->l->id);
//         if (cb->jmp) printf("  B%ld -> B%ld [label=\"jump\"];\n", cb->id, cb->jmp->id);
//     }
    
//     printf("}\n");
//     return 1;
// }
#endif